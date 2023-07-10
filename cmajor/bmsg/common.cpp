// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bmsg/common.schema' by cmajor binary protocol message generator version 5.0.0

module cmajor.common.message;

import cmajor.binary.message.io;

namespace common {

SourceLoc::SourceLoc()
     : SourceLoc::SourceLoc(bmpSourceLocId)
{
}

SourceLoc::SourceLoc(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), path(), line()
{
}

std::string SourceLoc::ClassName()
{
    return "common::SourceLoc";
}

void SourceLoc::Register()
{
    cmajor::bmp::RegisterMessage<SourceLoc>();
}

cmajor::bmp::BinaryMessage* SourceLoc::Create(uint32_t messageId)
{
    return new SourceLoc(messageId);
}

uint32_t SourceLoc::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(path);
    length += cmajor::bmp::Length(line);
    return length;
}

void SourceLoc::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, path);
    cmajor::bmp::Write(writer, line);
}

void SourceLoc::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, path);
    cmajor::bmp::Read(reader, line);
}


} // namespace common

namespace {

struct Reg
{
    Reg()
    {
        common::SourceLoc::Register();
    }
};

Reg reg;

} // namespace

