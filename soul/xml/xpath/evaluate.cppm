// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.xpath.evaluate;

import std.core;
import soul.xml.xpath.object;
import soul.xml.xpath.expr;
import soul.xml.node;

export namespace soul::xml::xpath {

std::unique_ptr<soul::xml::xpath::expr::Expr> ParseXPathExpr(const std::string& xpathExpr);
std::unique_ptr<soul::xml::xpath::Object> Evaluate(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node);
std::unique_ptr<soul::xml::xpath::Object> Evaluate(const std::string& xpathExpr, soul::xml::Node* node);
std::unique_ptr<soul::xml::xpath::NodeSet> EvaluateToNodeSet(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node);
std::unique_ptr<soul::xml::xpath::NodeSet> EvaluateToNodeSet(const std::string& xpathExpr, soul::xml::Node* node);
std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node);
std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(const std::string& xpathExpr, soul::xml::Node* node);
std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(const std::string& xpathExpr, const std::string& xmlFileName);

} // namespace soul::xml::xpath
