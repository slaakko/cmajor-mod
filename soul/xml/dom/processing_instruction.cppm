// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.processing.instruction;

import std.core;
import soul.xml.node;

export namespace soul::xml {

class ProcessingInstruction : public Node
{
public:
    ProcessingInstruction(const soul::ast::SourcePos& sourcePos_);
    ProcessingInstruction(const soul::ast::SourcePos& sourcePos_, const std::string& target_, const std::string& data_);
    const std::string& Target() const { return target; }
    const std::string& Data() const { return data; }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    std::string target;
    std::string data;
};

ProcessingInstruction* MakeProcessingInstruction(const std::string& target, const std::string& data);

} // namespace soul::xml
