// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binary.message.protocol;

import std.core;
import util;

export namespace cmajor::bmp {

std::string Version() 
{
    return "5.1.0";
}

class BinaryMessage
{
public:
    BinaryMessage(uint32_t id_);
    virtual ~BinaryMessage();
    virtual uint32_t Length() const = 0;
    virtual void Write(util::MemoryWriter& writer) const = 0;
    virtual void Read(util::MemoryReader& reader) = 0;
    uint32_t Id() const { return id; }
private:
    uint32_t id;
};

using FactoryFunction = BinaryMessage* (*)(uint32_t messageId);

size_t GetHashCode(const std::string& s);

uint32_t MakeMessageId(size_t hashCode);

class MessageRegistry
{
public:
    static MessageRegistry& Instance();
    MessageRegistry();
    template<typename T>
    void Register()
    {
        size_t hashCode = GetHashCode(T::ClassName());
        uint32_t messageId = MakeMessageId(hashCode);
        auto it = factoryMap.find(messageId);
        if (it == factoryMap.end())
        {
            factoryMap[messageId] = T::Create;
        }
        else
        {
            throw std::runtime_error("cmajor.binary.message.protocol: error: message id '" + std::to_string(messageId) + "' already in use, please change the message name");
        }
    }
    BinaryMessage* Create(uint32_t messageId);
private:
    std::map<uint32_t, FactoryFunction> factoryMap;
};

template<typename T>
void RegisterMessage()
{
    MessageRegistry::Instance().Register<T>();
}

BinaryMessage* ReadMessage(util::TcpSocket& socket);
void WriteMessage(util::TcpSocket& socket, BinaryMessage* message);

} // namespace cmajor::bmp
