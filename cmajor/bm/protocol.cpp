// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binary.message.protocol;

namespace cmajor::bmp {

BinaryMessage::BinaryMessage(uint32_t id_) : id(id_)
{
}

BinaryMessage::~BinaryMessage()
{
}

MessageRegistry& MessageRegistry::Instance()
{
    static MessageRegistry instance;
    return instance;
}

MessageRegistry::MessageRegistry()
{
}

BinaryMessage* MessageRegistry::Create(uint32_t messageId)
{
    auto it = factoryMap.find(messageId);
    if (it != factoryMap.cend())
    {
        FactoryFunction factoryFunction = it->second;
        return factoryFunction(messageId);
    }
    else
    {
        throw std::runtime_error("cmajor.binary.message.protocol: error: message id '" + std::to_string(messageId) + "' not registered");
    }
}

size_t GetHashCode(const std::string& s)
{
    size_t hashCode = 14695981039346656037u;
    for (char c : s)
    {
        hashCode = hashCode ^ static_cast<size_t>(static_cast<unsigned char>(c));
        hashCode = hashCode * 1099511628211u;
    }
    return hashCode;
}

uint32_t MakeMessageId(size_t hashCode)
{
    uint32_t messageId = static_cast<uint32_t>(1 + (hashCode % 0xFFFFFFFFull));
    return messageId;
}

BinaryMessage* ReadMessage(util::TcpSocket& socket)
{
    uint32_t messageLength = 0;
    uint8_t buffer[sizeof(messageLength)];
    int offset = 0;
    int bytesToReceive = sizeof(messageLength);
    int bytesReceived = socket.Receive(&buffer[offset], bytesToReceive);
    if (bytesReceived == 0)
    {
        return nullptr;
    }
    bytesToReceive = bytesToReceive - bytesReceived;
    offset = offset + bytesReceived;
    while (bytesToReceive > 0)
    {
        bytesReceived = socket.Receive(&buffer[offset], bytesToReceive);
        if (bytesReceived == 0)
        {
            return nullptr;
        }
        bytesToReceive = bytesToReceive - bytesReceived;
        offset = offset + bytesReceived;
    }
    util::MemoryReader lengthReader(&buffer[0], sizeof(messageLength));
    messageLength = lengthReader.ReadUInt();
    std::unique_ptr<uint8_t[]> mem(new uint8_t[messageLength + 4]);
    offset = 0;
    bytesToReceive = messageLength + 4;
    bytesReceived = socket.Receive(mem.get() + offset, bytesToReceive);
    if (bytesReceived == 0)
    {
        return nullptr;
    }
    bytesToReceive = bytesToReceive - bytesReceived;
    offset = offset + bytesReceived;
    while (bytesToReceive > 0)
    {
        bytesReceived = socket.Receive(mem.get() + offset, bytesToReceive);
        if (bytesReceived == 0)
        {
            return nullptr;
        }
        bytesToReceive = bytesToReceive - bytesReceived;
        offset = offset + bytesReceived;
    }
    util::MemoryReader messageReader(mem.get(), messageLength + 4);
    uint32_t messageId = messageReader.ReadUInt();
    BinaryMessage* message = MessageRegistry::Instance().Create(messageId);
    message->Read(messageReader);
    return message;
}

void WriteMessage(util::TcpSocket& socket, BinaryMessage* message)
{
    uint32_t messageLength = message->Length();
    uint8_t lengthBuffer[sizeof(messageLength)];
    util::MemoryWriter lengthWriter(&lengthBuffer[0], sizeof(messageLength));
    lengthWriter.Write(messageLength);
    socket.Send(&lengthBuffer[0], sizeof(messageLength));
    uint32_t messageId = message->Id();
    uint8_t messageIdBuffer[sizeof(messageId)];
    util::MemoryWriter idWriter(&messageIdBuffer[0], sizeof(messageId));
    idWriter.Write(messageId);
    socket.Send(&messageIdBuffer[0], sizeof(messageId));
    if (messageLength > 0)
    {
        std::unique_ptr<uint8_t[]> mem(new uint8_t[messageLength]);
        util::MemoryWriter messageWriter(mem.get(), messageLength);
        message->Write(messageWriter);
        socket.Send(mem.get(), messageLength);
    }
}

} // namespace cmajor::bmp
