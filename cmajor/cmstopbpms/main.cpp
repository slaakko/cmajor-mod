import cmajor.binary.portmap.message;
import cmajor.binary.message.protocol;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import std.core;
import util;

std::string CmajorRootDir()
{
    char* e = getenv("CMAJOR_ROOT");
    if (e == nullptr || !*e)
    {
        throw std::runtime_error("please set 'CMAJOR_ROOT' environment variable to contain /path/to/cmajor directory.");
    }
    return util::GetFullPath(std::string(e));
}

std::string CmajorConfigDir()
{
    std::string configDir = util::GetFullPath(util::Path::Combine(CmajorRootDir(), "config"));
    std::filesystem::create_directories(configDir);
    return configDir;
}

std::string BinaryPortMapConfigFilePath()
{
    return util::GetFullPath(util::Path::Combine(CmajorConfigDir(), "bpm.config.xml"));
}

void InitApplication()
{
    util::Init();
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        int portMapServicePort = 54421;
        std::string configFilePath = BinaryPortMapConfigFilePath();
        std::unique_ptr<soul::xml::Document> configDoc;
        if (std::filesystem::exists(configFilePath))
        {
            configDoc = soul::xml::ParseXmlFile(configFilePath);
            std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/bpm", configDoc.get());
            soul::xml::Node* node = nodeSet->GetNode(0);
            if (node->IsElementNode())
            {
                soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                std::string portMapServicePortAttribute = element->GetAttribute("portMapServicePort");
                if (!portMapServicePortAttribute.empty())
                {
                    portMapServicePort = std::stoi(portMapServicePortAttribute);
                }
            }
        }
        std::cout << "connecting to binary port map server..." << std::endl;
        util::TcpSocket socket("127.0.0.1", std::to_string(portMapServicePort));
        bpm::StopPortMapServerRequest request;
        std::cout << "sending stop server request..." << std::endl;
        cmajor::bmp::WriteMessage(socket, &request);
        std::unique_ptr<cmajor::bmp::BinaryMessage> reply(cmajor::bmp::ReadMessage(socket));
        if (reply)
        {
            if (reply->Id() == bpm::bmpStopPortMapServerReplyId)
            {
                std::cout << "got stop server reply." << std::endl;
                std::cout << "binary port map server stopped." << std::endl;
            }
            else
            {
                throw std::runtime_error("unknon reply id " + std::to_string(reply->Id()) + " received");
            }
        }
        else
        {
            throw std::runtime_error("got no reply");
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}
