// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmdoclib.source_code_printer;

import cmdoclib.input;
import cmajor.ast;
import cmajor.parsers;
import cmajor.symbols;
import soul.xml.dom;
import soul.ast.span;
import util;
import std.filesystem;

namespace cmdoclib {

class OperatorMap
{
public:
    std::string GetOperatorId(const std::string& groupId) const;
    static OperatorMap& Instance();
private:
    OperatorMap();
    std::map<std::string, std::string> operatorMap;
};

OperatorMap::OperatorMap()
{
    operatorMap["operator<<"] = "<<";
    operatorMap["operator>>"] = ">>";
    operatorMap["operator=="] = "==";
    operatorMap["operator="] = "=";
    operatorMap["operator<"] = "<";
    operatorMap["operator->"] = "->";
    operatorMap["operator++"] = "++";
    operatorMap["operator--"] = "--";
    operatorMap["operator+"] = "+";
    operatorMap["operator-"] = "-";
    operatorMap["operator*"] = "*";
    operatorMap["operator/"] = "/";
    operatorMap["operator%"] = "%";
    operatorMap["operator&"] = "&";
    operatorMap["operator|"] = "|";
    operatorMap["operator^"] = "^";
    operatorMap["operator!"] = "!";
    operatorMap["operator~"] = "~";
    operatorMap["operator[]"] = "[]";
    operatorMap["operator()"] = "()";
}

OperatorMap& OperatorMap::Instance()
{
    static OperatorMap instance;
    return instance;
}

std::string OperatorMap::GetOperatorId(const std::string& groupId) const
{
    auto it = operatorMap.find(groupId);
    if (it != operatorMap.cend())
    {
        std::string op = it->second;
        return op;
    }
    else
    {
        return std::string();
    }
}

std::string GetOperatorId(const std::string& groupId)
{
    return OperatorMap::Instance().GetOperatorId(groupId);
}

void WriteToElement(soul::xml::Element* element, const std::string& text)
{
    for (char c : text)
    {
        if (c == ' ')
        {
            element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        }
        else
        {
            element->AppendChild(soul::xml::MakeText(std::string(1, c)));
        }
    }
}

class SourceCodePrinter : public cmajor::ast::Visitor, public cmajor::ast::SourceTokenFormatter
{
public:
    SourceCodePrinter(const std::string& htmlFilePath_, const std::string& title_, const std::string& styleFilePath_, const std::vector<std::string>& lines_,
        cmajor::symbols::SymbolTable& symbolTable_, cmajor::symbols::Module& module_, int sourceFileIndex_);

    void WriteDocument();
    void MoveTo(const soul::ast::Span& span);
    void OpenLine();
    void CloseLine();
    void UseInputLine();
    void WriteRestOfInput();
    std::string MakeSymbolRef(cmajor::symbols::Symbol* symbol);
    std::string MakeSymbolRef(cmajor::symbols::Symbol* symbol, cmajor::symbols::Module* module);
    void WriteSpace(int n);
    void WriteSpace(int n, bool addToCurrentPos);
    void WriteLineNumber(const std::string& lineNumberText);
    void WriteLink(const std::string& linkText, const std::string& href, bool type);
    void WriteFunctionGroupId(const std::string& groupId);

    void Keyword(const std::string& token) override;
    void Identifier(const std::string& token) override;
    void Number(const std::string& token) override;
    void Char(const std::string& token) override;
    void String(const std::string& token) override;
    void Spaces(const std::string& token) override;
    void Comment(const std::string& token) override;
    void Other(const std::string& token) override;

    void WriteSpecifiers(const soul::ast::Span& specifierSpan, cmajor::ast::Specifiers specifiers);
    void WriteAttributes(cmajor::ast::AttributesNode* attributes);

    void Visit(cmajor::ast::AutoNode& autoNode) override;
    void Visit(cmajor::ast::BoolNode& boolNode) override;
    void Visit(cmajor::ast::SByteNode& sbyteNode) override;
    void Visit(cmajor::ast::ByteNode& byteNode) override;
    void Visit(cmajor::ast::ShortNode& shortNode) override;
    void Visit(cmajor::ast::UShortNode& ushortNode) override;
    void Visit(cmajor::ast::IntNode& intNode) override;
    void Visit(cmajor::ast::UIntNode& uintNode) override;
    void Visit(cmajor::ast::LongNode& longNode) override;
    void Visit(cmajor::ast::ULongNode& ulongNode) override;
    void Visit(cmajor::ast::FloatNode& floatNode) override;
    void Visit(cmajor::ast::DoubleNode& doubleNode) override;
    void Visit(cmajor::ast::CharNode& charNode) override;
    void Visit(cmajor::ast::WCharNode& wcharNode) override;
    void Visit(cmajor::ast::UCharNode& ucharNode) override;
    void Visit(cmajor::ast::VoidNode& voidNode) override;

    void Visit(cmajor::ast::BooleanLiteralNode& booleanLiteralNode) override;
    void Visit(cmajor::ast::SByteLiteralNode& sbyteLiteralNode) override;
    void Visit(cmajor::ast::ByteLiteralNode& byteLiteralNode) override;
    void Visit(cmajor::ast::ShortLiteralNode& shortLiteralNode) override;
    void Visit(cmajor::ast::UShortLiteralNode& ushortLiteralNode) override;
    void Visit(cmajor::ast::IntLiteralNode& intLiteralNode) override;
    void Visit(cmajor::ast::UIntLiteralNode& uintLiteralNode) override;
    void Visit(cmajor::ast::LongLiteralNode& longLiteralNode) override;
    void Visit(cmajor::ast::ULongLiteralNode& ulongLiteralNode) override;
    void Visit(cmajor::ast::FloatLiteralNode& floatLiteralNode) override;
    void Visit(cmajor::ast::DoubleLiteralNode& doubleLiteralNode) override;
    void Visit(cmajor::ast::CharLiteralNode& charLiteralNode) override;
    void Visit(cmajor::ast::WCharLiteralNode& wcharLiteralNode) override;
    void Visit(cmajor::ast::UCharLiteralNode& ucharLiteralNode) override;
    void Visit(cmajor::ast::StringLiteralNode& stringLiteralNode) override;
    void Visit(cmajor::ast::WStringLiteralNode& wstringLiteralNode) override;
    void Visit(cmajor::ast::UStringLiteralNode& ustringLiteralNode) override;
    void Visit(cmajor::ast::NullLiteralNode& nullLiteralNode) override;
    void Visit(cmajor::ast::ArrayLiteralNode& arrayLiteralNode) override;
    void Visit(cmajor::ast::StructuredLiteralNode& structuredLiteralNode) override;
    void Visit(cmajor::ast::UuidLiteralNode& uuidLiteralNode) override;

