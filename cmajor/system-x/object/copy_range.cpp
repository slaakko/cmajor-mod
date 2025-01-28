// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.copy.range;

namespace cmajor::systemx::object {

CopyRange::CopyRange(Section* fromSection_, Section* toSection_, int64_t startPos_, int64_t length_, uint8_t alignment_) :
    fromSection(fromSection_), toSection(toSection_), startPos(startPos_), length(length_), alignment(alignment_)
{
}

} // namespace cmajor::systemx::object
