// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef XPATH_EVALUATE_INCLUDED
#define XPATH_EVALUATE_INCLUDED
#include <xpath/object.hpp>
#include <xpath/expr.hpp>
#include <dom/node.hpp>
#include <util/binary_resource_ptr.hpp>

namespace soul::xml::xpath {

void SetModuleFileName(const std::string& moduleFileName);
void SetResourceFlags(util::ResourceFlags resourceFlags_);
std::unique_ptr<soul::xml::xpath::expr::Expr> ParseXPathExpr(const std::string& xpathExpr);
std::unique_ptr<soul::xml::xpath::Object> Evaluate(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node);
std::unique_ptr<soul::xml::xpath::Object> Evaluate(const std::string& xpathExpr, soul::xml::Node* node);
std::unique_ptr<soul::xml::xpath::NodeSet> EvaluateToNodeSet(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node);
std::unique_ptr<soul::xml::xpath::NodeSet> EvaluateToNodeSet(const std::string& xpathExpr, soul::xml::Node* node);
std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(soul::xml::xpath::expr::Expr* expr, soul::xml::Node* node);
std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(const std::string& xpathExpr, soul::xml::Node* node);
std::unique_ptr<soul::xml::Document> EvaluateToXMLInfoDocument(const std::string& xpathExpr, const std::string& xmlFileName);

} // namespace soul::xml::xpath

#endif // XPATH_EVALUATE_INCLUDED
