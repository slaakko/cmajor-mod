// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bmsg/common.schema' by cmajor binary protocol message generator version 5.0.0

export module cmajor.common.message;

import std.core;
import util;
import cmajor.binary.message.protocol;
import cmajor.binary.message.number;

export namespace common {

const uint32_t bmpSourceLocId = 2433844479;

class SourceLoc : public cmajor::bmp::BinaryMessage
{
public:
    SourceLoc();
    SourceLoc(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string path;
    int32_t line;
};


} // namespace common