    void Visit(cmajor::ast::CompileUnitNode& compileUnitNode) override;
    void Visit(cmajor::ast::NamespaceNode& namespaceNode) override;
    void Visit(cmajor::ast::AliasNode& aliasNode) override;
    void Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode) override;
    void Visit(cmajor::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::ast::TemplateIdNode& templateIdNode) override;
    void Visit(cmajor::ast::FunctionNode& functionNode) override;
    void Visit(cmajor::ast::ClassNode& classNode) override;
    void Visit(cmajor::ast::ThisInitializerNode& thisInitializerNode) override;
    void Visit(cmajor::ast::BaseInitializerNode& baseInitializerNode) override;
    void Visit(cmajor::ast::MemberInitializerNode& memberInitializerNode) override;
    void Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode) override;
    void Visit(cmajor::ast::ConstructorNode& constructorNode) override;
    void Visit(cmajor::ast::DestructorNode& destructorNode) override;
    void Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode) override;
    void Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode) override;
    void Visit(cmajor::ast::MemberVariableNode& memberVariableNode) override;
    void Visit(cmajor::ast::InterfaceNode& interfaceNode) override;
    void Visit(cmajor::ast::DelegateNode& delegateNode) override;
    void Visit(cmajor::ast::ClassDelegateNode& classDelegateNode) override;

    void Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode) override;
    void Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode) override;
    void Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode) override;
    void Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode) override;
    void Visit(cmajor::ast::PredicateConstraintNode& predicateConstraintNode) override;
    void Visit(cmajor::ast::IsConstraintNode& isConstraintNode) override;
    void Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode) override;
    void Visit(cmajor::ast::TypeNameConstraintNode& typeNameConstraintNode) override;
    void Visit(cmajor::ast::ConstructorConstraintNode& constructorConstraintNode) override;
    void Visit(cmajor::ast::DestructorConstraintNode& destructorConstraintNode) override;
    void Visit(cmajor::ast::MemberFunctionConstraintNode& memberFunctionConstraintNode) override;
    void Visit(cmajor::ast::FunctionConstraintNode& functionConstraintNode) override;
    void Visit(cmajor::ast::AxiomStatementNode& axiomStatementNode) override;
    void Visit(cmajor::ast::AxiomNode& axiomNode) override;
    void Visit(cmajor::ast::ConceptIdNode& conceptIdNode) override;
    void Visit(cmajor::ast::ConceptNode& conceptNode) override;

    void Visit(cmajor::ast::SameConstraintNode& sameConstraintNode) override;
    void Visit(cmajor::ast::DerivedConstraintNode& derivedConstraintNode) override;
    void Visit(cmajor::ast::ConvertibleConstraintNode& convertibleConstraintNode) override;
    void Visit(cmajor::ast::ExplicitlyConvertibleConstraintNode& explicitlyConvertibleConstraintNode) override;
    void Visit(cmajor::ast::CommonConstraintNode& commonConstraintNode) override;
    void Visit(cmajor::ast::NonreferenceTypeConstraintNode& nonreferenceTypeConstraintNode) override;

    void Visit(cmajor::ast::LabelNode& labelNode) override;
    void Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode) override;
    void Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode) override;
    void Visit(cmajor::ast::ReturnStatementNode& returnStatementNode) override;
    void Visit(cmajor::ast::IfStatementNode& ifStatementNode) override;
    void Visit(cmajor::ast::WhileStatementNode& whileStatementNode) override;
    void Visit(cmajor::ast::DoStatementNode& doStatementNode) override;
    void Visit(cmajor::ast::ForStatementNode& forStatementNode) override;
    void Visit(cmajor::ast::BreakStatementNode& breakStatementNode) override;
    void Visit(cmajor::ast::ContinueStatementNode& continueStatementNode) override;
    void Visit(cmajor::ast::GotoStatementNode& gotoStatementNode) override;
    void Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode) override;
    void Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode) override;
    void Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode) override;
    void Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode) override;
    void Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode) override;
    void Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode) override;
    void Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode) override;
    void Visit(cmajor::ast::SwitchStatementNode& switchStatementNode) override;
    void Visit(cmajor::ast::CaseStatementNode& caseStatementNode) override;
    void Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode) override;
    void Visit(cmajor::ast::GotoCaseStatementNode& gotoCaseStatementNode) override;
    void Visit(cmajor::ast::GotoDefaultStatementNode& gotoDefaultStatementNode) override;
    void Visit(cmajor::ast::ThrowStatementNode& throwStatementNode) override;
    void Visit(cmajor::ast::TryStatementNode& tryStatementNode) override;
    void Visit(cmajor::ast::CatchNode& catchNode) override;
    void Visit(cmajor::ast::AssertStatementNode& assertStatementNode) override;
    void Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode) override;
    void Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode) override;
    void Visit(cmajor::ast::ParenthesizedConditionalCompilationExpressionNode& parenthesizedConditionalCompilationExpressionNode) override;

    void Visit(cmajor::ast::TypedefNode& typedefNode) override;
    void Visit(cmajor::ast::ConstantNode& constantNode) override;
    void Visit(cmajor::ast::EnumTypeNode& enumTypeNode) override;
    void Visit(cmajor::ast::EnumConstantNode& enumConstantNode) override;
    void Visit(cmajor::ast::ParameterNode& parameterNode) override;
    void Visit(cmajor::ast::TemplateParameterNode& templateParameterNode) override;

    void Visit(cmajor::ast::ConstNode& constNode) override;
    void Visit(cmajor::ast::LValueRefNode& lvalueRefNode) override;
    void Visit(cmajor::ast::RValueRefNode& rvalueRefNode) override;
    void Visit(cmajor::ast::PointerNode& pointerNode) override;
    void Visit(cmajor::ast::ArrayNode& arrayNode) override;

    void Visit(cmajor::ast::DotNode& dotNode) override;
    void Visit(cmajor::ast::ArrowNode& arrowNode) override;
    void Visit(cmajor::ast::EquivalenceNode& equivalenceNode) override;
    void Visit(cmajor::ast::ImplicationNode& implicationNode) override;
    void Visit(cmajor::ast::DisjunctionNode& disjunctionNode) override;
    void Visit(cmajor::ast::ConjunctionNode& conjunctionNode) override;
    void Visit(cmajor::ast::BitOrNode& bitOrNode) override;
    void Visit(cmajor::ast::BitXorNode& bitXorNode) override;
    void Visit(cmajor::ast::BitAndNode& bitAndNode) override;
    void Visit(cmajor::ast::EqualNode& equalNode) override;
    void Visit(cmajor::ast::NotEqualNode& notEqualNode) override;
    void Visit(cmajor::ast::LessNode& lessNode) override;
    void Visit(cmajor::ast::GreaterNode& greaterNode) override;
    void Visit(cmajor::ast::LessOrEqualNode& lessOrEqualNode) override;
    void Visit(cmajor::ast::GreaterOrEqualNode& greaterOrEqualNode) override;
    void Visit(cmajor::ast::ShiftLeftNode& shiftLeftNode) override;
    void Visit(cmajor::ast::ShiftRightNode& shiftRightNode) override;
    void Visit(cmajor::ast::AddNode& addNode) override;
    void Visit(cmajor::ast::SubNode& subNode) override;
    void Visit(cmajor::ast::MulNode& mulNode) override;
    void Visit(cmajor::ast::DivNode& divNode) override;
    void Visit(cmajor::ast::RemNode& remNode) override;
    void Visit(cmajor::ast::NotNode& notNode) override;
    void Visit(cmajor::ast::UnaryPlusNode& unaryPlusNode) override;
    void Visit(cmajor::ast::UnaryMinusNode& unaryMinusNode) override;
    void Visit(cmajor::ast::PrefixIncrementNode& prefixIncrementNode) override;
    void Visit(cmajor::ast::PrefixDecrementNode& prefixDecrementNode) override;
    void Visit(cmajor::ast::DerefNode& derefNode) override;
    void Visit(cmajor::ast::AddrOfNode& addrOfNode) override;
    void Visit(cmajor::ast::ComplementNode& complementNode) override;
    void Visit(cmajor::ast::IsNode& isNode) override;
    void Visit(cmajor::ast::AsNode& asNode) override;
    void Visit(cmajor::ast::IndexingNode& indexingNode) override;
    void Visit(cmajor::ast::InvokeNode& invokeNode) override;
    void Visit(cmajor::ast::PostfixIncrementNode& postfixIncrementNode) override;
    void Visit(cmajor::ast::PostfixDecrementNode& postfixDecrementNode) override;
    void Visit(cmajor::ast::SizeOfNode& sizeOfNode) override;
    void Visit(cmajor::ast::TypeNameNode& typeNameNode) override;
    void Visit(cmajor::ast::CastNode& castNode) override;
    void Visit(cmajor::ast::ConstructNode& constructNode) override;
    void Visit(cmajor::ast::NewNode& newNode) override;
    void Visit(cmajor::ast::ThisNode& thisNode) override;
    void Visit(cmajor::ast::BaseNode& baseNode) override;
    void Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode) override;
private:
    Input* input;
    std::string htmlFilePath;
    std::string title;
    std::string styleFilePath;
    std::unique_ptr<soul::xml::Document> htmlDoc;
    std::unique_ptr<soul::xml::Element> htmlElement;
    std::unique_ptr<soul::xml::Element> bodyElement;
    std::unique_ptr<soul::xml::Element> lineElement;
    std::vector<std::string> lines;
    const std::vector<int>* lineStarts;
    int numDigits;
    int currentSourceLineNumber;
    int currentPos;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::symbols::Module& module;
    int sourceFileIndex;
    cmajor::symbols::FunctionSymbol* invokeFunctionSymbol;
    bool inBlockComment;
    soul::ast::LineColLen lineColLen;
};

SourceCodePrinter::SourceCodePrinter(const std::string& htmlFilePath_, const std::string& title_, const std::string& styleFilePath_, const std::vector<std::string>& lines_,
    cmajor::symbols::SymbolTable& symbolTable_, cmajor::symbols::Module& module_, int sourceFileIndex_) :
    input(GetInputPtr()), htmlFilePath(htmlFilePath_), title(title_), styleFilePath(styleFilePath_),
    htmlDoc(soul::xml::MakeDocument()), htmlElement(soul::xml::MakeElement("html")), bodyElement(soul::xml::MakeElement("body")), lines(lines_),
    lineStarts(nullptr), numDigits(util::Log10(static_cast<int>(lines.size()))), currentSourceLineNumber(1), currentPos(0), symbolTable(symbolTable_), module(module_),
    sourceFileIndex(sourceFileIndex_), invokeFunctionSymbol(nullptr), inBlockComment(false)
{
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> metaElement(soul::xml::MakeElement("meta"));
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement.release());
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText(title));
    headElement->AppendChild(titleElement.release());
    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("link"));
    linkElement->SetAttribute("rel", "stylesheet");
    linkElement->SetAttribute("type", "text/css");
    linkElement->SetAttribute("href", styleFilePath);
    headElement->AppendChild(linkElement.release());
    htmlElement->AppendChild(headElement.release());
}

void SourceCodePrinter::WriteDocument()
{
    std::ofstream htmlFile(htmlFilePath);
    util::CodeFormatter formatter(htmlFile);
    formatter.SetIndentSize(1);
    htmlElement->AppendChild(bodyElement.release());
    htmlDoc->AppendChild(htmlElement.release());
    htmlDoc->Write(formatter);
}

void SourceCodePrinter::MoveTo(const soul::ast::Span& span)
{
    lineColLen = soul::ast::SpanToLineColLen(span, *lineStarts);
    int lineNumber = lineColLen.line;
    while (lineNumber > currentSourceLineNumber)
    {
        if (!lineElement)
        {
            UseInputLine();
        }
        else
        {
            CloseLine();
        }
    }
    if (!lineElement)
    {
        OpenLine();
    }
    if (!span.IsValid()) return;
    if (currentPos < span.pos)
    {
        int length = span.pos - currentPos;
        WriteSpace(length);
    }
}

void SourceCodePrinter::OpenLine()
{
    lineElement.reset(soul::xml::MakeElement("span"));
    lineElement->SetAttribute("class", "code");
    lineElement->SetAttribute("xml:space", "preserve");
    lineElement->SetAttribute("id", std::to_string(currentSourceLineNumber));
    std::string lineNumberText = util::ToUtf8(util::FormatNumber(currentSourceLineNumber, numDigits));
    WriteLineNumber(lineNumberText);
    WriteSpace(1, false);
    if (lineStarts->size() < currentSourceLineNumber)
    {
        throw std::runtime_error("invalid line starts vector");
    }
    int lineStart = (*lineStarts)[currentSourceLineNumber];
    currentPos = lineStart;
}

void SourceCodePrinter::CloseLine()
{
    bodyElement->AppendChild(lineElement.release());
    bodyElement->AppendChild(soul::xml::MakeElement("br"));
    ++currentSourceLineNumber;
}

void SourceCodePrinter::UseInputLine()
{
    const std::string& line = lines[currentSourceLineNumber - 1];
    OpenLine();
    cmajor::parser::ParseSourceLine(line, this, inBlockComment);
    CloseLine();
}

void SourceCodePrinter::WriteRestOfInput()
{
    int n = static_cast<int>(lines.size());
    while (currentSourceLineNumber < n)
    {
        UseInputLine();
    }
}

std::string SourceCodePrinter::MakeSymbolRef(cmajor::symbols::Symbol* symbol)
{
    return MakeSymbolRef(symbol, symbol->GetModule());
}

