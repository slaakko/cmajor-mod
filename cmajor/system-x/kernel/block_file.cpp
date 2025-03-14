// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.block.file;

import cmajor.systemx.kernel.block.manager;
import cmajor.systemx.kernel.fs;
import cmajor.systemx.kernel.mount;
import util;

namespace cmajor::systemx::kernel {

BlockFile::BlockFile(const std::string& name_, INodeKey inodeKey_) : File(name_), inodeKey(inodeKey_), filePos(0), referenceCount(1)
{
}

File* BlockFile::Share()
{
    ++referenceCount;
    return this;
}

void BlockFile::Release(cmajor::systemx::kernel::Process* process)
{
    --referenceCount;
    if (referenceCount == 0)
    {
        delete this;
    }
}

std::vector<uint8_t> BlockFile::Read(int64_t count, cmajor::systemx::machine::Process* process)
{
    std::vector<uint8_t> data = File::Read(count, process);
    INodePtr inodePtr(nullptr);
    INode* inode = nullptr;
    if (count > 0)
    {
        inodePtr = GetINode(process);
        inode = inodePtr.Get();
    }
    while (count > 0)
    {
        int32_t blockNumber = GetBlockNumber(inode, process, false);
        int32_t blockOffset = filePos % Block::Size();
        int64_t bytesToRead = std::min(count, static_cast<int64_t>(Block::Size()) - blockOffset);
        int64_t bytesLeft = std::max(inode->FileSize() - filePos, static_cast<int64_t>(0));
        bytesToRead = std::min(bytesToRead, bytesLeft);
        if (bytesToRead == 0)
        {
            break;
        }
        if (blockNumber == -1)
        {
            for (int64_t i = 0; i < bytesToRead; ++i)
            {
                data.push_back(0u);
            }
        }
        else
        {
            BlockKey blockKey(inodeKey.fsNumber, blockNumber);
            BlockPtr blockPtr = GetBlock(blockKey, process);
            Block* block = blockPtr.Get();
            if (!block->IsValid())
            {
                Read(block, process);
            }
            for (int64_t i = 0; i < bytesToRead; ++i)
            {
                data.push_back(block->Data()[blockOffset + i]);
            }
        }
        count -= bytesToRead;
        filePos += bytesToRead;
    }
    if (inode)
    {
        inode->SetATime(util::GetCurrentDateTime());
        WriteINode(inode, process);
    }
    return data;
}

void foo(std::vector<uint8_t>& buf);

int64_t BlockFile::Write(const std::vector<uint8_t>& buffer, cmajor::systemx::machine::Process* process)
{
    int64_t bytesWritten = File::Write(buffer, process);
    INodePtr inodePtr(nullptr);
    INode* inode = nullptr;
    int64_t count = buffer.size();
    int32_t inodeNumBlocks = 0;
    if (count > 0)
    {
        inodePtr = GetINode(process);
        inode = inodePtr.Get();
    }
    while (count > 0)
    {
        int32_t logicalBlockNumber = static_cast<int32_t>(filePos / Block::Size());
        int32_t blockNumber = GetBlockNumber(inode, process, true);
        int32_t blockOffset = filePos % Block::Size();
        int64_t bytesToWrite = std::min(count, static_cast<int64_t>(Block::Size()) - blockOffset);
        BlockKey blockKey(inodeKey.fsNumber, blockNumber);
        BlockPtr blockPtr = GetBlock(blockKey, process);
        Block* block = blockPtr.Get();
        if (inode->NumberOfBlocks() > logicalBlockNumber && (blockOffset != 0 || bytesToWrite != Block::Size()))
        {
            Read(block, process);
        }
        else
        {
            block->Clear();
        }
        for (int64_t i = 0; i < bytesToWrite; ++i)
        {
            block->Data()[blockOffset + i] = buffer[i];
        }
        block->SetDirty();
        block->SetValid();
        Write(block, process);
        count -= bytesToWrite;
        filePos += bytesToWrite;
        bytesWritten += bytesToWrite;
        if (filePos > inode->FileSize())
        {
            inode->SetFileSize(filePos);
            inode->SetInternalFileSize(filePos);
            inode->SetCTime(util::GetCurrentDateTime());
            WriteINode(inode, process);
        }
    }
    if (inode)
    {
        inode->SetMTime(util::GetCurrentDateTime());
        WriteINode(inode, process);
    }
    return bytesWritten;
}

int64_t BlockFile::Seek(int64_t offset, Origin whence, cmajor::systemx::machine::Process* process)
{
    if (whence == Origin::seekSet)
    {
        SetFilePos(offset);
        return offset;
    }
    else if (whence == Origin::seekCur)
    {
        int64_t filePos = FilePos() + offset;
        SetFilePos(filePos);
        return filePos;
    }
    else if (whence == Origin::seekEnd)
    {
        INodePtr inode = GetINode(process);
        int64_t fileSize = inode.Get()->FileSize();
        int64_t filePos = fileSize + offset;
        SetFilePos(filePos);
        return filePos;
    }
    else
    {
        return 0;
    }
}

int64_t BlockFile::Tell(cmajor::systemx::machine::Process* process)
{
    return filePos;
}

} // namespace cmajor::systemx::kernel
