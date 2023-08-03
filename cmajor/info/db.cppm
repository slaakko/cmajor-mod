// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.info.db;

import std.core;

export namespace cmajor::info::db {

struct SourceLoc
{
    SourceLoc();
    std::string path;
    int line;
};

struct Location
{
    Location();
    bool IsValid() const;
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
    std::string name;
    std::string id;
};

struct Result
{
    Result();
    Type staticType;
    Type dynamicType;
    bool initialized;
    std::string value;
};

struct ChildResult
{
    ChildResult();
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

struct DepthRequest
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

struct EvaluateRequest
{
    EvaluateRequest();
    std::string expression;
};

struct EvaluateReply
{
    EvaluateReply();
    Result result;
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
    std::vector<ChildResult> results;
};

} // namespace cmajor::info::db
