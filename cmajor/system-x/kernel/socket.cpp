// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.socket;

import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.file;
import cmajor.systemx.kernel.io;
import cmajor.systemx.kernel.process;
import cmajor.systemx.machine;
import util;

namespace cmajor::systemx::kernel {

class Socket : public File
{
public:
    Socket(int64_t socket_);
    void Close(cmajor::systemx::kernel::Process* process) override;
    bool IsShareable() const override { return false; }
    bool IsReadable() const override { return true; }
    bool IsWritable() const override { return true; }
    bool IsConsole() const override { return false; }
    bool IsHostTextFile() const override { return false; }
    bool IsResource() const override { return false; }
    bool IsDecompressionFile() const override { return false; }
    bool HasColors() const override { return false; }
    int Columns() const override { return 0; }
    int Rows() const override { return 0; }
    int CursorX() const { return 0; }
    int CursorY() const { return 0; }
    std::vector<uint8_t> Read(int64_t count, cmajor::systemx::machine::Process* process) override;
    int64_t Write(const std::vector<uint8_t>& buffer, cmajor::systemx::machine::Process* process) override;
private:
    int64_t socket;
};

Socket::Socket(int64_t socket_) : File("SOCKET"), socket(socket_)
{
}

void Socket::Close(cmajor::systemx::kernel::Process* process)
{
    try
    {
        util::CloseSocket(socket);
    }
    catch (const std::runtime_error& ex)
    {
        throw SystemError(ESOCKET, ex.what(), __FUNCTION__);
    }
}

std::vector<uint8_t> Socket::Read(int64_t count, cmajor::systemx::machine::Process* process)
{
    std::vector<uint8_t> buffer(count, 0);
    try
    {
        int result = util::ReceiveSocket(socket, buffer.data(), count, 0);
        std::vector<uint8_t> buf;
        for (int i = 0; i < result; ++i)
        {
            buf.push_back(buffer[i]);
        }
        return buf;
    }
    catch (const std::runtime_error& ex)
    {
        throw SystemError(ESOCKET, ex.what(), __FUNCTION__);
    }
}

int64_t Socket::Write(const std::vector<uint8_t>& buffer, cmajor::systemx::machine::Process* process)
{
    try
    {
        int result = util::SendSocket(socket, buffer.data(), buffer.size(), 0);
        return result;
    }
    catch (const std::runtime_error& ex)
    {
        throw SystemError(ESOCKET, ex.what(), __FUNCTION__);
    }
}

int32_t Connect(Process* process, int64_t nodeAddr, int64_t serviceAddr)
{
    if (nodeAddr == 0)
    {
        throw SystemError(EPARAM, "node is null", __FUNCTION__);
    }
    if (serviceAddr == 0)
    {
        throw SystemError(EPARAM, "service is null", __FUNCTION__);
    }
    try
    {
        cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
        std::string node = ReadString(process, nodeAddr, mem);
        std::string service = ReadString(process, serviceAddr, mem);
        Socket* socket = new Socket(util::ConnectSocket(node, service));
        return process->GetFileTable().AddFile(socket);
    }
    catch (const std::runtime_error& ex)
    {
        throw SystemError(ESOCKET, ex.what(), __FUNCTION__);
    }
}

} // namespace cmajor::systemx::kernel
