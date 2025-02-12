// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.assembly.asm_util;

import util;

namespace cmajor::sbin::assembly {

uint64_t ParseHexNumber(const std::string& s)
{
    return util::ParseHexULong(s);
}

double ParseReal(const std::string& s)
{
    return std::stod(s);
}

uint64_t ParseInteger(const std::string& s)
{
    return std::stoll(s);
}

std::string ParseString(const std::string& s)
{
    std::string str;
    const char* p = s.c_str();
    const char* e = s.c_str() + s.length();
    if (p != e)
    {
        if (*p == '\'')
        {
            ++p;
        }
        else
        {
            throw std::runtime_error("invalid string");
        }
        while (p != e)
        {
            if (*p == '\'')
            {
                ++p;
                if (p != e && *p == '\'')
                {
                    str.append(1, '\'');
                    ++p;
                }
                else
                {
                    break;
                }
            }
            else
            {
                str.append(1, *p);
                ++p;
            }
        }
    }
    if (p != e)
    {
        throw std::runtime_error("invalid string");
    }
    return str;
}

} // namespace cmajor::sbin::assembly
