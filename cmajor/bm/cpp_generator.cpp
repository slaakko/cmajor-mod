// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binary.message.cpp.generator;

import cmajor.binary.message.schema.ast;
import cmajor.binary.message.protocol;
import cmajor.binary.message.schema.lexer;
import cmajor.binary.message.schema.rules;
import cmajor.binary.message.schema.parser;
import util;

namespace cmajor::binary::message::cpp {

class InterfaceGeneratorVisitor : public cmajor::binary::message::schema::ast::DefaultVisitor
{
public:
    InterfaceGeneratorVisitor(const std::string& schemaFileName_, const std::string& interfaceFileName_);
    void Visit(cmajor::binary::message::schema::ast::ForwardClassDeclarationNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::UsingAliasNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::BoolNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::SByteNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ByteNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ShortNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::UShortNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::IntNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::UIntNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::LongNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ULongNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::FloatNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::DoubleNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::CharNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::WCharNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::UCharNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::UuidNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::StringNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::NumberNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::DateNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::DateTimeNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::EnumConstantNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::EnumTypeNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ClassIdNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ArrayTypeNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::MemberVariableNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ClassNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::NamespaceNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::SourceFileNode& node) override;
private:
    std::string schemaFileName;
    std::string interfaceFileName;
    std::ofstream file;
    util::CodeFormatter formatter;
};

InterfaceGeneratorVisitor::InterfaceGeneratorVisitor(const std::string& schemaFileName_, const std::string& interfaceFileName_) : 
    schemaFileName(schemaFileName_), interfaceFileName(interfaceFileName_), file(interfaceFileName), formatter(file)
{
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ForwardClassDeclarationNode& node)
{
    formatter.WriteLine();
    formatter.WriteLine("class " + node.ClassId() + ";");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UsingAliasNode& node)
{
    formatter.WriteLine();
    formatter.WriteLine("using " + node.Id() + " = " + node.QualifiedCppId() + ";");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::BoolNode& node)
{
    formatter.Write("bool");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::SByteNode& node)
{
    formatter.Write("int8_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ByteNode& node)
{
    formatter.Write("uint8_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ShortNode& node)
{
    formatter.Write("int16_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UShortNode& node)
{
    formatter.Write("uint16_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::IntNode& node)
{
    formatter.Write("int32_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UIntNode& node)
{
    formatter.Write("uint32_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::LongNode& node)
{
    formatter.Write("int64_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ULongNode& node)
{
    formatter.Write("uint64_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::FloatNode& node)
{
    formatter.Write("float");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::DoubleNode& node)
{
    formatter.Write("double");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::CharNode& node)
{
    formatter.Write("char");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::WCharNode& node)
{
    formatter.Write("char16_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UCharNode& node)
{
    formatter.Write("char32_t");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UuidNode& node)
{
    formatter.Write("util::uuid");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::StringNode& node)
{
    formatter.Write("std::string");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::NumberNode& node)
{
    formatter.Write("cmajor::bmp::Number");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::DateNode& node)
{
    formatter.Write("util::Date");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::DateTimeNode& node)
{
    formatter.Write("util::DateTime");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::EnumConstantNode& node)
{
    formatter.WriteLine(node.Id());
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::EnumTypeNode& node)
{
    formatter.WriteLine();
    formatter.WriteLine("enum class " + node.Id());
    formatter.WriteLine("{");
    formatter.IncIndent();
    bool first = true;
    for (const auto& enumConstant : node.EnumConstants())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        enumConstant->Accept(*this);
    }
    formatter.DecIndent();
    formatter.WriteLine("};");
    formatter.WriteLine();
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ClassIdNode& node)
{
    formatter.Write(node.Id());
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ArrayTypeNode& node)
{
    formatter.Write("std::vector<");
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    formatter.Write(">");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::MemberVariableNode& node)
{
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    formatter.Write(" ");
    formatter.Write(node.Id());
    formatter.WriteLine(";");
}

void WriteDefaultConstructorDeclaration(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine(node.Id() + "();");
}

void WriteConstructionDeclaration(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine(node.Id() + "(uint32_t id_);");
}

void WriteClassNameDeclaration(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("static std::string ClassName();");
}

void WriteRegisterFunctionDeclaration(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("static void Register();");
}

void WriteFactoryFunctionDeclaration(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);");
}

void WriteLengthFunctionDeclaration(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("uint32_t Length() const override;");
}

void WriteWriteFunctionDeclaration(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("void Write(util::MemoryWriter& writer) const override;");
}

void WriteReadFunctionDeclaration(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("void Read(util::MemoryReader& reader) override;");
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ClassNode& node)
{
    uint32_t messageId = node.MessageId();
    if (node.Parent() && node.Parent()->IsGlobalNamespace())
    {
        formatter.Write("export ");
    }
    formatter.WriteLine("const uint32_t " + node.MessageIdConstantName() + " = " + std::to_string(node.MessageId()) + ";");
    formatter.WriteLine();
    if (node.Parent() && node.Parent()->IsGlobalNamespace())
    {
        formatter.Write("export ");
    }
    formatter.Write("class " + node.Id());
    formatter.WriteLine(" : public cmajor::bmp::BinaryMessage");
    formatter.WriteLine("{");
    formatter.WriteLine("public:");
    formatter.IncIndent();
    WriteDefaultConstructorDeclaration(formatter, node);
    WriteConstructionDeclaration(formatter, node);
    WriteClassNameDeclaration(formatter, node);
    WriteRegisterFunctionDeclaration(formatter, node);
    WriteFactoryFunctionDeclaration(formatter, node);
    WriteLengthFunctionDeclaration(formatter, node);
    WriteWriteFunctionDeclaration(formatter, node);
    WriteReadFunctionDeclaration(formatter, node);
    formatter.DecIndent();
    formatter.WriteLine("public:");
    formatter.IncIndent();
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    formatter.DecIndent();
    formatter.WriteLine("};");
    formatter.WriteLine();
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::NamespaceNode& node)
{
    if (!node.Id().empty())
    {
        formatter.WriteLine("export namespace " + node.Id() + " {");
        formatter.WriteLine();
        cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
        formatter.WriteLine();
        formatter.WriteLine("} // namespace " + node.Id());
    }
    else
    {
        cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    }
}

void InterfaceGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::SourceFileNode& node)
{
    formatter.WriteLine("// this file has been automatically generated from '" + schemaFileName + "' by cmajor binary protocol message generator version " + 
        cmajor::bmp::Version());
    formatter.WriteLine();
    formatter.WriteLine("export module " + node.Module()->ModuleName() + ";");
    formatter.WriteLine();
    formatter.WriteLine("import std.core;");
    formatter.WriteLine("import util;");
    formatter.WriteLine("import cmajor.binary.message.protocol;");
    formatter.WriteLine("import cmajor.binary.message.number;");
    for (const auto& import : node.Imports())
    {
        if (import->Prefix() == cmajor::binary::message::schema::ast::ImportPrefix::interfacePrefix)
        {
            formatter.WriteLine("import " + import->ModuleName() + ";");
        }
    }
    formatter.WriteLine();
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
}

class RegisterVisitor : public cmajor::binary::message::schema::ast::DefaultVisitor
{
public:
    RegisterVisitor(util::CodeFormatter& formatter_);
    void Visit(cmajor::binary::message::schema::ast::SourceFileNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ClassNode& node) override;
private:
    util::CodeFormatter& formatter;
};

RegisterVisitor::RegisterVisitor(util::CodeFormatter& formatter_) : formatter(formatter_)
{
}

void RegisterVisitor::Visit(cmajor::binary::message::schema::ast::SourceFileNode& node)
{
    formatter.WriteLine("namespace {");
    formatter.WriteLine();
    formatter.WriteLine("struct Reg");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("Reg()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.DecIndent();
    formatter.WriteLine("};");
    formatter.WriteLine();
    formatter.WriteLine("Reg reg;");
    formatter.WriteLine();
    formatter.WriteLine("} // namespace");
    formatter.WriteLine();
}

void RegisterVisitor::Visit(cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine(node.FullClassName() + "::Register();");
}

class ImplementationGeneratorVisitor : public cmajor::binary::message::schema::ast::DefaultVisitor
{
public:
    ImplementationGeneratorVisitor(const std::string& schemaFileName_, const std::string& implementationFileName_);
    void Visit(cmajor::binary::message::schema::ast::SourceFileNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::NamespaceNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ClassNode& node) override;
private:
    std::string schemaFileName;
    std::string implementationFileName;
    std::ofstream file;
    util::CodeFormatter formatter;
};

ImplementationGeneratorVisitor::ImplementationGeneratorVisitor(const std::string& schemaFileName_, const std::string& implementationFileName_) : 
    schemaFileName(schemaFileName_), implementationFileName(implementationFileName_), file(implementationFileName), formatter(file)
{
}

void ImplementationGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::SourceFileNode& node)
{
    formatter.WriteLine("// this file has been automatically generated from '" + schemaFileName + "' by cmajor binary protocol message generator version " + 
        cmajor::bmp::Version());
    formatter.WriteLine();
    formatter.WriteLine("module " + node.Module()->ModuleName() + ";");
    formatter.WriteLine();
    formatter.WriteLine("import cmajor.binary.message.io;");
    for (const auto& import : node.Imports())
    {
        if (import->Prefix() == cmajor::binary::message::schema::ast::ImportPrefix::implementationPrefix)
        {
            formatter.WriteLine("import " + import->ModuleName() + ";");
        }
    }
    formatter.WriteLine();
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    formatter.WriteLine();
    RegisterVisitor registerVisitor(formatter);
    node.Accept(registerVisitor);
}

void ImplementationGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::NamespaceNode& node)
{
    if (!node.Id().empty())
    {
        formatter.WriteLine("namespace " + node.Id() + " {");
        formatter.WriteLine();
        cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
        formatter.WriteLine();
        formatter.WriteLine("} // namespace " + node.Id());
    }
    else
    {
        cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    }
}

void WriteDefaultConstructorImplementation(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine(node.Id() + "::" + node.Id() + "()");
    formatter.IncIndent();
    formatter.WriteLine(" : " + node.Id() + "::" + node.Id() + "(" + node.MessageIdConstantName() + ")");
    formatter.DecIndent();
    formatter.WriteLine("{");
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteConstructorImplementation(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine(node.Id() + "::" + node.Id() + "(uint32_t id_)");
    formatter.IncIndent();
    formatter.Write(" : cmajor::bmp::BinaryMessage(id_)");
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            formatter.Write(", ");
            cmajor::binary::message::schema::ast::MemberVariableNode* memVar = static_cast<cmajor::binary::message::schema::ast::MemberVariableNode*>(n.get());
            formatter.Write(memVar->Id() + "()");
        }
    }
    formatter.WriteLine();
    formatter.DecIndent();
    formatter.WriteLine("{");
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteClassNameFunctionImplementation(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("std::string " + node.Id() + "::ClassName()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("return \"" + util::StringStr(node.FullClassName()) + "\";");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteRegisterFunctionImplementation(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("void " + node.Id() + "::Register()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("cmajor::bmp::RegisterMessage<" + node.Id() + ">();");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteFactoryFunctionImplementation(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("cmajor::bmp::BinaryMessage* " + node.Id() + "::Create(uint32_t messageId)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("return new " + node.Id() + "(messageId);");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteLengthFunctionImplementation(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("uint32_t " + node.Id() + "::Length() const");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("uint32_t length = 0;");
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            cmajor::binary::message::schema::ast::MemberVariableNode* memVar = static_cast<cmajor::binary::message::schema::ast::MemberVariableNode*>(n.get());
            formatter.WriteLine("length += cmajor::bmp::Length(" + memVar->Id() + ");");
        }
    }
    formatter.WriteLine("return length;");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteWriteFunctionImplementation(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("void " + node.Id() + "::Write(util::MemoryWriter& writer) const");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            cmajor::binary::message::schema::ast::MemberVariableNode* memVar = static_cast<cmajor::binary::message::schema::ast::MemberVariableNode*>(n.get());
            formatter.WriteLine("cmajor::bmp::Write(writer, " + memVar->Id() + ");");
        }
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteReadFunctionImplementation(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("void " + node.Id() + "::Read(util::MemoryReader& reader)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            cmajor::binary::message::schema::ast::MemberVariableNode* memVar = static_cast<cmajor::binary::message::schema::ast::MemberVariableNode*>(n.get());
            formatter.WriteLine("cmajor::bmp::Read(reader, " + memVar->Id() + ");");
        }
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void ImplementationGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ClassNode& node)
{
    WriteDefaultConstructorImplementation(formatter, node);
    WriteConstructorImplementation(formatter, node);
    WriteClassNameFunctionImplementation(formatter, node);
    WriteRegisterFunctionImplementation(formatter, node);
    WriteFactoryFunctionImplementation(formatter, node);
    WriteLengthFunctionImplementation(formatter, node);
    WriteWriteFunctionImplementation(formatter, node);
    WriteReadFunctionImplementation(formatter, node);
}

void GenerateBinaryMessageFiles(const std::string& schemaFileName, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << schemaFileName << std::endl;
    }
    std::string content = util::ReadFile(schemaFileName);
    std::u32string ucontent = util::ToUtf32(content);
    if (ucontent.empty())
    {
        throw std::runtime_error("file '" + schemaFileName + "' is empty: need at least an export module declaration.");
    }
    auto lexer = cmajor::binary::message::schema::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), schemaFileName);
    lexer.SetRuleNameMapPtr(cmajor::binary::message::schema::rules::GetRuleNameMapPtr());
    using LexerType = decltype(lexer);
    std::unique_ptr<cmajor::binary::message::schema::ast::SourceFileNode> sourceFile = cmajor::binary::message::schema::parser::BinaryMessageSchemaParser<LexerType>::Parse(lexer);
    std::string interfaceFileName = util::GetFullPath(util::Path::ChangeExtension(schemaFileName, ".cppm"));
    InterfaceGeneratorVisitor interfaceGeneratorVisitor(schemaFileName, interfaceFileName);
    sourceFile->Accept(interfaceGeneratorVisitor);
    if (verbose)
    {
        std::cout << "==> " << interfaceFileName << std::endl;
    }
    std::string implementationFileName = util::GetFullPath(util::Path::ChangeExtension(schemaFileName, ".cpp"));
    ImplementationGeneratorVisitor implementationGeneratorVisitor(schemaFileName, implementationFileName);
    sourceFile->Accept(implementationGeneratorVisitor);
    if (verbose)
    {
        std::cout << "==> " << implementationFileName << std::endl;
    }
}

} // namespace cmajor::binary::message