std::string SourceCodePrinter::MakeSymbolRef(cmajor::symbols::Symbol* symbol, cmajor::symbols::Module* module)
{
    if (!symbol) return std::string();
    if (symbol->IsParentSymbol())
    {
        if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(symbol);
            return MakeSymbolRef(specialization->GetClassTemplate());
        }
        else
        {
            std::string file = util::ToUtf8(symbol->Id());
            std::string contentFilePath = "../doc";
            if (file.empty() || file == "ns_")
            {
                file = "index";
                contentFilePath = "..";
            }
            if (module)
            {
                std::string libraryPrefix;
                if (module != symbolTable.GetModule())
                {
                    auto it = input->libraryPrefixMap.find(util::ToUtf8(module->Name()));
                    if (it != input->libraryPrefixMap.cend())
                    {
                        libraryPrefix = it->second;
                        if (!libraryPrefix.empty())
                        {
                            libraryPrefix = util::Path::Combine("..", libraryPrefix);
                        }
                    }
                    else
                    {
                        std::cerr << "module '" << util::ToUtf8(module->Name()) <<  "' not found in source code printer" << "\n";
                        return std::string();
                    }
                    if (file == "index")
                    {
                        contentFilePath = util::Path::Combine(util::Path::Combine("../..", libraryPrefix), util::ToUtf8(module->Name()));
                    }
                    else
                    {
                        contentFilePath = util::Path::Combine(util::Path::Combine("../..", libraryPrefix), util::ToUtf8(module->Name()) + "/doc");
                    }
                }
            }
            return util::Path::Combine(contentFilePath, file + ".html");
        }
    }
    else if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupSymbol || symbol->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol ||
        !symbol->Parent() || symbol->Parent()->GetSymbolType() == cmajor::symbols::SymbolType::declarationBlock || symbol->Parent()->IsFunctionSymbol())
    {
        return std::string();
    }
    else
    {
        return MakeSymbolRef(symbol->Parent(), module) + "#" + util::ToUtf8(symbol->Id());
    }
}

void SourceCodePrinter::WriteSpace(int n)
{
    return WriteSpace(n, true);
}

void SourceCodePrinter::WriteSpace(int n, bool addToCurrentPos)
{
    for (int i = 0; i < n; ++i)
    {
        lineElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
    }
    if (addToCurrentPos)
    {
        currentPos += n;
    }
}

void SourceCodePrinter::WriteLineNumber(const std::string& lineNumberText)
{
    std::unique_ptr<soul::xml::Element> lineNumberElement(soul::xml::MakeElement("span"));
    lineNumberElement->SetAttribute("class", "lineNumber");
    WriteToElement(lineNumberElement.get(), lineNumberText);
    lineElement->AppendChild(lineNumberElement.release());
}

void SourceCodePrinter::WriteLink(const std::string& linkText, const std::string& href, bool type)
{
    if (href.empty())
    {
        std::unique_ptr<soul::xml::Element> identifierElement(soul::xml::MakeElement("span"));
        if (type)
        {
            identifierElement->SetAttribute("class", "type");
        }
        else
        {
            identifierElement->SetAttribute("class", "identifier");
        }
        WriteToElement(identifierElement.get(), linkText);
        lineElement->AppendChild(identifierElement.release());
        currentPos += static_cast<int>(linkText.length());
    }
    else
    {
        std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
        linkElement->SetAttribute("href", href);
        WriteToElement(linkElement.get(), linkText);
        lineElement->AppendChild(linkElement.release());
        currentPos += static_cast<int>(linkText.length());
    }
}

void SourceCodePrinter::WriteFunctionGroupId(const std::string& groupId)
{
    std::string operatorId = GetOperatorId(groupId);
    if (operatorId.empty())
    {
        Identifier(groupId);
    }
    else
    {
        Keyword("operator");
        Other(operatorId);
    }
}

void SourceCodePrinter::Keyword(const std::string& token)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("span"));
    keywordElement->SetAttribute("class", "keyword");
    WriteToElement(keywordElement.get(), token);
    lineElement->AppendChild(keywordElement.release());
    currentPos += static_cast<int>(token.length());
}

void SourceCodePrinter::Identifier(const std::string& token)
{
    std::unique_ptr<soul::xml::Element> identifierElement(soul::xml::MakeElement("span"));
    identifierElement->SetAttribute("class", "identifier");
    WriteToElement(identifierElement.get(), token);
    lineElement->AppendChild(identifierElement.release());
    currentPos += static_cast<int>(token.length());
}

void SourceCodePrinter::Number(const std::string& token)
{
    std::unique_ptr<soul::xml::Element> numberElement(soul::xml::MakeElement("span"));
    numberElement->SetAttribute("class", "number");
    WriteToElement(numberElement.get(), token);
    lineElement->AppendChild(numberElement.release());
    currentPos += static_cast<int>(token.length());
}

void SourceCodePrinter::Char(const std::string& token)
{
    std::unique_ptr<soul::xml::Element> charElement(soul::xml::MakeElement("span"));
    charElement->SetAttribute("class", "char");
    WriteToElement(charElement.get(), token);
    lineElement->AppendChild(charElement.release());
    currentPos += static_cast<int>(token.length());
}

void SourceCodePrinter::String(const std::string& token)
{
    std::unique_ptr<soul::xml::Element> stringElement(soul::xml::MakeElement("span"));
    stringElement->SetAttribute("class", "string");
    WriteToElement(stringElement.get(), token);
    lineElement->AppendChild(stringElement.release());
    currentPos += static_cast<int>(token.length());
}

void SourceCodePrinter::Spaces(const std::string& token)
{
    WriteSpace(static_cast<int>(token.length()));
}

void SourceCodePrinter::Comment(const std::string& token)
{
    std::unique_ptr<soul::xml::Element> commentElement(soul::xml::MakeElement("span"));
    commentElement->SetAttribute("class", "comment");
    WriteToElement(commentElement.get(), token);
    lineElement->AppendChild(commentElement.release());
    currentPos += static_cast<int>(token.length());
}

void SourceCodePrinter::Other(const std::string& token)
{
    std::unique_ptr<soul::xml::Element> otherElement(soul::xml::MakeElement("span"));
    otherElement->SetAttribute("class", "other");
    WriteToElement(otherElement.get(), token);
    lineElement->AppendChild(otherElement.release());
    currentPos += static_cast<int>(token.length());
}

void SourceCodePrinter::WriteSpecifiers(const soul::ast::Span& specifierSpan, cmajor::ast::Specifiers specifiers)
{
    if (specifiers == cmajor::ast::Specifiers::none) return;
    MoveTo(specifierSpan);
    bool space = false;
    if ((specifiers & cmajor::ast::Specifiers::public_) != cmajor::ast::Specifiers::none)
    {
        Keyword("public");
        space = true;
    }
    else if ((specifiers & cmajor::ast::Specifiers::protected_) != cmajor::ast::Specifiers::none)
    {
        Keyword("protected");
        space = true;
    }
    else if ((specifiers & cmajor::ast::Specifiers::private_) != cmajor::ast::Specifiers::none)
    {
        Keyword("private");
        space = true;
    }
    else if ((specifiers & cmajor::ast::Specifiers::internal_) != cmajor::ast::Specifiers::none)
    {
        Keyword("internal");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("static");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("virtual");
        space = true;
    }
    else if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("override");
        space = true;
    }
    else if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("abstract");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("inline");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("explicit");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("extern");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("suppress");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("default");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("constexpr");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("cdecl");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("nothrow");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("throw");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("new");
        space = true;
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        if (space)
        {
            WriteSpace(1);
        }
        Keyword("unit_test");
        space = true;
    }
}

void SourceCodePrinter::WriteAttributes(cmajor::ast::AttributesNode* attributes)
{
    MoveTo(attributes->GetSpan());
    Other("[");
    int n = static_cast<int>(attributes->GetAttributes().size());
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::AttributeNode* attribute = attributes->GetAttributes()[i].get();
        MoveTo(attribute->GetSpan());
        Identifier(util::ToUtf8(attribute->Name()));
        if (attribute->HasValue())
        {
            Other("=");
            String("\"" + util::ToUtf8(util::StringStr(attribute->Value())) + "\"");
        }
    }
    Other("]");
}

void SourceCodePrinter::Visit(cmajor::ast::AutoNode& autoNode)
{
    MoveTo(autoNode.GetSpan());
    Keyword("auto");
}

void SourceCodePrinter::Visit(cmajor::ast::BoolNode& boolNode)
{
    MoveTo(boolNode.GetSpan());
    Keyword("bool");
}

void SourceCodePrinter::Visit(cmajor::ast::SByteNode& sbyteNode)
{
    MoveTo(sbyteNode.GetSpan());
    Keyword("sbyte");
}

void SourceCodePrinter::Visit(cmajor::ast::ByteNode& byteNode)
{
    MoveTo(byteNode.GetSpan());
    Keyword("byte");
}

void SourceCodePrinter::Visit(cmajor::ast::ShortNode& shortNode)
{
    MoveTo(shortNode.GetSpan());
    Keyword("short");
}

void SourceCodePrinter::Visit(cmajor::ast::UShortNode& ushortNode)
{
    MoveTo(ushortNode.GetSpan());
    Keyword("ushort");
}

void SourceCodePrinter::Visit(cmajor::ast::IntNode& intNode)
{
    MoveTo(intNode.GetSpan());
    Keyword("int");
}

void SourceCodePrinter::Visit(cmajor::ast::UIntNode& uintNode)
{
    MoveTo(uintNode.GetSpan());
    Keyword("uint");
}

void SourceCodePrinter::Visit(cmajor::ast::LongNode& longNode)
{
    MoveTo(longNode.GetSpan());
    Keyword("long");
}

void SourceCodePrinter::Visit(cmajor::ast::ULongNode& ulongNode)
{
    MoveTo(ulongNode.GetSpan());
    Keyword("ulong");
}

void SourceCodePrinter::Visit(cmajor::ast::FloatNode& floatNode)
{
    MoveTo(floatNode.GetSpan());
    Keyword("float");
}

void SourceCodePrinter::Visit(cmajor::ast::DoubleNode& doubleNode)
{
    MoveTo(doubleNode.GetSpan());
    Keyword("double");
}

void SourceCodePrinter::Visit(cmajor::ast::CharNode& charNode)
{
    MoveTo(charNode.GetSpan());
    Keyword("char");
}

void SourceCodePrinter::Visit(cmajor::ast::WCharNode& wcharNode)
{
    MoveTo(wcharNode.GetSpan());
    Keyword("wchar");
}

void SourceCodePrinter::Visit(cmajor::ast::UCharNode& ucharNode)
{
    MoveTo(ucharNode.GetSpan());
    Keyword("uchar");
}

