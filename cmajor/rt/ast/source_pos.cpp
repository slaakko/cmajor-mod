// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <ast/source_pos.hpp>

namespace soul::ast {

    std::string ToString(const SourcePos& sourcePos)
    {
        std::string s("(");
        if (sourcePos.IsValid())
        {
            s.append(std::to_string(sourcePos.line)).append(1, ',').append(std::to_string(sourcePos.col)).append(1, ')');
        }
        else
        {
            s.append(1, ')');
        }
        return s;
    }

} // namespace soul::ast
