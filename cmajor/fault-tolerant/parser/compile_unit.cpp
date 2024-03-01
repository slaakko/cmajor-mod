// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.compile.unit;

import cmajor.fault.tolerant.parser.namespaces;
import cmajor.fault.tolerant.parser.alias;
import cmajor.fault.tolerant.parser.concepts;
import cmajor.fault.tolerant.parser.function;
import cmajor.fault.tolerant.parser.classes;
import cmajor.fault.tolerant.parser.interface;
import cmajor.fault.tolerant.parser.enums;
import cmajor.fault.tolerant.parser.constant;
import cmajor.fault.tolerant.parser.delegate;
import cmajor.fault.tolerant.parser.global.variable;
import cmajor.fault.tolerant.parser.templates;
import cmajor.fault.tolerant.ast.alias;
import util;

namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseDefinition(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match namespaceDefinitionMatch = ParseNamespaceDefinition(context, lexer);
    if (namespaceDefinitionMatch.hit)
    {
        return namespaceDefinitionMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match usingAliasMatch = ParseUsingAlias(context, lexer);
    if (usingAliasMatch.hit)
    {
        return usingAliasMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match typedefMatch = ParseTypedef(context, lexer);
    if (typedefMatch.hit)
    {
        return typedefMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match conceptMatch = ParseConcept(context, lexer);
    if (conceptMatch.hit)
    {
        return conceptMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match functionMatch = ParseFunction(context, lexer);
    if (functionMatch.hit)
    {
        return functionMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match classMatch = ParseClass(context, lexer);
    if (classMatch.hit)
    {
        return classMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match interfaceMatch = ParseInterface(context, lexer);
    if (interfaceMatch.hit)
    {
        return interfaceMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match enumTypeMatch = ParseEnumType(context, lexer);
    if (enumTypeMatch.hit)
    {
        return enumTypeMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match constantMatch = ParseConstant(context, lexer);
    if (constantMatch.hit)
    {
        return constantMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match delegateMatch = ParseDelegate(context, lexer);
    if (delegateMatch.hit)
    {
        return delegateMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match classDelegateMatch = ParseClassDelegate(context, lexer);
    if (classDelegateMatch.hit)
    {
        return classDelegateMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match globalVariableMatch = ParseGlobalVariable(context, lexer);
    if (globalVariableMatch.hit)
    {
        return globalVariableMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match instantiationRequestMatch = ParseInstantiationRequest(context, lexer);
    if (instantiationRequestMatch.hit)
    {
        return instantiationRequestMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> ParseCompileUnit(Context& context, Lexer& lexer, const std::string& filePath)
{
    ++lexer;
    std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> compileUnit(new cmajor::fault::tolerant::ast::CompileUnitNode());
    compileUnit->SetFilePath(filePath);
    compileUnit->SetLineStarts(lexer.GetLineStartIndeces());
    context.SetCompileUnit(compileUnit.get());
    ParseNamespaceContent(context, lexer, compileUnit->GlobalNs());
    compileUnit->GlobalNs()->Make();
    compileUnit->Make();
    if (*lexer != soul::lexer::END_TOKEN)
    {
        compileUnit->SetValid(false);
    }
    return compileUnit;
}

std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> ParseCmajorCompileUnit(const std::string& cmFilePath)
{
    std::string sourceFile = util::ReadFile(cmFilePath);
    std::u16string content = util::ToUtf16(sourceFile);
    Lexer lexer = cmajor::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), cmFilePath);
    Context context;
    std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> compileUnit = ParseCompileUnit(context, lexer, cmFilePath);
    return compileUnit;
}

} // namespace cmajor::fault::tolerant::parser