void SourceCodePrinter::Visit(cmajor::ast::VoidNode& voidNode)
{
    MoveTo(voidNode.GetSpan());
    Keyword("void");
}

void SourceCodePrinter::Visit(cmajor::ast::BooleanLiteralNode& booleanLiteralNode)
{
    MoveTo(booleanLiteralNode.GetSpan());
    Keyword(booleanLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::SByteLiteralNode& sbyteLiteralNode)
{
    MoveTo(sbyteLiteralNode.GetSpan());
    Number(sbyteLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::ByteLiteralNode& byteLiteralNode)
{
    MoveTo(byteLiteralNode.GetSpan());
    Number(byteLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::ShortLiteralNode& shortLiteralNode)
{
    MoveTo(shortLiteralNode.GetSpan());
    Number(shortLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::UShortLiteralNode& ushortLiteralNode)
{
    MoveTo(ushortLiteralNode.GetSpan());
    Number(ushortLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::IntLiteralNode& intLiteralNode)
{
    MoveTo(intLiteralNode.GetSpan());
    Number(intLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::UIntLiteralNode& uintLiteralNode)
{
    MoveTo(uintLiteralNode.GetSpan());
    Number(uintLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::LongLiteralNode& longLiteralNode)
{
    MoveTo(longLiteralNode.GetSpan());
    Number(longLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::ULongLiteralNode& ulongLiteralNode)
{
    MoveTo(ulongLiteralNode.GetSpan());
    Number(ulongLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::FloatLiteralNode& floatLiteralNode)
{
    MoveTo(floatLiteralNode.GetSpan());
    Number(floatLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::DoubleLiteralNode& doubleLiteralNode)
{
    MoveTo(doubleLiteralNode.GetSpan());
    Number(doubleLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::CharLiteralNode& charLiteralNode)
{
    MoveTo(charLiteralNode.GetSpan());
    Char(charLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::WCharLiteralNode& wcharLiteralNode)
{
    MoveTo(wcharLiteralNode.GetSpan());
    Char(wcharLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::UCharLiteralNode& ucharLiteralNode)
{
    MoveTo(ucharLiteralNode.GetSpan());
    Char(ucharLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::StringLiteralNode& stringLiteralNode)
{
    MoveTo(stringLiteralNode.GetSpan());
    String(stringLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::WStringLiteralNode& wstringLiteralNode)
{
    MoveTo(wstringLiteralNode.GetSpan());
    String(wstringLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::UStringLiteralNode& ustringLiteralNode)
{
    MoveTo(ustringLiteralNode.GetSpan());
    String(ustringLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::NullLiteralNode& nullLiteralNode)
{
    MoveTo(nullLiteralNode.GetSpan());
    Keyword(nullLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::ArrayLiteralNode& arrayLiteralNode)
{
    MoveTo(arrayLiteralNode.GetSpan());
    Other("[");
    int n = arrayLiteralNode.Values().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        arrayLiteralNode.Values()[i]->Accept(*this);
    }
    Other("]");
}

void SourceCodePrinter::Visit(cmajor::ast::StructuredLiteralNode& structuredLiteralNode)
{
    MoveTo(structuredLiteralNode.GetSpan());
    Other("{");
    int n = structuredLiteralNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        structuredLiteralNode.Members()[i]->Accept(*this);
    }
    Other("}");
}

void SourceCodePrinter::Visit(cmajor::ast::UuidLiteralNode& uuidLiteralNode)
{
    MoveTo(uuidLiteralNode.GetSpan());
    Other(uuidLiteralNode.ToString());
}

void SourceCodePrinter::Visit(cmajor::ast::CompileUnitNode& compileUnitNode)
{
    std::string content = util::ReadFile(compileUnitNode.FilePath());
    std::u32string ucontent = util::ToUtf32(content);
    compileUnitNode.ComputeLineStarts(ucontent);
    lineStarts = &compileUnitNode.LineStarts();
    compileUnitNode.GlobalNs()->Accept(*this);
    if (lineElement)
    {
        CloseLine();
    }
    WriteRestOfInput();
}

void SourceCodePrinter::Visit(cmajor::ast::NamespaceNode& namespaceNode)
{
    int n = namespaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* node = namespaceNode.Members()[i];
        node->Accept(*this);
    }
}

void SourceCodePrinter::Visit(cmajor::ast::AliasNode& aliasNode)
{
    MoveTo(aliasNode.GetSpan());
    Keyword("using");
    WriteSpace(1);
    aliasNode.Id()->Accept(*this);
    Other(" = ");
    aliasNode.TypeExpr()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode)
{
    MoveTo(namespaceImportNode.GetSpan());
    Keyword("using");
    WriteSpace(1);
    namespaceImportNode.Ns()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    MoveTo(identifierNode.GetSpan());
    cmajor::symbols::FunctionSymbol* invokedFunctionSymbol = symbolTable.GetInvoke(&identifierNode);
    if (invokedFunctionSymbol)
    {
        WriteLink(util::ToUtf8(identifierNode.Str()), MakeSymbolRef(invokedFunctionSymbol), false);
    }
    else
    {
        cmajor::symbols::Symbol* symbol = symbolTable.GetMappedSymbol(&identifierNode);
        if (symbol)
        {
            WriteLink(util::ToUtf8(identifierNode.Str()), MakeSymbolRef(symbol), symbol->IsTypeSymbol());
        }
        else
        {
            Identifier(util::ToUtf8(identifierNode.Str()));
        }
    }
}

void SourceCodePrinter::Visit(cmajor::ast::TemplateIdNode& templateIdNode)
{
    MoveTo(templateIdNode.GetSpan());
    templateIdNode.Primary()->Accept(*this);
    Other("<");
    int n = templateIdNode.TemplateArguments().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::Node* arg = templateIdNode.TemplateArguments()[i];
        arg->Accept(*this);
    }
    Other(">");
}

void SourceCodePrinter::Visit(cmajor::ast::FunctionNode& functionNode)
{
    if (lineColLen.line == 422)
    {
        int x = 0;
    }
    MoveTo(functionNode.GetSpan());
    if (functionNode.GetAttributes())
    {
        WriteAttributes(functionNode.GetAttributes());
    }
    WriteSpecifiers(functionNode.SpecifierSpan(), functionNode.GetSpecifiers());
    if (functionNode.ReturnTypeExpr())
    {
        functionNode.ReturnTypeExpr()->Accept(*this);
    }
    MoveTo(functionNode.GroupIdSpan());
    cmajor::symbols::Symbol* symbol = symbolTable.GetMappedSymbol(&functionNode);
    if (symbol)
    {
        WriteLink(util::ToUtf8(functionNode.GroupId()), MakeSymbolRef(symbol), false);
    }
    else
    {
        WriteFunctionGroupId(util::ToUtf8(functionNode.GroupId()));
    }
    int nt = functionNode.TemplateParameters().Count();
    if (nt > 0)
    {
        Other("<");
        for (int i = 0; i < nt; ++i)
        {
            if (i > 0)
            {
                Other(", ");
            }
            functionNode.TemplateParameters()[i]->Accept(*this);
        }
        Other(">");
    }
    Other("(");
    int n = functionNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        functionNode.Parameters()[i]->Accept(*this);
    }
    Other(")");
    if (functionNode.WhereConstraint())
    {
        functionNode.WhereConstraint()->Accept(*this);
    }
    if (functionNode.Body())
    {
        functionNode.Body()->Accept(*this);
    }
    else
    {
        Other(";");
    }
}

void SourceCodePrinter::Visit(cmajor::ast::ClassNode& classNode)
{
    if (classNode.GetAttributes())
    {
        WriteAttributes(classNode.GetAttributes());
    }
    WriteSpecifiers(classNode.SpecifierSpan(), classNode.GetSpecifiers());
    MoveTo(classNode.ClassSpan());
    Keyword("class");
    classNode.Id()->Accept(*this);
    int nt = classNode.TemplateParameters().Count();
    if (nt > 0)
    {
        Other("<");
        for (int i = 0; i < nt; ++i)
        {
            if (i > 0)
            {
                Other(", ");
            }
            classNode.TemplateParameters()[i]->Accept(*this);
        }
        Other(">");
    }
    int nb = classNode.BaseClassOrInterfaces().Count();
    if (nb > 0)
    {
        Other(" : ");
        for (int i = 0; i < nb; ++i)
        {
            if (i > 0)
            {
                Other(", ");
            }
            cmajor::ast::Node* baseClassOrInterface = classNode.BaseClassOrInterfaces()[i];
            baseClassOrInterface->Accept(*this);
        }
    }
    if (classNode.WhereConstraint())
    {
        classNode.WhereConstraint()->Accept(*this);
    }
    MoveTo(classNode.BeginBraceSpan());
    Other("{");
    int n = classNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* node = classNode.Members()[i];
        node->Accept(*this);
    }
    MoveTo(classNode.EndBraceSpan());
    Other("}");
}

void SourceCodePrinter::Visit(cmajor::ast::ThisInitializerNode& thisInitializerNode)
{
    MoveTo(thisInitializerNode.GetSpan());
    Keyword("this");
    Other("(");
    int n = thisInitializerNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::Node* arg = thisInitializerNode.Arguments()[i];
        arg->Accept(*this);
    }
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::BaseInitializerNode& baseInitializerNode)
{
    MoveTo(baseInitializerNode.GetSpan());
    Keyword("base");
    Other("(");
    int n = baseInitializerNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::Node* arg = baseInitializerNode.Arguments()[i];
        arg->Accept(*this);
    }
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::MemberInitializerNode& memberInitializerNode)
{
    MoveTo(memberInitializerNode.GetSpan());
    memberInitializerNode.MemberId()->Accept(*this);
    Other("(");
    int n = memberInitializerNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::Node* arg = memberInitializerNode.Arguments()[i];
        arg->Accept(*this);
    }
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode)
{
    if (staticConstructorNode.GetAttributes())
    {
        WriteAttributes(staticConstructorNode.GetAttributes());
    }
    WriteSpecifiers(staticConstructorNode.SpecifierSpan(), staticConstructorNode.GetSpecifiers());
    staticConstructorNode.ClassId()->Accept(*this);
    Other("()");
    int n = staticConstructorNode.Initializers().Count();
    if (n > 0)
    {
        Other(" : ");
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                Other(", ");
            }
            cmajor::ast::Node* initializer = staticConstructorNode.Initializers()[i];
            initializer->Accept(*this);
        }
    }
    if (staticConstructorNode.WhereConstraint())
    {
        staticConstructorNode.WhereConstraint()->Accept(*this);
    }
    if (staticConstructorNode.Body())
    {
        staticConstructorNode.Body()->Accept(*this);
    }
    else
    {
        Other(";");
    }
}

void SourceCodePrinter::Visit(cmajor::ast::ConstructorNode& constructorNode)
{
    if (constructorNode.GetAttributes())
    {
        WriteAttributes(constructorNode.GetAttributes());
    }
    WriteSpecifiers(constructorNode.SpecifierSpan(), constructorNode.GetSpecifiers());
    constructorNode.ClassId()->Accept(*this);
    Other("(");
    int n = constructorNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::ParameterNode* parameter = constructorNode.Parameters()[i];
        parameter->Accept(*this);
    }
    Other(")");
    int ni = constructorNode.Initializers().Count();
    if (ni > 0)
    {
        Other(" : ");
        for (int i = 0; i < ni; ++i)
        {
            if (i > 0)
            {
                Other(", ");
            }
            cmajor::ast::Node* initializer = constructorNode.Initializers()[i];
            initializer->Accept(*this);
        }
    }
    if (constructorNode.WhereConstraint())
    {
        constructorNode.WhereConstraint()->Accept(*this);
    }
    if (constructorNode.Body())
    {
        constructorNode.Body()->Accept(*this);
    }
    else
    {
        Other(";");
    }
}

void SourceCodePrinter::Visit(cmajor::ast::DestructorNode& destructorNode)
{
    if (destructorNode.GetAttributes())
    {
        WriteAttributes(destructorNode.GetAttributes());
    }
    WriteSpecifiers(destructorNode.SpecifierSpan(), destructorNode.GetSpecifiers());
    if (destructorNode.GetSpecifiers() != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
    }
    Other("~");
    destructorNode.ClassId()->Accept(*this);
    Other("(");
    Other(")");
    if (destructorNode.WhereConstraint())
    {
        destructorNode.WhereConstraint()->Accept(*this);
    }
    if (destructorNode.Body())
    {
        destructorNode.Body()->Accept(*this);
    }
    else
    {
        Other(";");
    }
}

void SourceCodePrinter::Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode)
{
    if (memberFunctionNode.GetAttributes())
    {
        WriteAttributes(memberFunctionNode.GetAttributes());
    }
    WriteSpecifiers(memberFunctionNode.SpecifierSpan(), memberFunctionNode.GetSpecifiers());
    memberFunctionNode.ReturnTypeExpr()->Accept(*this);
    MoveTo(memberFunctionNode.GroupIdSpan());
    cmajor::symbols::Symbol* symbol = symbolTable.GetMappedSymbol(&memberFunctionNode);
    if (symbol)
    {
        WriteLink(util::ToUtf8(memberFunctionNode.GroupId()), MakeSymbolRef(symbol), false);
    }
    else
    {
        WriteFunctionGroupId(util::ToUtf8(memberFunctionNode.GroupId()));
    }
    Other("(");
    int n = memberFunctionNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::ParameterNode* parameter = memberFunctionNode.Parameters()[i];
        parameter->Accept(*this);
    }
    Other(")");
    if ((memberFunctionNode.GetSpecifiers() & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
        Keyword("const");
    }
    if (memberFunctionNode.WhereConstraint())
    {
        memberFunctionNode.WhereConstraint()->Accept(*this);
    }
    if (memberFunctionNode.Body())
    {
        memberFunctionNode.Body()->Accept(*this);
    }
    else
    {
        Other(";");
    }
}

void SourceCodePrinter::Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode)
{
    if (conversionFunctionNode.GetAttributes())
    {
        WriteAttributes(conversionFunctionNode.GetAttributes());
    }
    WriteSpecifiers(conversionFunctionNode.SpecifierSpan(), conversionFunctionNode.GetSpecifiers());
    if (conversionFunctionNode.GetSpecifiers() != cmajor::ast::Specifiers::none && conversionFunctionNode.GetSpecifiers() != cmajor::ast::Specifiers::const_)
    {
        WriteSpace(1);
    }
    Keyword("operator");
    conversionFunctionNode.ReturnTypeExpr()->Accept(*this);
    Other("(");
    Other(")");
    if ((conversionFunctionNode.GetSpecifiers() & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
        Keyword("const");
    }
    if (conversionFunctionNode.WhereConstraint())
    {
        conversionFunctionNode.WhereConstraint()->Accept(*this);
    }
    if (conversionFunctionNode.Body())
    {
        conversionFunctionNode.Body()->Accept(*this);
    }
    else
    {
        Other(";");
    }
}

void SourceCodePrinter::Visit(cmajor::ast::MemberVariableNode& memberVariableNode)
{
    if (memberVariableNode.GetAttributes())
    {
        WriteAttributes(memberVariableNode.GetAttributes());
    }
    WriteSpecifiers(memberVariableNode.SpecifierSpan(), memberVariableNode.GetSpecifiers());
    memberVariableNode.TypeExpr()->Accept(*this);
    memberVariableNode.Id()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::InterfaceNode& interfaceNode)
{
    if (interfaceNode.GetAttributes())
    {
        WriteAttributes(interfaceNode.GetAttributes());
    }
    WriteSpecifiers(interfaceNode.SpecifierSpan(), interfaceNode.GetSpecifiers());
    if (interfaceNode.GetSpecifiers() != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
    }
    Keyword("interface");
    interfaceNode.Id()->Accept(*this);
    MoveTo(interfaceNode.BeginBraceSpan());
    Other("{");
    int n = interfaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* member = interfaceNode.Members()[i];
        member->Accept(*this);
    }
    MoveTo(interfaceNode.EndBraceSpan());
    Other("}");
}

void SourceCodePrinter::Visit(cmajor::ast::DelegateNode& delegateNode)
{
    MoveTo(delegateNode.GetSpan());
    WriteSpecifiers(delegateNode.GetSpan(), delegateNode.GetSpecifiers());
    if (delegateNode.GetSpecifiers() != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
    }
    Keyword("delegate");
    delegateNode.ReturnTypeExpr()->Accept(*this);
    delegateNode.Id()->Accept(*this);
    Other("(");
    int n = delegateNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::ParameterNode* parameter = delegateNode.Parameters()[i];
        parameter->Accept(*this);
    }
    Other(");");
}

void SourceCodePrinter::Visit(cmajor::ast::ClassDelegateNode& classDelegateNode)
{
    MoveTo(classDelegateNode.GetSpan());
    WriteSpecifiers(classDelegateNode.GetSpan(), classDelegateNode.GetSpecifiers());
    if (classDelegateNode.GetSpecifiers() != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
    }
    Keyword("class");
    WriteSpace(1);
    Keyword("delegate");
    classDelegateNode.ReturnTypeExpr()->Accept(*this);
    classDelegateNode.Id()->Accept(*this);
    Other("(");
    int n = classDelegateNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::ParameterNode* parameter = classDelegateNode.Parameters()[i];
        parameter->Accept(*this);
    }
    Other(");");
}

void SourceCodePrinter::Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode)
{
    MoveTo(parenthesizedConstraintNode.GetSpan());
    Other("(");
    parenthesizedConstraintNode.Constraint()->Accept(*this);
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode)
{
    MoveTo(disjunctiveConstraintNode.GetSpan());
    disjunctiveConstraintNode.Left()->Accept(*this);
    Keyword(" or ");
    disjunctiveConstraintNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode)
{
    MoveTo(conjunctiveConstraintNode.GetSpan());
    conjunctiveConstraintNode.Left()->Accept(*this);
    Keyword(" and ");
    conjunctiveConstraintNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode)
{
    MoveTo(whereConstraintNode.GetSpan());
    Keyword("where");
    whereConstraintNode.Constraint()->Accept(*this);
    if (whereConstraintNode.Semicolon())
    {
        Other(";");
    }
}

void SourceCodePrinter::Visit(cmajor::ast::PredicateConstraintNode& predicateConstraintNode)
{
    MoveTo(predicateConstraintNode.GetSpan());
    predicateConstraintNode.InvokeExpr()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::IsConstraintNode& isConstraintNode)
{
    MoveTo(isConstraintNode.GetSpan());
    isConstraintNode.TypeExpr()->Accept(*this);
    Keyword(" is ");
    isConstraintNode.ConceptOrTypeName()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode)
{
    MoveTo(multiParamConstraintNode.GetSpan());
    multiParamConstraintNode.ConceptId()->Accept(*this);
    Other("<");
    int n = multiParamConstraintNode.TypeExprs().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        multiParamConstraintNode.TypeExprs()[i]->Accept(*this);
    }
    Other(">");
}

