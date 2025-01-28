// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.uri;

import std.core;

export namespace util {

class InvalidUriException : public std::runtime_error
{
public:
    InvalidUriException(const std::string& message_);
};

struct Uri
{
    Uri();
    Uri(const std::string& uri);
    std::string ToString();
    bool IsEmpty() const { return scheme.empty(); }
    std::string scheme;
    std::string authority;
    std::string path;
    std::string query;
    std::string fragment;
};

} // namespace util
