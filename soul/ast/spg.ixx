// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.spg;

import std.core;
import soul.ast.common;

export namespace soul::ast::spg {
    
enum class FileKind
{
    spgFile, parserFile
};

class File
{
public:
    File(FileKind kind_, const std::string& filePath_);
    FileKind Kind() const { return kind; }
    const std::string& FilePath() const { return filePath; }
private:
    FileKind kind;
    std::string filePath;
};

class SpgFileDeclaration
{
public:
    SpgFileDeclaration(FileKind fileKind_, const std::string& filePath_);
    virtual ~SpgFileDeclaration();
    FileKind GetFileKind() const { return fileKind; }
    const std::string& FilePath() const { return filePath; }
private:
    FileKind fileKind;
    std::string filePath;
};

class ParserFileDeclaration : public SpgFileDeclaration
{
public:
    ParserFileDeclaration(const std::string& filePath_);
};

class ParserFile : public File
{
public:
    ParserFile(const std::string& filePath_);
};

class SpgFile : public File
{
public:
    SpgFile(const std::string& filePath_, const std::string& projectName_);
    const std::string& ProjectName() const { return projectName; }
    void AddDeclaration(SpgFileDeclaration* declaration);
    const std::vector<std::unique_ptr<SpgFileDeclaration>>& Declarations() const { return declarations; }
private:
    std::string projectName;
    std::vector<std::unique_ptr<SpgFileDeclaration>> declarations;
    std::vector<std::unique_ptr<ParserFile>> parserFiles;
};

} // namespace soul::ast::spg