void SourceCodePrinter::Visit(cmajor::ast::TypeNameConstraintNode& typeNameConstraintNode)
{
    MoveTo(typeNameConstraintNode.GetSpan());
    Keyword("typename");
    typeNameConstraintNode.TypeId()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::ConstructorConstraintNode& constructorConstraintNode)
{
    MoveTo(constructorConstraintNode.GetSpan());
    constructorConstraintNode.TypeParamId()->Accept(*this);
    Other("(");
    int n = constructorConstraintNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::ParameterNode* parameter = constructorConstraintNode.Parameters()[i];
        parameter->Accept(*this);
    }
    Other(");");
}

void SourceCodePrinter::Visit(cmajor::ast::DestructorConstraintNode& destructorConstraintNode)
{
    MoveTo(destructorConstraintNode.GetSpan());
    Other("~");
    destructorConstraintNode.TypeParamId()->Accept(*this);
    Other("(");
    Other(");");
}

void SourceCodePrinter::Visit(cmajor::ast::MemberFunctionConstraintNode& memberFunctionConstraintNode)
{
    MoveTo(memberFunctionConstraintNode.GetSpan());
    memberFunctionConstraintNode.ReturnTypeExpr()->Accept(*this);
    memberFunctionConstraintNode.TypeParamId()->Accept(*this);
    Other(".");
    WriteFunctionGroupId(util::ToUtf8(memberFunctionConstraintNode.GroupId()));
    Other("(");
    int n = memberFunctionConstraintNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::ParameterNode* parameter = memberFunctionConstraintNode.Parameters()[i];
        parameter->Accept(*this);
    }
    Other(");");
}

