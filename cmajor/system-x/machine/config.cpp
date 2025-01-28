// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.machine.config;

import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;
import std.filesystem;

namespace {

std::string CmajorConfigDir()
{
    return util::Path::Combine(util::CmajorRoot(), "config");
}

} // namespace

namespace cmajor::systemx::machine {

class Configuration
{
public:
    static Configuration& Instance();
    std::string FilePath() const { return filePath; }
    int NumProcessors() const { return numProcessors; }
    int NumLocalRegs() const { return numLocalRegs; }
    int ClockTickMilliseconds() const { return clockTickMilliseconds; }
    int MaxProcs() const { return maxProcs; }
    int MaxOpenFiles() const { return maxOpenFiles; }
    int MaxFilesystems() const { return maxFilesystems; }
    int NumCachedBlocks() const { return numCachedBlocks; }
    int NumBlockHashQueues() const { return numBlockHashQueues; }
    int NumCachedINodes() const { return numCachedINodes; }
    int NumINodeHashQueues() const { return numINodeHashQueues; }
    int KernelStackSize() const { return kernelStackSize; }
    int RootFSNumBlocks() const { return rootFSNumBlocks; }
    int RootFSMaxFiles() const { return rootFSMaxFiles; }
    int32_t UMask() const { return umask; }
    int32_t UID() const { return uid; }
    int32_t GID() const { return gid; }
private:
    Configuration();
    void Read();
    void Write();
    std::string filePath;
    int numProcessors;
    int numLocalRegs;
    int clockTickMilliseconds;
    int maxProcs;
    int maxOpenFiles;
    int maxFilesystems;
    int numCachedBlocks;
    int numBlockHashQueues;
    int numCachedINodes;
    int numINodeHashQueues;
    int kernelStackSize;
    int rootFSNumBlocks;
    int rootFSMaxFiles;
    int32_t umask;
    int32_t uid;
    int32_t gid;
    static std::unique_ptr<Configuration> instance;
};

Configuration& Configuration::Instance()
{
    static Configuration instance;
    return instance;
}

Configuration::Configuration() :
    filePath(util::Path::Combine(CmajorConfigDir(), "sx.machine.config.xml")),
    numProcessors(1),
    numLocalRegs(8),
    clockTickMilliseconds(250),
    maxProcs(1024),
    maxOpenFiles(256),
    maxFilesystems(64),
    numCachedBlocks(256),
    numBlockHashQueues(16),
    numCachedINodes(256),
    numINodeHashQueues(16),
    kernelStackSize(16384),
    rootFSNumBlocks(16384),
    rootFSMaxFiles(1024),
    umask(0),
    uid(1),
    gid(0)
{
    if (!std::filesystem::exists(filePath))
    {
        Write();
    }
    Read();
}

void Configuration::Read()
{
    std::unique_ptr<soul::xml::Document> configDoc = soul::xml::ParseXmlFile(filePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("configuration", configDoc.get());
    int n = nodeSet->Count();
    if (n == 1)
    {
        soul::xml::Node* node = nodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* configurationElement = static_cast<soul::xml::Element*>(node);
            std::string numProcessorsAttribute = configurationElement->GetAttribute("numProcessors");
            if (numProcessorsAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'numProcessors' attribute not found");
            }
            numProcessors = std::stoi(util::ToUtf8(numProcessorsAttribute));
            std::string numLocalRegsAttribute = configurationElement->GetAttribute("numLocalRegs");
            if (numLocalRegsAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'numLocalRegs' attribute not found");
            }
            numLocalRegs = std::stoi(util::ToUtf8(numLocalRegsAttribute));
            std::string clockTickMillisecondsAttribute = configurationElement->GetAttribute("clockTickMilliseconds");
            if (clockTickMillisecondsAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'clockTickMilliseconds' attribute not found");
            }
            clockTickMilliseconds = std::stoi(util::ToUtf8(clockTickMillisecondsAttribute));
            std::string maxProcsAttribute = configurationElement->GetAttribute("maxProcs");
            if (maxProcsAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'maxProcs' attribute not found");
            }
            maxProcs = std::stoi(util::ToUtf8(maxProcsAttribute));
            std::string maxOpenFilesAttribute = configurationElement->GetAttribute("maxOpenFiles");
            if (maxOpenFilesAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'maxOpenFiles' attribute not found");
            }
            maxOpenFiles = std::stoi(util::ToUtf8(maxOpenFilesAttribute));
            std::string maxFilesystemsAttribute = configurationElement->GetAttribute("maxFilesystems");
            if (maxFilesystemsAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'maxFilesystems' attribute not found");
            }
            maxFilesystems = std::stoi(util::ToUtf8(maxFilesystemsAttribute));
            std::string numCachedBlocksAttribute = configurationElement->GetAttribute("numCachedBlocks");
            if (numCachedBlocksAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'numCachedBlocks' attribute not found");
            }
            numCachedBlocks = std::stoi(util::ToUtf8(numCachedBlocksAttribute));
            std::string numBlockHashQueuesAttribute = configurationElement->GetAttribute("numBlockHashQueues");
            if (numBlockHashQueuesAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'numBlockHashQueues' attribute not found");
            }
            numBlockHashQueues = std::stoi(util::ToUtf8(numBlockHashQueuesAttribute));
            std::string numCachedINodesAttribute = configurationElement->GetAttribute("numCachedINodes");
            if (numCachedINodesAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'numCachedINodes' attribute not found");
            }
            numCachedINodes = std::stoi(util::ToUtf8(numCachedINodesAttribute));
            std::string numINodeHashQueuesAttribute = configurationElement->GetAttribute("numINodeHashQueues");
            if (numINodeHashQueuesAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'numINodeHashQueues' attribute not found");
            }
            numINodeHashQueues = std::stoi(util::ToUtf8(numINodeHashQueuesAttribute));
            std::string kernelStackSizeAttribute = configurationElement->GetAttribute("kernelStackSize");
            if (kernelStackSizeAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'kernelStackSize' attribute not found");
            }
            kernelStackSize = std::stoi(util::ToUtf8(kernelStackSizeAttribute));
            std::string rootFSNumBlocksAttribute = configurationElement->GetAttribute("rootFSNumBlocks");
            if (rootFSNumBlocksAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'rootFSNumBlocks' attribute not found");
            }
            rootFSNumBlocks = std::stoi(util::ToUtf8(rootFSNumBlocksAttribute));
            std::string rootFSMaxFilesAttribute = configurationElement->GetAttribute("rootFSMaxFiles");
            if (rootFSMaxFilesAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'rootFSMaxFiles' attribute not found");
            }
            rootFSMaxFiles = std::stoi(util::ToUtf8(rootFSMaxFilesAttribute));
            std::string umaskAttribute = configurationElement->GetAttribute("umask");
            if (umaskAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'umask' attribute not found");
            }
            if (umaskAttribute.length() != 3 ||
                umaskAttribute[0] < '0' || umaskAttribute[0] > '7' ||
                umaskAttribute[1] < '0' || umaskAttribute[1] > '7' ||
                umaskAttribute[2] < '0' || umaskAttribute[2] > '7')
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': invalid 'umask' attribute: three octal digits expected");
            }
            umask = util::ParseOctal(util::ToUtf8(umaskAttribute));
            std::string uidAttribute = configurationElement->GetAttribute("uid");
            if (uidAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'uid' attribute not found");
            }
            uid = std::stoi(util::ToUtf8(uidAttribute));
            std::string gidAttribute = configurationElement->GetAttribute("gid");
            if (gidAttribute.empty())
            {
                throw std::runtime_error("error reading configuration from '" + filePath + "': 'gid' attribute not found");
            }
            gid = std::stoi(util::ToUtf8(gidAttribute));
        }
    }
    else
    {
        throw std::runtime_error("error reading configuration from '" + filePath + "': single 'configuration' element expected");
    }
}

