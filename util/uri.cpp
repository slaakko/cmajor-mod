// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.uri;

import util.text.util;

namespace util {

InvalidUriException::InvalidUriException(const std::string& message_) : std::runtime_error(message_)
{
}

int ParseHexDigit(char c, const std::string& uri)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        return 10 + c - 'a';
    }
    else if (c >= 'A' && c <= 'F')
    {
        return 10 + c - 'A';
    }
    else
    {
        throw InvalidUriException("invalid URI '" + uri + "': HEXDIGIT expected");
    }
}

std::string MakePath(const std::string& s)
{
    if (s.starts_with('/'))
    {
        if (s.length() >= 3)
        {
            if (std::isalpha(s[1]) && s[2] == ':')
            {
                return ToUpperNarrow(std::string(1, s[1])) + s.substr(2);
            }
        }
    }
    return s;
}

std::string PercentEncode(unsigned char c)
{
    static char hexNibble[] = "0123456789ABCDEF";
    char highNibble = hexNibble[(c >> 4) & 0x0F];
    char lowNibble = hexNibble[c & 0x0F];
    std::string s(1, '%');
    s.append(1, highNibble);
    s.append(1, lowNibble);
    return s;
}

std::string PercentEncode(const std::string& s, bool allowAuthorityChars, bool allowPathChars)
{
    std::string encoded;
    for (char c : s)
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '-') || (c == '.') || (c == '_') || (c == '~'))
        {
            encoded.append(1, c);
        }
        else if (allowAuthorityChars && ((c == ':') || (c == '@') || (c == '[') || (c == ']') || (c == '=')))
        {
            encoded.append(1, c);
        }
        else if (allowPathChars && ((c == '/')))
        {
            encoded.append(1, '/');
        }
        else
        {
            encoded.append(PercentEncode(c));
        }
    }
    return encoded;
}

std::string PercentEncode(const std::string& s)
{
    return PercentEncode(s, false, false);
}

std::string EncodePath(const std::string& path, bool hasAuthority)
{
    if (hasAuthority && path.length() >= 2 && std::isalpha(path[0]) && path[1] == ':')
    {
        return "/" + PercentEncode(path, true, true);
    }
    else if (hasAuthority && !path.empty() && path[0] != '/')
    {
        return "/" + PercentEncode(path, true, true);
    }
    else
    {
        return PercentEncode(path, true, true);
    }
}

Uri::Uri()
{
}

Uri::Uri(const std::string& uri)
{
    int state = 0;
    int returnState = 0;
    int highByte = 0;
    std::string s;
    for (char c : uri)
    {
        switch (state)
        {
            case 0:
            {
                switch (c)
                {
                    case '%':
                    {
                        state = 1;
                        returnState = 0;
                        break;
                    }
                    case ':':
                    {
                        scheme = s;
                        s.clear();
                        state = 10;
                        break;
                    }
                    default:
                    {
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.')
                        {
                            s.append(1, c);
                        }
                        else
                        {
                            throw InvalidUriException("invalid URI '" + uri + "': invalid character in scheme");
                        }
                        break;
                    }
                }
                break;
            }
            case 1:
            {
                highByte = ParseHexDigit(c, uri);
                state = 2;
                break;
            }
            case 2:
            {
                int lowByte = ParseHexDigit(c, uri);
                s.append(1, char(16 * highByte + lowByte));
                state = returnState;
                break;
            }
            case 10:
            {
                switch (c)
                {
                    case '/':
                    {
                        state = 11;
                        break;
                    }
                    case '?':
                    {
                        authority = s;
                        s.clear();
                        state = 30;
                        break;
                    }
                    case '#':
                    {
                        authority = s;
                        s.clear();
                        state = 40;
                        break;
                    }
                    default:
                    {
                        s.append(1, c);
                        state = 20;
                        break;
                    }
                }
                break;
            }
            case 11:
            {
                switch (c)
                {
                    case '/':
                    {
                        state = 12;
                        break;
                    }
                    default:
                    {
                        authority = s;
                        s.clear();
                        s.append(1, '/');
                        s.append(1, c);
                        state = 20;
                        break;
                    }
                }
                break;
            }
            case 12:
            {
                switch (c)
                {
                    case '%':
                    {
                        state = 1;
                        returnState = 12;
                        break;
                    }
                    case '/':
                    {
                        authority = s;
                        s.clear();
                        s.append(1, '/');
                        state = 20;
                        break;
                    }
                    case '?':
                    {
                        authority = s;
                        s.clear();
                        state = 30;
                        break;
                    }
                    case '#':
                    {
                        authority = s;
                        s.clear();
                        state = 40;
                        break;
                    }
                    default:
                    {
                        s.append(1, c);
                        break;
                    }
                }
                break;
            }
            case 20:
            {
                switch (c)
                {
                    case '%':
                    {
                        state = 1;
                        returnState = 20;
                        break;
                    }
                    case '?':
                    {
                        path = MakePath(s);
                        s.clear();
                        state = 30;
                        break;
                    }
                    case '#':
                    {
                        path = MakePath(s);
                        s.clear();
                        state = 40;
                        break;
                    }
                    default:
                    {
                        s.append(1, c);
                        break;
                    }
                }
                break;
            }
            case 30:
            {
                switch (c)
                {
                    case '%':
                    {
                        state = 1;
                        returnState = 30;
                        break;
                    }
                    case '#':
                    {
                        query = s;
                        s.clear();
                        state = 40;
                        break;
                    }
                    default:
                    {
                        s.append(1, c);
                        break;
                    }
                }
                break;
            }
            case 40:
            {
                switch (c)
                {
                    case '%':
                    {
                        state = 1;
                        returnState = 40;
                        break;
                    }
                    default:
                    {
                        s.append(1, c);
                        break;
                    }
                }
                break;
            }
        }
    }
    if (state == 0)
    {
        throw InvalidUriException("invalid URI '" + uri + "': no scheme");
    }
    else if (state == 1 || state == 2)
    {
        throw InvalidUriException("invalid URI '" + uri + "': two HEX BYTES exptected after %");
    }
    else if (state == 10 || state == 11 || state == 12)
    {
        authority = s;
    }
    else if (state == 20)
    {
        path = MakePath(s);
    }
    else if (state == 30)
    {
        query = s;
    }
    else if (state == 40)
    {
        fragment = s;
    }
}

std::string Uri::ToString()
{
    if (IsEmpty()) return std::string();
    std::string uriStr = PercentEncode(scheme, false, false);
    uriStr.append(1, ':');
    if (scheme == "file")
    {
        uriStr.append(1, '/');
        uriStr.append(1, '/');
        uriStr.append(PercentEncode(authority, true, false));
    }
    uriStr.append(EncodePath(path, scheme == "file"));
    if (!query.empty())
    {
        uriStr.append(1, '?').append(PercentEncode(query, true, true));
    }
    if (!fragment.empty())
    {
        uriStr.append(1, '#').append(PercentEncode(fragment, true, true));
    }
    return uriStr;
}

} // namespace util
