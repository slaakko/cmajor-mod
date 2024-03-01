// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binary.message.cmajor.generator;

import cmajor.binary.message.schema.ast;
import cmajor.binary.message.protocol;
import cmajor.binary.message.schema.lexer;
import cmajor.binary.message.schema.rules;
import cmajor.binary.message.schema.parser;
import util;

namespace cmajor::binary::message::cm {

class MessageFileGeneratorVisitor : public cmajor::binary::message::schema::ast::DefaultVisitor
{
public:
    MessageFileGeneratorVisitor(const std::string& schemaFileName_, const std::string& messageFileName_);
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
    std::string messageFileName;
    std::ofstream file;
    util::CodeFormatter formatter;
    bool registrationGenerated; 
};

MessageFileGeneratorVisitor::MessageFileGeneratorVisitor(const std::string& schemaFileName_, const std::string& messageFileName_) : 
    schemaFileName(schemaFileName_), messageFileName(messageFileName_), file(messageFileName), formatter(file), registrationGenerated(false)
{
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ForwardClassDeclarationNode& node)
{
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UsingAliasNode& node)
{
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::BoolNode& node)
{
    formatter.Write("bool");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::SByteNode& node)
{
    formatter.Write("sbyte");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ByteNode& node)
{
    formatter.Write("byte");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ShortNode& node) 
{
    formatter.Write("short");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UShortNode& node)
{
    formatter.Write("ushort");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::IntNode& node)
{
    formatter.Write("int");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UIntNode& node)
{
    formatter.Write("uint");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::LongNode& node)
{
    formatter.Write("long");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ULongNode& node)
{
    formatter.Write("ulong");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::FloatNode& node)
{
    formatter.Write("float");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::DoubleNode& node)
{
    formatter.Write("double");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::CharNode& node)
{
    formatter.Write("char");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::WCharNode& node)
{
    formatter.Write("wchar");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UCharNode& node)
{
    formatter.Write("uchar");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::UuidNode& node)
{
    formatter.Write("Uuid");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::StringNode& node)
{
    formatter.Write("string");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::NumberNode& node)
{
    formatter.Write("System.Net.Bmp.Number");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::DateNode& node)
{
    formatter.Write("Date");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::DateTimeNode& node)
{
    formatter.Write("DateTime");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::EnumConstantNode& node)
{
    formatter.WriteLine(node.Id());
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::EnumTypeNode& node)
{
    formatter.WriteLine();
    formatter.WriteLine("public enum " + node.Id());
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

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ClassIdNode& node)
{
    formatter.Write(node.Id());
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ArrayTypeNode& node)
{
    formatter.Write("List<");
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    formatter.Write(">");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::MemberVariableNode& node)
{
    formatter.Write("public ");
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    formatter.Write(" ");
    formatter.Write(node.Id());
    formatter.WriteLine(";");
}

void WriteDefaultConstructor(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public " + node.Id() + "() : this(" + node.MessageIdConstantName() + ")");
    formatter.WriteLine("{");
    formatter.WriteLine("}");
}

void WriteConstructor(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public " + node.Id() + "(uint messageId_) : base(messageId_)");
    formatter.WriteLine("{");
    formatter.WriteLine("}");
}

void WriteClassNameFunction(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public static string ClassName()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("return \"" + util::StringStr(node.FullClassName()) + "\";");
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void WriteRegisterFunction(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public static void Register()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("System.Net.Bmp.RegisterMessage<" + node.Id() + ">();");
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void WriteFactoryFunction(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public static System.Net.Bmp.BinaryMessage* Create(uint messageId)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("return new " + node.Id() + "(messageId);");
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void WriteLengthFunction(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public override uint Length() const");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("uint length = 0u;");
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            cmajor::binary::message::schema::ast::MemberVariableNode* memVar = static_cast<cmajor::binary::message::schema::ast::MemberVariableNode*>(n.get());
            formatter.WriteLine("length = length + System.Net.Bmp.Length(" + memVar->Id() + ");");
        }
    }
    formatter.WriteLine("return length;");
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void WriteWriteFunction(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public override void Write(MemoryWriter& writer)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            cmajor::binary::message::schema::ast::MemberVariableNode* memVar = static_cast<cmajor::binary::message::schema::ast::MemberVariableNode*>(n.get());
            formatter.WriteLine("System.Net.Bmp.Write(writer, " + memVar->Id() + ");");
        }
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void WriteReadFunction(util::CodeFormatter& formatter, cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public override void Read(MemoryReader& reader)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            cmajor::binary::message::schema::ast::MemberVariableNode* memVar = static_cast<cmajor::binary::message::schema::ast::MemberVariableNode*>(n.get());
            formatter.WriteLine("System.Net.Bmp.Read(reader, " + memVar->Id() + ");");
        }
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine("public const uint " + node.MessageIdConstantName() + " = " + std::to_string(node.MessageId()) + "u;");
    formatter.WriteLine();
    formatter.Write("public class " + node.Id());
    formatter.WriteLine(" : System.Net.Bmp.BinaryMessage");
    formatter.WriteLine("{");
    formatter.IncIndent();
    WriteDefaultConstructor(formatter, node);
    WriteConstructor(formatter, node);
    WriteClassNameFunction(formatter, node);
    WriteRegisterFunction(formatter, node);
    WriteFactoryFunction(formatter, node);
    WriteLengthFunction(formatter, node);
    WriteWriteFunction(formatter, node);
    WriteReadFunction(formatter, node);
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

class RegisterVisitor : public cmajor::binary::message::schema::ast::DefaultVisitor
{
public:
    RegisterVisitor(util::CodeFormatter& formatter_);
    void Visit(cmajor::binary::message::schema::ast::NamespaceNode& node) override;
    void Visit(cmajor::binary::message::schema::ast::ClassNode& node) override;
private:
    util::CodeFormatter& formatter;
};

RegisterVisitor::RegisterVisitor(util::CodeFormatter& formatter_) : formatter(formatter_)
{
}

void RegisterVisitor::Visit(cmajor::binary::message::schema::ast::NamespaceNode& node)
{
    formatter.WriteLine("public class Reg");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("static Reg() : instance(new Reg())");
    formatter.WriteLine("{");
    formatter.WriteLine("}");
    formatter.WriteLine("private Reg() : registered(false)");
    formatter.WriteLine("{");
    formatter.WriteLine("}");
    formatter.WriteLine("public static Reg& Instance()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("return *instance;");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine("public void Register()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("if (!registered)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (auto& n : node.Nodes())
    {
        n->Accept(*this);
    }
    formatter.WriteLine("registered = true;");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine("private static UniquePtr<Reg> instance;");
    formatter.WriteLine("private bool registered;");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
    formatter.WriteLine("public void Register()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("Reg.Instance().Register();");
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void RegisterVisitor::Visit(cmajor::binary::message::schema::ast::ClassNode& node)
{
    formatter.WriteLine(node.FullCmajorClassName() + ".Register();");
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::NamespaceNode& node)
{
    if (!node.Id().empty())
    {
        formatter.WriteLine("namespace " + node.Id() + " {");
        formatter.WriteLine();
        for (auto& n : node.Nodes())
        {
            n->Accept(*this);
        }
        formatter.WriteLine();
        registrationGenerated = true;
        RegisterVisitor registerVisitor(formatter);
        node.Accept(registerVisitor);
        formatter.WriteLine();
        formatter.WriteLine("} // namespace " + node.Id());
    }
    else
    {
        cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
        formatter.WriteLine();
        if (!registrationGenerated)
        {
            RegisterVisitor registerVisitor(formatter);
            node.Accept(registerVisitor);
        }
    }
}

void MessageFileGeneratorVisitor::Visit(cmajor::binary::message::schema::ast::SourceFileNode& node)
{
    formatter.WriteLine("// this file has been automatically generated from '" + schemaFileName + "' by cmajor binary protocol message generator version " +
        cmajor::bmp::Version());
    formatter.WriteLine();
    formatter.WriteLine("using System;");
    formatter.WriteLine("using System.Net.Bmp;");
    for (const auto& import : node.Imports())
    {
        if (import->Prefix() == cmajor::binary::message::schema::ast::ImportPrefix::interfacePrefix)
        {
            formatter.WriteLine("using " + import->ModuleName() + ";");
        }
    }
    formatter.WriteLine();
    cmajor::binary::message::schema::ast::DefaultVisitor::Visit(node);
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
    std::string messageFileName = util::GetFullPath(util::Path::ChangeExtension(schemaFileName, ".cm"));
    MessageFileGeneratorVisitor messageFileGeneratorVisitor(schemaFileName, messageFileName);
    sourceFile->Accept(messageFileGeneratorVisitor);
    if (verbose)
    {
        std::cout << "==> " << messageFileName << std::endl;
    }
}

} // namespace cmajor::binary::message
