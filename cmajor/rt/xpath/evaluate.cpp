// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <xpath/evaluate.hpp>
#include <xpath/xpath.lexer.hpp>
#include <xpath/xpath.parser.hpp>
#include <xpath/context.hpp>
#include <xpath/xpath_rules.hpp>
#include <dom/document.hpp>
#include <dom_parser/dom_parser.hpp>
#include <util/system.hpp>

namespace soul::xml::xpath {

std::string moduleFileName;
util::ResourceFlags resourceFlags = util::ResourceFlags::none;

void SetModuleFileName(const std::string& moduleFileName_)
{
    moduleFileName = moduleFileName_;
}

void SetResourceFlags(util::ResourceFlags resourceFlags_)
{
    resourceFlags = resourceFlags_;
}

std::unique_ptr<soul::xml::xpath::expr::Expr> ParseXPathExpr(const std::string& xpathExpr)
{
    if (!moduleFileName.empty())
    {
        std::u32string u32expr = util::ToUtf32(xpathExpr);
        auto lexer = soul::xml::xpath::lexer::MakeLexer(moduleFileName, resourceFlags, u32expr.c_str(), u32expr.c_str() + u32expr.length(), "xpath expression");
        lexer.SetRuleNameMapPtr(cmajor::cpp::xml::xpath::parser::rules::GetRuleNameMapPtr());
        using LexerType = decltype(lexer);
        std::unique_ptr<soul::xml::xpath::expr::Expr> expr = soul::xml::xpath::parser::XPathParser<LexerType>::Parse(lexer);
        expr->SetStr(xpathExpr);
        return expr;
    }
    else
    {
        std::u32string u32expr = util::ToUtf32(xpathExpr);
        auto lexer = soul::xml::xpath::lexer::MakeLexer(u32expr.c_str(), u32expr.c_str() + u32expr.length(), "xpath expression");
        lexer.SetRuleNameMapPtr(cmajor::cpp::xml::xpath::parser::rules::GetRuleNameMapPtr());
        using LexerType = decltype(lexer);
        std::unique_ptr<soul::xml::xpath::expr::Expr> expr = soul::xml::xpath::parser::XPathParser<LexerType>::Parse(lexer);
        expr->SetStr(xpathExpr);
        return expr;
    }
}

std::unique_ptr<soul::xml::xpath::Object> Evaluate(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node)
{
    Context context(node, 1, 1);
    return expr->Evaluate(context);
}

std::unique_ptr<soul::xml::xpath::Object> Evaluate(const std::string& xpathExpr, soul::xml::Node* node)
{
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = ParseXPathExpr(xpathExpr);
    return Evaluate(expr.get(), node);
}

std::unique_ptr<soul::xml::xpath::NodeSet> EvaluateToNodeSet(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node)
{
    std::unique_ptr<soul::xml::xpath::Object> result = Evaluate(expr, node);
    if (result->IsNodeSet())
    {
        return std::unique_ptr<soul::xml::xpath::NodeSet>(static_cast<soul::xml::xpath::NodeSet*>(result.release()));
    }
    else
    {
        throw std::runtime_error("node-set result expected");
    }
}

std::unique_ptr<soul::xml::xpath::NodeSet> EvaluateToNodeSet(const std::string& xpathExpr, soul::xml::Node* node)
{
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = ParseXPathExpr(xpathExpr);
    return EvaluateToNodeSet(expr.get(), node);
}

std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(soul::xml::xpath::expr::Expr* expr, const std::string& parsingError,
    soul::xml::Node* node, const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> infoDoc = soul::xml::MakeDocument();
    soul::xml::Element* xpathElement = soul::xml::MakeElement("xpath");
    infoDoc->AppendChild(xpathElement);
    if (expr)
    {
        soul::xml::Element* exprElement = soul::xml::MakeElement("expr-string");
        exprElement->SetAttribute("value", expr->Str());
        xpathElement->AppendChild(exprElement);
    }
    if (!xmlFileName.empty())
    {
        soul::xml::Element* xmlFileNameElement = soul::xml::MakeElement("xmlFileName");
        xmlFileNameElement->SetAttribute("value", xmlFileName);
        xpathElement->AppendChild(xmlFileNameElement);
    }
    soul::xml::Element* resultElement = soul::xml::MakeElement("result");
    if (expr)
    {
        try
        {
            xpathElement->AppendChild(expr->ToXmlElement());
            std::unique_ptr<soul::xml::xpath::Object> result = Evaluate(expr, node);
            resultElement->AppendChild(result->ToXmlElement());
        }
        catch (const std::exception& ex)
        {
            soul::xml::Element* errorElement = soul::xml::MakeElement("error");
            errorElement->SetAttribute("value", ex.what());
            resultElement->AppendChild(errorElement);
        }
    }
    else if (!parsingError.empty())
    {
        soul::xml::Element* errorElement = soul::xml::MakeElement("error");
        errorElement->SetAttribute("value", parsingError);
        resultElement->AppendChild(errorElement);
    }
    xpathElement->AppendChild(resultElement);
    return infoDoc;
}

std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node, const std::string& xmlFileName)
{
    return EvaluateToXMLInfoDocument(expr, std::string(), node, xmlFileName);
}

std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node)
{
    return EvaluateToXMLInfoDocument(expr, node, std::string());
}

std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(const std::string& xpathExpr, soul::xml::Node* node, const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr;
    std::string parsingError;
    try
    {
        expr = ParseXPathExpr(xpathExpr);
    }
    catch (const std::exception& ex)
    {
        parsingError = ex.what();
    }
    return EvaluateToXMLInfoDocument(expr.get(), parsingError, node, xmlFileName);
}

std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(const std::string& xpathExpr, soul::xml::Node* node)
{
    return EvaluateToXMLInfoDocument(xpathExpr, node, std::string());
}

std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(const std::string& xpathExpr, const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> doc = soul::xml::ParseXmlFile(xmlFileName);
    std::unique_ptr<soul::xml::Document> infoDoc = EvaluateToXMLInfoDocument(xpathExpr, doc.get(), xmlFileName);
    return infoDoc;
}

} // namespace soul::xml::xpath