void SourceCodePrinter::Visit(cmajor::ast::FunctionConstraintNode& functionConstraintNode)
{
    MoveTo(functionConstraintNode.GetSpan());
    functionConstraintNode.ReturnTypeExpr()->Accept(*this);
    WriteSpace(1);
    WriteFunctionGroupId(util::ToUtf8(functionConstraintNode.GroupId()));
    Other("(");
    int n = functionConstraintNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::ParameterNode* parameter = functionConstraintNode.Parameters()[i];
        parameter->Accept(*this);
    }
    Other(");");
}

void SourceCodePrinter::Visit(cmajor::ast::AxiomStatementNode& axiomStatementNode)
{
    MoveTo(axiomStatementNode.GetSpan());
    axiomStatementNode.Expression()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::AxiomNode& axiomNode)
{
    MoveTo(axiomNode.GetSpan());
    Keyword("axiom");
    if (axiomNode.Id())
    {
        axiomNode.Id()->Accept(*this);
    }
    int n = axiomNode.Parameters().Count();
    if (n > 0)
    {
        Other("(");
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                Other(", ");
            }
            cmajor::ast::ParameterNode* parameter = axiomNode.Parameters()[i];
            parameter->Accept(*this);
        }
        Other(")");
    }
    MoveTo(axiomNode.BeginBraceSpan());
    Other("{");
    int ns = axiomNode.Statements().Count();
    for (int i = 0; i < ns; ++i)
    {
        cmajor::ast::AxiomStatementNode* axiomStatement = axiomNode.Statements()[i];
        axiomStatement->Accept(*this);
    }
    MoveTo(axiomNode.EndBraceSpan());
    Other("}");
}

void SourceCodePrinter::Visit(cmajor::ast::ConceptIdNode& conceptIdNode)
{
    MoveTo(conceptIdNode.GetSpan());
    conceptIdNode.Id()->Accept(*this);
    Other("<");
    int n = conceptIdNode.TypeParameters().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        conceptIdNode.TypeParameters()[i]->Accept(*this);
    }
    Other(">");
}

void SourceCodePrinter::Visit(cmajor::ast::ConceptNode& conceptNode)
{
    MoveTo(conceptNode.GetSpan());
    WriteSpecifiers(conceptNode.GetSpan(), conceptNode.GetSpecifiers());
    if (conceptNode.GetSpecifiers() != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
    }
    Keyword("concept");
    conceptNode.Id()->Accept(*this);
    Other("<");
    int nt = conceptNode.TypeParameters().Count();
    for (int i = 0; i < nt; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        conceptNode.TypeParameters()[i]->Accept(*this);
    }
    Other(">");
    if (conceptNode.Refinement())
    {
        Other(" : ");
        conceptNode.Refinement()->Accept(*this);
    }
    int nc = conceptNode.Constraints().Count();
    for (int i = 0; i < nc; ++i)
    {
        if (conceptNode.Constraints()[i]->IsHeaderConstraint())
        {
            conceptNode.Constraints()[i]->Accept(*this);
        }
    }
    MoveTo(conceptNode.BeginBraceSpan());
    Other("{");
    for (int i = 0; i < nc; ++i)
    {
        if (!conceptNode.Constraints()[i]->IsHeaderConstraint())
        {
            conceptNode.Constraints()[i]->Accept(*this);
        }
    }
    int na = conceptNode.Axioms().Count();
    for (int i = 0; i < na; ++i)
    {
        conceptNode.Axioms()[i]->Accept(*this);
    }
    MoveTo(conceptNode.EndBraceSpan());
    Other("}");
}

void SourceCodePrinter::Visit(cmajor::ast::SameConstraintNode& sameConstraintNode)
{
    // todo
}

void SourceCodePrinter::Visit(cmajor::ast::DerivedConstraintNode& derivedConstraintNode)
{
    // todo
}

void SourceCodePrinter::Visit(cmajor::ast::ConvertibleConstraintNode& convertibleConstraintNode)
{
    // todo
}

void SourceCodePrinter::Visit(cmajor::ast::ExplicitlyConvertibleConstraintNode& explicitlyConvertibleConstraintNode)
{
    // todo
}

void SourceCodePrinter::Visit(cmajor::ast::CommonConstraintNode& commonConstraintNode)
{
    // todo
}

void SourceCodePrinter::Visit(cmajor::ast::NonreferenceTypeConstraintNode& nonreferenceTypeConstraintNode)
{
    // todo
}

void SourceCodePrinter::Visit(cmajor::ast::LabelNode& labelNode)
{
    MoveTo(labelNode.GetSpan());
    Identifier(util::ToUtf8(labelNode.Label()));
    Other(": ");
}

void SourceCodePrinter::Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode)
{
    labeledStatementNode.Label()->Accept(*this);
    labeledStatementNode.Stmt()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode)
{
    MoveTo(compoundStatementNode.GetSpan());
    Other("{");
    int n = compoundStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statement = compoundStatementNode.Statements()[i];
        statement->Accept(*this);
    }
    MoveTo(compoundStatementNode.EndSpan());
    Other("}");
}

void SourceCodePrinter::Visit(cmajor::ast::ReturnStatementNode& returnStatementNode)
{
    MoveTo(returnStatementNode.GetSpan());
    Keyword("return");
    if (returnStatementNode.Expression())
    {
        returnStatementNode.Expression()->Accept(*this);
    }
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::IfStatementNode& ifStatementNode)
{
    MoveTo(ifStatementNode.GetSpan());
    Keyword("if");
    MoveTo(ifStatementNode.LParenSpan());

    Other("(");
    ifStatementNode.Condition()->Accept(*this);
    MoveTo(ifStatementNode.RParenSpan());
    Other(")");
    ifStatementNode.ThenS()->Accept(*this);
    if (ifStatementNode.ElseS())
    {
        MoveTo(ifStatementNode.ElseSpan());
        Keyword("else");
        ifStatementNode.ElseS()->Accept(*this);
    }
}

void SourceCodePrinter::Visit(cmajor::ast::WhileStatementNode& whileStatementNode)
{
    MoveTo(whileStatementNode.GetSpan());
    Keyword("while");
    MoveTo(whileStatementNode.LParenSpan());
    Other("(");
    whileStatementNode.Condition()->Accept(*this);
    MoveTo(whileStatementNode.RParenSpan());
    Other(")");
    whileStatementNode.Statement()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::DoStatementNode& doStatementNode)
{
    MoveTo(doStatementNode.GetSpan());
    Keyword("do");
    doStatementNode.Statement()->Accept(*this);
    MoveTo(doStatementNode.WhileSpan());
    Keyword("while");
    MoveTo(doStatementNode.LParenSpan());
    Other("(");
    doStatementNode.Condition()->Accept(*this);
    MoveTo(doStatementNode.RParenSpan());
    Other(");");
}

void SourceCodePrinter::Visit(cmajor::ast::ForStatementNode& forStatementNode)
{
    MoveTo(forStatementNode.GetSpan());
    Keyword("for");
    MoveTo(forStatementNode.LParenSpan());
    Other("(");
    forStatementNode.InitS()->Accept(*this);
    if (forStatementNode.Condition())
    {
        forStatementNode.Condition()->Accept(*this);
    }
    Other(";");
    forStatementNode.LoopS()->Accept(*this);
    MoveTo(forStatementNode.RParenSpan());
    Other(")");
    forStatementNode.ActionS()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::BreakStatementNode& breakStatementNode)
{
    MoveTo(breakStatementNode.GetSpan());
    Keyword("break");
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::ContinueStatementNode& continueStatementNode)
{
    MoveTo(continueStatementNode.GetSpan());
    Keyword("continue");
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::GotoStatementNode& gotoStatementNode)
{
    MoveTo(gotoStatementNode.GetSpan());
    Keyword("goto");
    WriteSpace(1);
    Identifier(util::ToUtf8(gotoStatementNode.Target()));
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode)
{
    MoveTo(constructionStatementNode.GetSpan());
    constructionStatementNode.TypeExpr()->Accept(*this);
    constructionStatementNode.Id()->Accept(*this);
    if (constructionStatementNode.Assignment())
    {
        Other(" = ");
        constructionStatementNode.Arguments()[0]->Accept(*this);
    }
    else if (!constructionStatementNode.Empty())
    {
        Other("(");
        int n = constructionStatementNode.Arguments().Count();
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                Other(", ");
            }
            constructionStatementNode.Arguments()[i]->Accept(*this);
        }
        Other(")");
    }
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode)
{
    MoveTo(deleteStatementNode.GetSpan());
    Keyword("delete");
    deleteStatementNode.Expression()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode)
{
    MoveTo(destroyStatementNode.GetSpan());
    Keyword("destroy");
    destroyStatementNode.Expression()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode)
{
    MoveTo(assignmentStatementNode.GetSpan());
    assignmentStatementNode.TargetExpr()->Accept(*this);
    Other(" = ");
    assignmentStatementNode.SourceExpr()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode)
{
    MoveTo(expressionStatementNode.GetSpan());
    expressionStatementNode.Expression()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode)
{
    MoveTo(emptyStatementNode.GetSpan());
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode)
{
    MoveTo(rangeForStatementNode.GetSpan());
    Keyword("for");
    MoveTo(rangeForStatementNode.LParenSpan());
    Other("(");
    rangeForStatementNode.TypeExpr()->Accept(*this);
    rangeForStatementNode.Id()->Accept(*this);
    MoveTo(rangeForStatementNode.ColonSpan());
    Other(":");
    rangeForStatementNode.Container()->Accept(*this);
    MoveTo(rangeForStatementNode.RParenSpan());
    Other(")");
    rangeForStatementNode.Action()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::SwitchStatementNode& switchStatementNode)
{
    MoveTo(switchStatementNode.GetSpan());
    Keyword("switch");
    MoveTo(switchStatementNode.LParenSpan());
    Other("(");
    switchStatementNode.Condition()->Accept(*this);
    MoveTo(switchStatementNode.RParenSpan());
    Other(")");
    MoveTo(switchStatementNode.BeginBraceSpan());
    Other("{");
    int nc = switchStatementNode.Cases().Count();
    for (int i = 0; i < nc; ++i)
    {
        cmajor::ast::CaseStatementNode* caseStatement = switchStatementNode.Cases()[i];
        caseStatement->Accept(*this);
    }
    if (switchStatementNode.Default())
    {
        switchStatementNode.Default()->Accept(*this);
    }
    MoveTo(switchStatementNode.EndBraceSpan());
    Other("}");
}

