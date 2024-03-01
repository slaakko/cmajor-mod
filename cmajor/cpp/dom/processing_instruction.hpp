// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_PROCESSING_INSTRUCTION
#define DOM_PROCESSING_INSTRUCTION
#include <dom/node.hpp>

namespace soul::xml {

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

#endif // DOM_PROCESSING_INSTRUCTION
