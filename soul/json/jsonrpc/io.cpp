// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.io;

import soul.json;
import soul.json.rpc.error;

namespace soul::json::rpc {

std::vector<std::string> ReadHeaders(util::Stream& stream)
{
    std::vector<std::string> headers;
    try
    {
        util::BinaryStreamReader reader(stream);
        std::string header;
        int state = 0;
        bool ready = false;
        while (!ready)
        {
            char c = reader.ReadChar();
            switch (state)
            {
                case 0:
                {
                    switch (c)
                    {
                        case '\r':
                        {
                            state = 1;
                            break;
                        }
                        default:
                        {
                            header.append(1, c);
                            break;
                        }
                    }
                    break;
                }
                case 1:
                {
                    switch (c)
                    {
                        case '\n':
                        {
                            if (header.empty())
                            {
                                ready = true;
                            }
                            else
                            {
                                headers.push_back(header);
                                header.clear();
                            }
                            state = 0;
                        }
                    }
                    break;
                }
            }
        }
        if (state != 0)
        {
            throw JsonRpcException(parseError, "header parse error");
        }
    }
    catch (util::EndOfStreamException& ex)
    {
        return std::vector<std::string>();
    }
    catch (const std::exception& ex)
    {
        throw JsonRpcException(parseError, "header parse error: " + std::string(ex.what()));
    }
    return headers;
}

void ParseHeader(const std::string& header, std::string& headerName, std::string& headerValue)
{
    int state = 0;
    for (char c : header)
    {
        switch (state)
        {
            case 0:
            {
                switch (c)
                {
                    case ':':
                    {
                        state = 1;
                        break;
                    }
                    default:
                    {
                        headerName.append(1, c);
                        break;
                    }
                }
                break;
            }
            case 1:
            {
                switch (c)
                {
                    case ' ':
                    {
                        state = 2;
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                headerValue.append(1, c);
                break;
            }
        }
    }
    if (state != 2)
    {
        throw JsonRpcException(parseError, "header parse error");
    }
}

void ParseHeaders(Context& context, const std::vector<std::string>& headers)
{
    for (const auto& header : headers)
    {
        std::string headerName;
        std::string headerValue;
        ParseHeader(header, headerName, headerValue);
        if (headerName == "Content-Length")
        {
            context.SetContentLength(std::stoi(headerValue));
        }
        else if (headerName == "Content-Type")
        {
            if (headerValue == "utf-8" || headerValue == "utf8")
            {
                context.SetContentType("utf-8");
            }
            else
            {
                throw JsonRpcException(invalidRequest, "unsupported JSON-RPC content type: '" + headerValue + "'");
            }
        }
        else
        {
            throw JsonRpcException(invalidRequest, "unsupported JSON-RPC header: '" + headerName + "'");
        }
    }
}

std::string ReadContent(util::Stream& stream, const Context& inputContext)
{
    std::string content;
    try
    {
        util::BinaryStreamReader reader(stream);
        int n = inputContext.ContentLength();
        for (int i = 0; i < n; ++i)
        {
            char c = reader.ReadChar();
            content.append(1, c);
        }
    }
    catch (const std::exception& ex)
    {
        throw JsonRpcException(invalidRequest, "invalid content length: " + std::string(ex.what()));
    }
    return content;
}

std::unique_ptr<util::JsonValue> ParseContent(const std::string& content)
{
    try
    {
        std::unique_ptr<util::JsonValue> jsonValue = soul::json::ParseJsonValue(content);
        if (!jsonValue->IsStructuredValue())
        {
            throw JsonRpcException(invalidRequest, "content must consists of an array or of an object");
        }
        return jsonValue;
    }
    catch (const std::exception& ex)
    {
        throw JsonRpcException(parseError, "content parse error: " + std::string(ex.what()));
    }
}

void WriteHeaders(util::Stream& stream, const Context& outputContext)
{
    util::BinaryStreamWriter writer(stream);
    writer.Write("Content-Length: " + std::to_string(outputContext.ContentLength()) + "\r\n", false);
    writer.Write("\r\n", false);
}

void WriteContent(util::Stream& stream, const std::string& content)
{
    util::BinaryStreamWriter writer(stream);
    writer.Write(content, false);
}

void WriteJsonValue(util::JsonValue* value, util::Stream& stream)
{
    std::stringstream sstream;
    util::CodeFormatter formatter(sstream);
    value->Write(formatter);
    std::string valueContent = sstream.str();
    Context outputContext;
    outputContext.SetContentLength(valueContent.length());
    WriteHeaders(stream, outputContext);
    WriteContent(stream, valueContent);
}

} // namespace soul::json::rpc