void SourceCodePrinter::Visit(cmajor::ast::CaseStatementNode& caseStatementNode)
{
    MoveTo(caseStatementNode.GetSpan());
    int n = caseStatementNode.CaseExprs().Count();
    for (int i = 0; i < n; ++i)
    {
        MoveTo(caseStatementNode.CaseSpans()[i]);
        Keyword("case");
        caseStatementNode.CaseExprs()[i]->Accept(*this);
        Other(":");
    }
    int ns = caseStatementNode.Statements().Count();
    for (int i = 0; i < ns; ++i)
    {
        caseStatementNode.Statements()[i]->Accept(*this);
    }
}

void SourceCodePrinter::Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode)
{
    MoveTo(defaultStatementNode.GetSpan());
    Keyword("default");
    Other(":");
    int ns = defaultStatementNode.Statements().Count();
    for (int i = 0; i < ns; ++i)
    {
        defaultStatementNode.Statements()[i]->Accept(*this);
    }
}

void SourceCodePrinter::Visit(cmajor::ast::GotoCaseStatementNode& gotoCaseStatementNode)
{
    MoveTo(gotoCaseStatementNode.GetSpan());
    Keyword("goto");
    WriteSpace(1);
    Keyword("case");
    gotoCaseStatementNode.CaseExpr()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::GotoDefaultStatementNode& gotoDefaultStatementNode)
{
    MoveTo(gotoDefaultStatementNode.GetSpan());
    Keyword("goto");
    WriteSpace(1);
    Keyword("default");
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::ThrowStatementNode& throwStatementNode)
{
    MoveTo(throwStatementNode.GetSpan());
    Keyword("throw");
    if (throwStatementNode.Expression())
    {
        throwStatementNode.Expression()->Accept(*this);
    }
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::TryStatementNode& tryStatementNode)
{
    MoveTo(tryStatementNode.GetSpan());
    Keyword("try");
    tryStatementNode.TryBlock()->Accept(*this);
    int n = tryStatementNode.Catches().Count();
    for (int i = 0; i < n; ++i)
    {
        tryStatementNode.Catches()[i]->Accept(*this);
    }
}

void SourceCodePrinter::Visit(cmajor::ast::CatchNode& catchNode)
{
    MoveTo(catchNode.GetSpan());
    Keyword("catch");
    MoveTo(catchNode.LParenSpan());
    Other("(");
    catchNode.TypeExpr()->Accept(*this);
    if (catchNode.Id())
    {
        catchNode.Id()->Accept(*this);
    }
    MoveTo(catchNode.RParenSpan());
    Other(")");
    catchNode.CatchBlock()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::AssertStatementNode& assertStatementNode)
{
    MoveTo(assertStatementNode.GetSpan());
    Keyword("#assert");
    assertStatementNode.AssertExpr()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode)
{
    MoveTo(conditionalCompilationDisjunctionNode.GetSpan());
    conditionalCompilationDisjunctionNode.Left()->Accept(*this);
    Other(" || ");
    conditionalCompilationDisjunctionNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode)
{
    MoveTo(conditionalCompilationConjunctionNode.GetSpan());
    conditionalCompilationConjunctionNode.Left()->Accept(*this);
    Other(" && ");
    conditionalCompilationConjunctionNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode)
{
    MoveTo(conditionalCompilationNotNode.GetSpan());
    Other("!");
    conditionalCompilationNotNode.Expr()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode)
{
    MoveTo(conditionalCompilationPrimaryNode.GetSpan());
    Identifier(util::ToUtf8(conditionalCompilationPrimaryNode.Symbol()));
}

void SourceCodePrinter::Visit(cmajor::ast::ParenthesizedConditionalCompilationExpressionNode& parenthesizedConditionalCompilationExpressionNode)
{
    MoveTo(parenthesizedConditionalCompilationExpressionNode.GetSpan());
    Other("(");
    parenthesizedConditionalCompilationExpressionNode.Expr()->Accept(*this);
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode)
{
    MoveTo(conditionalCompilationStatementNode.IfPart()->KeywordSpan());
    Keyword("#if");
    MoveTo(conditionalCompilationStatementNode.IfPart()->LParenSpan());
    Other("(");
    conditionalCompilationStatementNode.IfPart()->Expr()->Accept(*this);
    MoveTo(conditionalCompilationStatementNode.IfPart()->RParenSpan());
    Other(")");
    int ni = conditionalCompilationStatementNode.IfPart()->Statements().Count();
    for (int i = 0; i < ni; ++i)
    {
        conditionalCompilationStatementNode.IfPart()->Statements()[i]->Accept(*this);
    }
    int nep = conditionalCompilationStatementNode.ElifParts().Count();
    for (int i = 0; i < nep; ++i)
    {
        cmajor::ast::ConditionalCompilationPartNode* elifPart = conditionalCompilationStatementNode.ElifParts()[i];
        MoveTo(elifPart->KeywordSpan());
        Keyword("#elif");
        MoveTo(elifPart->LParenSpan());
        Other("(");
        elifPart->Expr()->Accept(*this);
        MoveTo(elifPart->RParenSpan());
        Other(")");
        int ns = elifPart->Statements().Count();
        for (int i = 0; i < ns; ++i)
        {
            elifPart->Statements()[i]->Accept(*this);
        }
    }
    if (conditionalCompilationStatementNode.ElsePart())
    {
        MoveTo(conditionalCompilationStatementNode.ElsePart()->KeywordSpan());
        Keyword("#else");
        int ns = conditionalCompilationStatementNode.ElsePart()->Statements().Count();
        for (int i = 0; i < ns; ++i)
        {
            conditionalCompilationStatementNode.ElsePart()->Statements()[i]->Accept(*this);
        }
    }
    MoveTo(conditionalCompilationStatementNode.EndIfSpan());
    Keyword("#endif");
}

void SourceCodePrinter::Visit(cmajor::ast::TypedefNode& typedefNode)
{
    MoveTo(typedefNode.GetSpan());
    WriteSpecifiers(typedefNode.GetSpan(), typedefNode.GetSpecifiers());
    if (typedefNode.GetSpecifiers() != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
    }
    Keyword("typedef");
    typedefNode.TypeExpr()->Accept(*this);
    typedefNode.Id()->Accept(*this);
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::ConstantNode& constantNode)
{
    MoveTo(constantNode.GetSpan());
    WriteSpecifiers(constantNode.GetSpan(), constantNode.GetSpecifiers());
    if (constantNode.GetSpecifiers() != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
    }
    Keyword("const");
    constantNode.TypeExpr()->Accept(*this);
    constantNode.Id()->Accept(*this);
    if (constantNode.Value())
    {
        Other(" = ");
        constantNode.Value()->Accept(*this);
    }
    Other(";");
}

void SourceCodePrinter::Visit(cmajor::ast::EnumTypeNode& enumTypeNode)
{
    MoveTo(enumTypeNode.GetSpan());
    WriteSpecifiers(enumTypeNode.GetSpan(), enumTypeNode.GetSpecifiers());
    if (enumTypeNode.GetSpecifiers() != cmajor::ast::Specifiers::none)
    {
        WriteSpace(1);
    }
    Keyword("enum");
    enumTypeNode.Id()->Accept(*this);
    if (enumTypeNode.GetUnderlyingType())
    {
        Other(" : ");
        enumTypeNode.GetUnderlyingType()->Accept(*this);
    }
    MoveTo(enumTypeNode.BeginBraceSpan());
    Other("{");
    int n = enumTypeNode.Constants().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        cmajor::ast::EnumConstantNode* constant = enumTypeNode.Constants()[i];
        constant->Accept(*this);
    }
    MoveTo(enumTypeNode.EndBraceSpan());
    Other("}");
}

void SourceCodePrinter::Visit(cmajor::ast::EnumConstantNode& enumConstantNode)
{
    MoveTo(enumConstantNode.GetSpan());
    enumConstantNode.Id()->Accept(*this);
    if (enumConstantNode.HasValue())
    {
        Other(" = ");
        enumConstantNode.GetValue()->Accept(*this);
    }
}

void SourceCodePrinter::Visit(cmajor::ast::ParameterNode& parameterNode)
{
    MoveTo(parameterNode.GetSpan());
    parameterNode.TypeExpr()->Accept(*this);
    if (parameterNode.Id() && !parameterNode.ArtificialId())
    {
        parameterNode.Id()->Accept(*this);
    }
}

void SourceCodePrinter::Visit(cmajor::ast::TemplateParameterNode& templateParameterNode)
{
    MoveTo(templateParameterNode.GetSpan());
    templateParameterNode.Id()->Accept(*this);
    if (templateParameterNode.DefaultTemplateArgument())
    {
        Other(" = ");
        templateParameterNode.DefaultTemplateArgument()->Accept(*this);
    }
}

void SourceCodePrinter::Visit(cmajor::ast::ConstNode& constNode)
{
    MoveTo(constNode.GetSpan());
    Keyword("const");
    WriteSpace(1);
    constNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::LValueRefNode& lvalueRefNode)
{
    MoveTo(lvalueRefNode.GetSpan());
    lvalueRefNode.Subject()->Accept(*this);
    Other("&");
}

void SourceCodePrinter::Visit(cmajor::ast::RValueRefNode& rvalueRefNode)
{
    MoveTo(rvalueRefNode.GetSpan());
    rvalueRefNode.Subject()->Accept(*this);
    Other("&&");
}

void SourceCodePrinter::Visit(cmajor::ast::PointerNode& pointerNode)
{
    MoveTo(pointerNode.GetSpan());
    pointerNode.Subject()->Accept(*this);
    Other("*");
}

void SourceCodePrinter::Visit(cmajor::ast::ArrayNode& arrayNode)
{
    MoveTo(arrayNode.GetSpan());
    arrayNode.Subject()->Accept(*this);
    Other("[");
    if (arrayNode.Size())
    {
        arrayNode.Size()->Accept(*this);
    }
    Other("]");
}

