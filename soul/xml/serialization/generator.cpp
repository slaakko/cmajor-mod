// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.serialization.generator;

import soul.xml.serialization.version;
import soul.xml.serialization.parser;
import soul.xml.serialization.spg.rules;
import soul.xml.serialization.lexer;
import soul.xml.serialization.ast;
import soul.lexer.error;
import util;

namespace soul::xml::serialization {

void WriteIndentedText(util::CodeFormatter& formatter, const std::string& cppText)
{
    std::string line;
    bool startOfLine = true;
    bool lbraceSeen = false;
    for (char c : cppText)
    {
        if (c == '\n')
        {
            formatter.WriteLine(line);
            line.clear();
            startOfLine = true;
            if (lbraceSeen)
            {
                formatter.IncIndent();
                lbraceSeen = false;
            }
        }
        else if (c == ' ')
        {
            if (!startOfLine)
            {
                line.append(1, c);
            }
        }
        else if (c == '{')
        {
            line.append(1, c);
            lbraceSeen = true;
        }
        else if (c == '}')
        {
            line.append(1, c);
            formatter.DecIndent();
        }
        else if (c != '\r')
        {
            startOfLine = false;
            line.append(1, c);
        }
    }
    if (!line.empty())
    {
        formatter.WriteLine(line);
    }
}

class InterfaceGeneratorVisitor : public soul::xml::serialization::ast::DefaultVisitor
{
public:
    InterfaceGeneratorVisitor(const std::string& xmlserFileName_, const std::string& interfaceFileName_);
    void Visit(soul::xml::serialization::ast::ForwardClassDeclarationNode& node) override;
    void Visit(soul::xml::serialization::ast::UsingAliasNode& node) override;
    void Visit(soul::xml::serialization::ast::BoolNode& node) override;
    void Visit(soul::xml::serialization::ast::SByteNode& node) override;
    void Visit(soul::xml::serialization::ast::ByteNode& node) override;
    void Visit(soul::xml::serialization::ast::ShortNode& node) override;
    void Visit(soul::xml::serialization::ast::UShortNode& node) override;
    void Visit(soul::xml::serialization::ast::IntNode& node) override;
    void Visit(soul::xml::serialization::ast::UIntNode& node) override;
    void Visit(soul::xml::serialization::ast::LongNode& node) override;
    void Visit(soul::xml::serialization::ast::ULongNode& node) override;
    void Visit(soul::xml::serialization::ast::FloatNode& node) override;
    void Visit(soul::xml::serialization::ast::DoubleNode& node) override;
    void Visit(soul::xml::serialization::ast::CharNode& node) override;
    void Visit(soul::xml::serialization::ast::WCharNode& node) override;
    void Visit(soul::xml::serialization::ast::UCharNode& node) override;
    void Visit(soul::xml::serialization::ast::UuidNode& node) override;
    void Visit(soul::xml::serialization::ast::StringNode& node) override;
    void Visit(soul::xml::serialization::ast::WStringNode& node) override;
    void Visit(soul::xml::serialization::ast::UStringNode& node) override;
    void Visit(soul::xml::serialization::ast::DateNode& node) override;
    void Visit(soul::xml::serialization::ast::DateTimeNode& node) override;
    void Visit(soul::xml::serialization::ast::TimePointNode& node) override;
    void Visit(soul::xml::serialization::ast::DurationNode& node) override;
    void Visit(soul::xml::serialization::ast::EnumConstantNode& node) override;
    void Visit(soul::xml::serialization::ast::EnumTypeNode& node) override;
    void Visit(soul::xml::serialization::ast::ClassIdNode& node) override;
    void Visit(soul::xml::serialization::ast::PointerTypeNode& node) override;
    void Visit(soul::xml::serialization::ast::ArrayTypeNode& node) override;
    void Visit(soul::xml::serialization::ast::MemberVariableNode& node) override;
    void Visit(soul::xml::serialization::ast::CppBlockNode& node) override;
    void Visit(soul::xml::serialization::ast::ClassNode& node) override;
    void Visit(soul::xml::serialization::ast::NamespaceNode& node) override;
    void Visit(soul::xml::serialization::ast::SourceFileNode& node) override;
private:
    std::string xmlserFileName;
    std::string interfaceFileName;
    std::ofstream file;
    util::CodeFormatter formatter;
};

InterfaceGeneratorVisitor::InterfaceGeneratorVisitor(const std::string& xmlserFileName_, const std::string& interfaceFileName_) :
    xmlserFileName(xmlserFileName_), interfaceFileName(interfaceFileName_), file(interfaceFileName), formatter(file)
{
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::ForwardClassDeclarationNode& node)
{
    formatter.WriteLine();
    formatter.WriteLine("class " + node.ClassId() + ";");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::UsingAliasNode& node)
{
    formatter.WriteLine();
    formatter.WriteLine("using " + node.Id() + " = " + node.QualifiedCppId() + ";");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::BoolNode& node)
{
    formatter.Write("bool");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::SByteNode& node)
{
    formatter.Write("int8_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::ByteNode& node)
{
    formatter.Write("uint8_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::ShortNode& node)
{
    formatter.Write("int16_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::UShortNode& node)
{
    formatter.Write("uint16_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::IntNode& node)
{
    formatter.Write("int32_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::UIntNode& node)
{
    formatter.Write("uint32_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::LongNode& node)
{
    formatter.Write("int64_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::ULongNode& node)
{
    formatter.Write("uint64_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::FloatNode& node)
{
    formatter.Write("float");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::DoubleNode& node)
{
    formatter.Write("double");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::CharNode& node)
{
    formatter.Write("char");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::WCharNode& node)
{
    formatter.Write("char16_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::UCharNode& node)
{
    formatter.Write("char32_t");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::UuidNode& node)
{
    formatter.Write("util::uuid");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::StringNode& node)
{
    formatter.Write("std::string");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::WStringNode& node)
{
    formatter.Write("std::u16string");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::UStringNode& node)
{
    formatter.Write("std::u32string");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::DateNode& node)
{
    formatter.Write("util::Date");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::DateTimeNode& node)
{
    formatter.Write("util::DateTime");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::TimePointNode& node)
{
    formatter.Write("std::chrono::steady_clock::time_point");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::DurationNode& node)
{
    formatter.Write("std::chrono::steady_clock::duration");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::EnumConstantNode& node)
{
    formatter.WriteLine(node.Id());
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::EnumTypeNode& node)
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

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::ClassIdNode& node)
{
    formatter.Write(node.Id());
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::PointerTypeNode& node)
{
    formatter.Write("std::unique_ptr<");
    soul::xml::serialization::ast::DefaultVisitor::Visit(node);
    formatter.Write(">");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::ArrayTypeNode& node)
{
    formatter.Write("std::vector<");
    soul::xml::serialization::ast::DefaultVisitor::Visit(node);
    formatter.Write(">");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::MemberVariableNode& node)
{
    soul::xml::serialization::ast::DefaultVisitor::Visit(node);
    formatter.Write(" ");
    formatter.Write(node.Id());
    formatter.WriteLine(";");
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::CppBlockNode& node)
{
    if (node.Implementation()) return;
    WriteIndentedText(formatter, node.CppText());
}

void WriteDefaultConstructionDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine(node.Id() + "();");
}

void WriteVirtualDestructorDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("virtual ~" + node.Id() + "();");
}

void WriteClassNameDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    if (node.BaseClassId().empty())
    {
        formatter.WriteLine("virtual std::string ClassName() const;");
    }
    else
    {
        formatter.WriteLine("std::string ClassName() const override;");
    }
}

void WriteStaticClassNameDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("static std::string StaticClassName();");
}

void WriteFactoryFunctionDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("static void* Create();");
}

void WriteRegisterFunctionDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("static void Register();");
}

void WriteWriteToXmlFileFunctionDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void WriteToXmlFile(const std::string& xmlFileName);");
}

void WriteToXmlStringFunctionDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("std::string ToXmlString();");
}

void WriteReadFromXmlFileFunctionDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void ReadFromXmlFile(const std::string& xmlFileName);");
}

void WriteReadFromXmlStringFunctionDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void ReadFromXmlString(const std::string& xmlString);");
}

void WriteToXmlDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    if (node.BaseClassId().empty())
    {
        formatter.WriteLine("virtual soul::xml::Element* ToXml(const std::string& fieldName) const;");
    }
    else
    {
        formatter.WriteLine("soul::xml::Element* ToXml(const std::string& fieldName) const override;");
    }
}

void WriteFromXmlDeclaration(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    if (node.BaseClassId().empty())
    {
        formatter.WriteLine("virtual void FromXml(soul::xml::Element* element);");
    }
    else
    {
        formatter.WriteLine("void FromXml(soul::xml::Element* element) override;");
    }
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::ClassNode& node)
{
    if (node.Parent() && node.Parent()->IsGlobalNamespace())
    {
        formatter.Write("export ");
    }
    formatter.Write("class " + node.Id());
    if (!node.BaseClassId().empty())
    {
        formatter.Write(" : public " + node.BaseClassId());
    }
    formatter.WriteLine();
    formatter.WriteLine("{");
    formatter.WriteLine("public:");
    formatter.IncIndent();
    WriteDefaultConstructionDeclaration(formatter, node);
    if (node.BaseClassId().empty())
    {
        WriteVirtualDestructorDeclaration(formatter, node);
    }
    WriteClassNameDeclaration(formatter, node);
    WriteStaticClassNameDeclaration(formatter, node);
    WriteFactoryFunctionDeclaration(formatter, node);
    WriteRegisterFunctionDeclaration(formatter, node);
    if (node.BaseClassId().empty())
    {
        WriteWriteToXmlFileFunctionDeclaration(formatter, node);
        WriteToXmlStringFunctionDeclaration(formatter, node);
        WriteReadFromXmlFileFunctionDeclaration(formatter, node);
        WriteReadFromXmlStringFunctionDeclaration(formatter, node);
    }
    WriteToXmlDeclaration(formatter, node);
    WriteFromXmlDeclaration(formatter, node);
    formatter.DecIndent();
    formatter.WriteLine("public:");
    formatter.IncIndent();
    soul::xml::serialization::ast::DefaultVisitor::Visit(node);
    formatter.DecIndent();
    formatter.WriteLine("};");
    formatter.WriteLine();
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::NamespaceNode& node)
{
    if (!node.Id().empty())
    {
        formatter.WriteLine("export namespace " + node.Id() + " {");
        formatter.WriteLine();
        soul::xml::serialization::ast::DefaultVisitor::Visit(node);
        formatter.WriteLine();
        formatter.WriteLine("} // namespace " + node.Id());
    }
    else
    {
        soul::xml::serialization::ast::DefaultVisitor::Visit(node);
    }
}

void InterfaceGeneratorVisitor::Visit(soul::xml::serialization::ast::SourceFileNode& node)
{
    formatter.WriteLine("// this file has been automatically generated from '" + xmlserFileName + "' by xmlsergen version " + soul::xml::serialization::version::Version());
    formatter.WriteLine();
    formatter.WriteLine("export module " + node.Module()->ModuleName() + ";");
    formatter.WriteLine();
    formatter.WriteLine("import std.core;");
    formatter.WriteLine("import util.uuid;");
    formatter.WriteLine("import util.time;");
    formatter.WriteLine("import soul.xml.element;");
    for (const auto& import : node.Imports())
    {
        if (import->Prefix() == soul::xml::serialization::ast::ImportPrefix::interfacePrefix)
        {
            formatter.WriteLine("import " + import->ModuleName() + ";");
        }
    }
    formatter.WriteLine();
    soul::xml::serialization::ast::DefaultVisitor::Visit(node);
}

class RegisterVisitor : public soul::xml::serialization::ast::DefaultVisitor
{
public:
    RegisterVisitor(util::CodeFormatter& formatter_);
    void Visit(soul::xml::serialization::ast::SourceFileNode& node) override;
    void Visit(soul::xml::serialization::ast::ClassNode& node) override;
private:
    util::CodeFormatter& formatter;
};

RegisterVisitor::RegisterVisitor(util::CodeFormatter& formatter_) : formatter(formatter_)
{
}

void RegisterVisitor::Visit(soul::xml::serialization::ast::SourceFileNode& node)
{
    formatter.WriteLine("namespace {");
    formatter.WriteLine();
    formatter.WriteLine("struct Reg");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("Reg()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    soul::xml::serialization::ast::DefaultVisitor::Visit(node);
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

void RegisterVisitor::Visit(soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine(node.FullClassName() + "::Register();");
}

class ImplementationGeneratorVisitor : public soul::xml::serialization::ast::DefaultVisitor
{
public:
    ImplementationGeneratorVisitor(const std::string& xmlserFileName_, const std::string& implementationFileName_);
    void Visit(soul::xml::serialization::ast::SourceFileNode& node) override;
    void Visit(soul::xml::serialization::ast::NamespaceNode& node) override;
    void Visit(soul::xml::serialization::ast::ClassNode& node) override;
    void Visit(soul::xml::serialization::ast::CppBlockNode& node) override;
private:
    std::string xmlserFileName;
    std::string implementationFileName;
    std::ofstream file;
    util::CodeFormatter formatter;
};

ImplementationGeneratorVisitor::ImplementationGeneratorVisitor(const std::string& xmlserFileName_, const std::string& implementationFileName_) :
    xmlserFileName(xmlserFileName_), implementationFileName(implementationFileName_), file(implementationFileName), formatter(file)
{
}

void ImplementationGeneratorVisitor::Visit(soul::xml::serialization::ast::SourceFileNode& node)
{
    formatter.WriteLine("// this file has been automatically generated from '" + xmlserFileName + "' by xmlsergen version " + soul::xml::serialization::version::Version());
    formatter.WriteLine();
    formatter.WriteLine("module " + node.Module()->ModuleName() + ";");
    formatter.WriteLine();
    formatter.WriteLine("import soul.xml.document;");
    formatter.WriteLine("import soul.xml.dom.parser;");
    formatter.WriteLine("import soul.xml.serialization.class_registry;");
    formatter.WriteLine("import soul.xml.serialization.exp;");
    formatter.WriteLine("import soul.xml.serialization.imp;");
    for (const auto& import : node.Imports())
    {
        if (import->Prefix() == soul::xml::serialization::ast::ImportPrefix::implementationPrefix)
        {
            formatter.WriteLine("import " + import->ModuleName() + ";");
        }
    }
    formatter.WriteLine();
    soul::xml::serialization::ast::DefaultVisitor::Visit(node);
    formatter.WriteLine();
    RegisterVisitor registerVisitor(formatter);
    node.Accept(registerVisitor);
}

void ImplementationGeneratorVisitor::Visit(soul::xml::serialization::ast::NamespaceNode& node)
{
    if (!node.Id().empty())
    {
        formatter.WriteLine("namespace " + node.Id() + " {");
        formatter.WriteLine();
        soul::xml::serialization::ast::DefaultVisitor::Visit(node);
        formatter.WriteLine();
        formatter.WriteLine("} // namespace " + node.Id());
    }
    else
    {
        soul::xml::serialization::ast::DefaultVisitor::Visit(node);
    }
}

void WriteDefaultConstructionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine(node.Id() + "::" + node.Id() + "()");
    formatter.IncIndent();
    bool first = true;
    if (node.Nodes().empty())
    {
        if (!node.BaseClassId().empty())
        {
            formatter.Write(" : " + node.BaseClassId() + "()");
            first = false;
        }
    }
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            if (first)
            {
                first = false;
                formatter.Write(" : ");
                if (!node.BaseClassId().empty())
                {
                    formatter.Write(node.BaseClassId() + "(), ");
                }
            }
            else
            {
                formatter.Write(", ");
            }
            soul::xml::serialization::ast::MemberVariableNode* memVar = static_cast<soul::xml::serialization::ast::MemberVariableNode*>(n.get());
            formatter.Write(memVar->Id() + "()");
        }
    }
    if (!first)
    {
        formatter.WriteLine();
    }
    formatter.DecIndent();
    formatter.WriteLine("{");
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteDestructorImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine(node.Id() + "::~" + node.Id() + "()");
    formatter.WriteLine("{");
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteClassNameFunctionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("std::string " + node.Id() + "::ClassName() const");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("return \"" + util::StringStr(node.FullClassName()) + "\";");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteStaticClassNameFunctionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("std::string " + node.Id() + "::StaticClassName()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("return \"" + util::StringStr(node.FullClassName()) + "\";");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteFactoryFunctionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void* " + node.Id() + "::Create()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("return new " + node.Id() + "();");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteRegisterFunctionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void " + node.Id() + "::Register()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("soul::xml::serialization::RegisterClass<" + node.Id() + ">();");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteWriteToXmlFileFunctionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void " + node.Id() + "::WriteToXmlFile(const std::string& xmlFileName)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("soul::xml::Document xmlDoc;");
    formatter.WriteLine("xmlDoc.AppendChild(ToXml(\"" + node.Id() + "\"));");
    formatter.WriteLine("std::ofstream xmlFile(xmlFileName);");
    formatter.WriteLine("util::CodeFormatter formatter(xmlFile);");
    formatter.WriteLine("formatter.SetIndentSize(2);");
    formatter.WriteLine("xmlDoc.Write(formatter);");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteToXmlStringFunctionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("std::string " + node.Id() + "::ToXmlString()");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("std::stringstream stringStream;");
    formatter.WriteLine("soul::xml::Document xmlDoc;");
    formatter.WriteLine("xmlDoc.AppendChild(ToXml(\"" + node.Id() + "\"));");
    formatter.WriteLine("util::CodeFormatter formatter(stringStream);");
    formatter.WriteLine("formatter.SetIndentSize(2);");
    formatter.WriteLine("xmlDoc.Write(formatter);");
    formatter.WriteLine("return stringStream.str();");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteReadFromXmlFileFunctionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void " + node.Id() + "::ReadFromXmlFile(const std::string& xmlFileName)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);");
    formatter.WriteLine("FromXml(xmlDoc->DocumentElement());");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteReadFromXmlStringFunctionImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void " + node.Id() + "::ReadFromXmlString(const std::string& xmlString)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, \"" + node.Id() + "\");");
    formatter.WriteLine("FromXml(xmlDoc->DocumentElement());");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteToXmlImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("soul::xml::Element* " + node.Id() + "::ToXml(const std::string& fieldName) const");
    formatter.WriteLine("{");
    formatter.IncIndent();
    if (!node.BaseClassId().empty())
    {
        formatter.WriteLine("soul::xml::Element* element = " + node.BaseClassId() + "::ToXml(fieldName);");
        formatter.WriteLine("element->SetAttribute(\"className\", \"" + node.FullClassName() + "\");");
    }
    else
    {
        formatter.WriteLine("soul::xml::Element* element = soul::xml::MakeElement(fieldName);");
    }
    for (const auto& member : node.Nodes())
    {
        if (member->IsMemberVariableNode())
        {
            soul::xml::serialization::ast::MemberVariableNode* memberVariable = static_cast<soul::xml::serialization::ast::MemberVariableNode*>(member.get());
            formatter.WriteLine("element->AppendChild(soul::xml::serialization::ToXml(" + memberVariable->Id() + ", \"" + memberVariable->Id() + "\"));"); 
        }
    }
    formatter.WriteLine("return element;");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void WriteFromXmlImplementation(util::CodeFormatter& formatter, soul::xml::serialization::ast::ClassNode& node)
{
    formatter.WriteLine("void " + node.Id() + "::FromXml(soul::xml::Element* element)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    if (!node.BaseClassId().empty())
    {
        formatter.WriteLine(node.BaseClassId() + "::FromXml(element);");
    }
    for (const auto& n : node.Nodes())
    {
        if (n->IsMemberVariableNode())
        {
            soul::xml::serialization::ast::MemberVariableNode* memberVariable = static_cast<soul::xml::serialization::ast::MemberVariableNode*>(n.get());
            formatter.WriteLine("soul::xml::serialization::FromXml(element, \"" + memberVariable->Id() + "\", " + memberVariable->Id() + ");");
        }
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void ImplementationGeneratorVisitor::Visit(soul::xml::serialization::ast::ClassNode& node)
{
    WriteDefaultConstructionImplementation(formatter, node);
    if (node.BaseClassId().empty())
    {
        WriteDestructorImplementation(formatter, node);
    }
    WriteClassNameFunctionImplementation(formatter, node);
    WriteStaticClassNameFunctionImplementation(formatter, node);
    WriteFactoryFunctionImplementation(formatter, node);
    WriteRegisterFunctionImplementation(formatter, node);
    if (node.BaseClassId().empty())
    {
        WriteWriteToXmlFileFunctionImplementation(formatter, node);
        WriteToXmlStringFunctionImplementation(formatter, node);
        WriteReadFromXmlFileFunctionImplementation(formatter, node);
        WriteReadFromXmlStringFunctionImplementation(formatter, node);
    }
    WriteToXmlImplementation(formatter, node);
    WriteFromXmlImplementation(formatter, node);
}

void ImplementationGeneratorVisitor::Visit(soul::xml::serialization::ast::CppBlockNode& node)
{
    if (!node.Implementation()) return;
    WriteIndentedText(formatter, node.CppText());
}

void GenerateClassDefinitions(const std::string& xmlserFileName, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << xmlserFileName << std::endl;
    }
    std::string content = util::ReadFile(xmlserFileName);
    std::u32string ucontent = util::ToUtf32(content);
    if (ucontent.empty())
    {
        throw std::runtime_error("file '" + xmlserFileName + "' is empty: need at least an export module declaration.");
    }
    auto lexer = soul::xml::serialization::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), xmlserFileName);
    lexer.SetRuleNameMapPtr(soul::xml::serialization::spg::rules::GetRuleNameMapPtr());
    using LexerType = decltype(lexer);
    std::unique_ptr<soul::xml::serialization::ast::SourceFileNode> sourceFile = soul::xml::serialization::parser::XmlSerParser<LexerType>::Parse(lexer);
    std::string interfaceFileName = util::GetFullPath(util::Path::ChangeExtension(xmlserFileName, ".cppm"));
    InterfaceGeneratorVisitor interfaceGeneratorVisitor(xmlserFileName, interfaceFileName);
    sourceFile->Accept(interfaceGeneratorVisitor);
    if (verbose)
    {
        std::cout << "==> " << interfaceFileName << std::endl;
    }
    std::string implementationFileName = util::GetFullPath(util::Path::ChangeExtension(xmlserFileName, ".cpp"));
    ImplementationGeneratorVisitor implementationGeneratorVisitor(xmlserFileName, implementationFileName);
    sourceFile->Accept(implementationGeneratorVisitor);
    if (verbose)
    {
        std::cout << "==> " << implementationFileName << std::endl;
    }
}

} // namespace soul::xml::serialization
