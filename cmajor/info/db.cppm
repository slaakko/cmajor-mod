// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.info.db;

import soul.xml.dom;
import std.core;

export namespace cmajor::info::db {

struct SourceLoc
{
    SourceLoc();
    std::unique_ptr<soul::xml::Element> ToXml() const;
    std::string path;
    int line;
};

struct Location
{
    Location();
    bool IsValid() const;
    std::unique_ptr<soul::xml::Element> ToXml() const;
    std::string func;
    std::string addr;
    std::string file;
    int line;
    int scol;
    int ecol;
    int level;
};

struct Type
{
    Type();
    std::unique_ptr<soul::xml::Element> ToXml() const;
    std::string name;
    std::string id;
};

struct Result
{
    Result();
    std::unique_ptr<soul::xml::Element> ToXml() const;
    Type staticType;
    Type dynamicType;
    bool initialized;
    std::string value;
};

struct ChildResult
{
    ChildResult();
    std::unique_ptr<soul::xml::Element> ToXml() const;
    std::string expr;
    std::string name;
    std::string type;
    std::string dynType;
    std::string value;
    int count;
};

struct BreakReply
{
    BreakReply();
    std::vector<std::string> breakpointIds;
};

struct DeleteReply
{
};

struct DepthReply
{
    DepthReply();
    int depth;
};

struct FramesRequest
{
    FramesRequest();
    int lowFrame;
    int highFrame;
};

struct FramesReply
{
    FramesReply();
    std::vector<Location> frames;
};

struct EvaluateReply
{
    EvaluateReply();
    std::unique_ptr<soul::xml::Element> ToXml() const;
    Result result;
    bool success;
    std::string error;
};

struct CountRequest
{
    CountRequest();
    std::string expression;
};

struct CountReply
{
    CountReply();
    int count;
};

struct EvaluateChildRequest
{
    EvaluateChildRequest();
    std::string expression;
    int start;
    int count;
};

struct EvaluateChildReply
{
    EvaluateChildReply();
    std::unique_ptr<soul::xml::Element> ToXml() const;
    std::vector<ChildResult> results;
    bool success;
    std::string error;
};

} // namespace cmajor::info::db