void SourceCodePrinter::Visit(cmajor::ast::DotNode& dotNode)
{
    MoveTo(dotNode.GetSpan());
    dotNode.Subject()->Accept(*this);
    Other(".");
    dotNode.MemberId()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ArrowNode& arrowNode)
{
    MoveTo(arrowNode.GetSpan());
    arrowNode.Subject()->Accept(*this);
    Other("->");
    arrowNode.MemberId()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::EquivalenceNode& equivalenceNode)
{
    MoveTo(equivalenceNode.GetSpan());
    equivalenceNode.Left()->Accept(*this);
    Other(" <=> ");
    equivalenceNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ImplicationNode& implicationNode)
{
    MoveTo(implicationNode.GetSpan());
    implicationNode.Left()->Accept(*this);
    Other(" => ");
    implicationNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::DisjunctionNode& disjunctionNode)
{
    MoveTo(disjunctionNode.GetSpan());
    disjunctionNode.Left()->Accept(*this);
    Other(" || ");
    disjunctionNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ConjunctionNode& conjunctionNode)
{
    MoveTo(conjunctionNode.GetSpan());
    conjunctionNode.Left()->Accept(*this);
    Other(" && ");
    conjunctionNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::BitOrNode& bitOrNode)
{
    MoveTo(bitOrNode.GetSpan());
    bitOrNode.Left()->Accept(*this);
    Other(" | ");
    bitOrNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::BitXorNode& bitXorNode)
{
    MoveTo(bitXorNode.GetSpan());
    bitXorNode.Left()->Accept(*this);
    Other(" ^ ");
    bitXorNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::BitAndNode& bitAndNode)
{
    MoveTo(bitAndNode.GetSpan());
    bitAndNode.Left()->Accept(*this);
    Other(" & ");
    bitAndNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::EqualNode& equalNode)
{
    MoveTo(equalNode.GetSpan());
    equalNode.Left()->Accept(*this);
    Other(" == ");
    equalNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::NotEqualNode& notEqualNode)
{
    MoveTo(notEqualNode.GetSpan());
    notEqualNode.Left()->Accept(*this);
    Other(" != ");
    notEqualNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::LessNode& lessNode)
{
    MoveTo(lessNode.GetSpan());
    lessNode.Left()->Accept(*this);
    Other(" < ");
    lessNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::GreaterNode& greaterNode)
{
    MoveTo(greaterNode.GetSpan());
    greaterNode.Left()->Accept(*this);
    Other(" > ");
    greaterNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::LessOrEqualNode& lessOrEqualNode)
{
    MoveTo(lessOrEqualNode.GetSpan());
    lessOrEqualNode.Left()->Accept(*this);
    Other(" <= ");
    lessOrEqualNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::GreaterOrEqualNode& greaterOrEqualNode)
{
    MoveTo(greaterOrEqualNode.GetSpan());
    greaterOrEqualNode.Left()->Accept(*this);
    Other(" >= ");
    greaterOrEqualNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ShiftLeftNode& shiftLeftNode)
{
    MoveTo(shiftLeftNode.GetSpan());
    shiftLeftNode.Left()->Accept(*this);
    Other(" << ");
    shiftLeftNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ShiftRightNode& shiftRightNode)
{
    MoveTo(shiftRightNode.GetSpan());
    shiftRightNode.Left()->Accept(*this);
    Other(" >> ");
    shiftRightNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::AddNode& addNode)
{
    MoveTo(addNode.GetSpan());
    addNode.Left()->Accept(*this);
    Other(" + ");
    addNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::SubNode& subNode)
{
    MoveTo(subNode.GetSpan());
    subNode.Left()->Accept(*this);
    Other(" - ");
    subNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::MulNode& mulNode)
{
    MoveTo(mulNode.GetSpan());
    mulNode.Left()->Accept(*this);
    Other(" * ");
    mulNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::DivNode& divNode)
{
    MoveTo(divNode.GetSpan());
    divNode.Left()->Accept(*this);
    Other(" / ");
    divNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::RemNode& remNode)
{
    MoveTo(remNode.GetSpan());
    remNode.Left()->Accept(*this);
    Other(" % ");
    remNode.Right()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::NotNode& notNode)
{
    MoveTo(notNode.GetSpan());
    Other("!");
    notNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::UnaryPlusNode& unaryPlusNode)
{
    MoveTo(unaryPlusNode.GetSpan());
    Other("+");
    unaryPlusNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::UnaryMinusNode& unaryMinusNode)
{
    MoveTo(unaryMinusNode.GetSpan());
    Other("-");
    unaryMinusNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::PrefixIncrementNode& prefixIncrementNode)
{
    MoveTo(prefixIncrementNode.GetSpan());
    Other("++");
    prefixIncrementNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::PrefixDecrementNode& prefixDecrementNode)
{
    MoveTo(prefixDecrementNode.GetSpan());
    Other("--");
    prefixDecrementNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::DerefNode& derefNode)
{
    MoveTo(derefNode.GetSpan());
    Other("*");
    derefNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::AddrOfNode& addrOfNode)
{
    MoveTo(addrOfNode.GetSpan());
    Other("&");
    addrOfNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::ComplementNode& complementNode)
{
    MoveTo(complementNode.GetSpan());
    Other("~");
    complementNode.Subject()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::IsNode& isNode)
{
    MoveTo(isNode.GetSpan());
    isNode.Expr()->Accept(*this);
    Keyword(" is ");
    isNode.TargetTypeExpr()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::AsNode& asNode)
{
    MoveTo(asNode.GetSpan());
    asNode.Expr()->Accept(*this);
    Keyword(" as ");
    asNode.TargetTypeExpr()->Accept(*this);
}

void SourceCodePrinter::Visit(cmajor::ast::IndexingNode& indexingNode)
{
    MoveTo(indexingNode.GetSpan());
    indexingNode.Subject()->Accept(*this);
    Other("[");
    indexingNode.Index()->Accept(*this);
    Other("]");
}

void SourceCodePrinter::Visit(cmajor::ast::InvokeNode& invokeNode)
{
    MoveTo(invokeNode.GetSpan());
    invokeNode.Subject()->Accept(*this);
    Other("(");
    int n = invokeNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        invokeNode.Arguments()[i]->Accept(*this);
    }
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::PostfixIncrementNode& postfixIncrementNode)
{
    MoveTo(postfixIncrementNode.GetSpan());
    postfixIncrementNode.Subject()->Accept(*this);
    Other("++");
}

void SourceCodePrinter::Visit(cmajor::ast::PostfixDecrementNode& postfixDecrementNode)
{
    MoveTo(postfixDecrementNode.GetSpan());
    postfixDecrementNode.Subject()->Accept(*this);
    Other("--");
}

void SourceCodePrinter::Visit(cmajor::ast::SizeOfNode& sizeOfNode)
{
    MoveTo(sizeOfNode.GetSpan());
    Keyword("sizeof");
    Other("(");
    sizeOfNode.Expression()->Accept(*this);
    Other(")");

}

void SourceCodePrinter::Visit(cmajor::ast::TypeNameNode& typeNameNode)
{
    MoveTo(typeNameNode.GetSpan());
    Keyword("typename");
    Other("(");
    typeNameNode.Expression()->Accept(*this);
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::CastNode& castNode)
{
    MoveTo(castNode.GetSpan());
    Keyword("cast");
    Other("<");
    castNode.TargetTypeExpr()->Accept(*this);
    Other(">");
    Other("(");
    castNode.SourceExpr()->Accept(*this);
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::ConstructNode& constructNode)
{
    MoveTo(constructNode.GetSpan());
    Keyword("construct");
    Other("<");
    constructNode.TypeExpr()->Accept(*this);
    Other(">");
    Other("(");
    int n = constructNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        constructNode.Arguments()[i]->Accept(*this);
    }
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::NewNode& newNode)
{
    MoveTo(newNode.GetSpan());
    Keyword("new");
    WriteSpace(1);
    newNode.TypeExpr()->Accept(*this);
    Other("(");
    int n = newNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            Other(", ");
        }
        newNode.Arguments()[i]->Accept(*this);
    }
    Other(")");
}

void SourceCodePrinter::Visit(cmajor::ast::ThisNode& thisNode)
{
    MoveTo(thisNode.GetSpan());
    Keyword("this");
}

void SourceCodePrinter::Visit(cmajor::ast::BaseNode& baseNode)
{
    MoveTo(baseNode.GetSpan());
    Keyword("base");
}

void SourceCodePrinter::Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode)
{
    MoveTo(parenthesizedExpressionNode.GetSpan());
    Other("(");
    parenthesizedExpressionNode.Subject()->Accept(*this);
    Other(")");
}

void GenerateSourceCode(cmajor::ast::Project* project, cmajor::binder::BoundCompileUnit* boundCompileUnit, std::map<int, File>& fileMap)
{
    std::string fileName = util::Path::GetFileName(boundCompileUnit->GetCompileUnitNode()->FilePath());
    Input* input = GetInputPtr();
    std::string targetDir = input->targetDirPath;
    std::filesystem::create_directories(targetDir);
    std::string contentDir = util::GetFullPath(util::Path::Combine(targetDir, "content"));
    std::filesystem::create_directories(contentDir);
    std::string projectDir = util::GetFullPath(util::Path::Combine(contentDir, util::ToUtf8(project->Name())));
    std::filesystem::create_directories(projectDir);
    std::string fileDir = util::GetFullPath(util::Path::Combine(projectDir, "file"));
    std::filesystem::create_directories(fileDir);
    std::string htmlFileName = util::Path::ChangeExtension(fileName, ".cm.html");
    std::string htmlFilePath = util::Path::Combine(fileDir, htmlFileName);
    std::string title = util::ToUtf8(project->Name());
    title.append(1, '/').append(fileName);
    File file;
    file.name = title;
    file.htmlFilePath = util::Path::Combine(util::Path::Combine(util::Path::Combine("../..", util::ToUtf8(project->Name())), "file"), htmlFileName);
    int fileIndex = boundCompileUnit->GetCompileUnitNode()->FileIndex();
    fileMap[fileIndex] = file;
    std::vector<std::string> lines;
    std::ifstream inputFile(boundCompileUnit->GetCompileUnitNode()->FilePath());
    std::string line;
    while (std::getline(inputFile, line))
    {
        lines.push_back(line);
    }
    std::string styleFilePath = "../../../style/code.css";
    SourceCodePrinter printer(htmlFilePath, title, styleFilePath, lines, boundCompileUnit->GetSymbolTable(), boundCompileUnit->GetModule(), fileIndex);
    boundCompileUnit->GetCompileUnitNode()->Accept(printer);
    printer.WriteDocument();
}

} // namespace cmdoclib