void Configuration::Write()
{
    std::ofstream file(filePath);
    soul::xml::Document configDoc;
    soul::xml::Element* configurationElement = soul::xml::MakeElement("configuration");
    configurationElement->SetAttribute("numProcessors", std::to_string(numProcessors));
    configurationElement->SetAttribute("numLocalRegs", std::to_string(numLocalRegs));
    configurationElement->SetAttribute("clockTickMilliseconds", std::to_string(clockTickMilliseconds));
    configurationElement->SetAttribute("maxProcs", std::to_string(maxProcs));
    configurationElement->SetAttribute("maxOpenFiles", std::to_string(maxOpenFiles));
    configurationElement->SetAttribute("maxFilesystems", std::to_string(maxFilesystems));
    configurationElement->SetAttribute("numCachedBlocks", std::to_string(numCachedBlocks));
    configurationElement->SetAttribute("numBlockHashQueues", std::to_string(numBlockHashQueues));
    configurationElement->SetAttribute("numCachedINodes", std::to_string(numCachedINodes));
    configurationElement->SetAttribute("numINodeHashQueues", std::to_string(numINodeHashQueues));
    configurationElement->SetAttribute("kernelStackSize", std::to_string(kernelStackSize));
    configurationElement->SetAttribute("rootFSNumBlocks", std::to_string(rootFSNumBlocks));
    configurationElement->SetAttribute("rootFSMaxFiles", std::to_string(rootFSMaxFiles));
    configurationElement->SetAttribute("umask", util::ToOctalString(umask, 3));
    configurationElement->SetAttribute("uid", std::to_string(uid));
    configurationElement->SetAttribute("gid", std::to_string(gid));
    configDoc.AppendChild(configurationElement);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    configDoc.Write(formatter);
}

int NumProcessors()
{
    return Configuration::Instance().NumProcessors();
}

int NumLocalRegs()
{
    return Configuration::Instance().NumLocalRegs();
}

int ClockTickMilliseconds()
{
    return Configuration::Instance().ClockTickMilliseconds();
}

int MaxProcs()
{
    return Configuration::Instance().MaxProcs();
}

int MaxOpenFiles()
{
    return Configuration::Instance().MaxOpenFiles();
}

int MaxFilesystems()
{
    return Configuration::Instance().MaxFilesystems();
}

int NumCachedBlocks()
{
    return Configuration::Instance().NumCachedBlocks();
}

int NumBlockHashQueues()
{
    return Configuration::Instance().NumBlockHashQueues();
}

int NumCachedINodes()
{
    return Configuration::Instance().NumCachedINodes();
}

int NumINodeHashQueues()
{
    return Configuration::Instance().NumINodeHashQueues();
}

int KernelStackSize()
{
    return Configuration::Instance().KernelStackSize();
}

int RootFSNumBlocks()
{
    return Configuration::Instance().RootFSNumBlocks();
}

int RootFSMaxFiles()
{
    return Configuration::Instance().RootFSMaxFiles();
}

int32_t UID()
{
    return Configuration::Instance().UID();
}

int32_t GID()
{
    return Configuration::Instance().GID();
}

int32_t UMask()
{
    return Configuration::Instance().UMask();
}

std::string ConfigFilePath()
{
    return Configuration::Instance().FilePath();
}

} // namespace cmajor::systemx::machine
