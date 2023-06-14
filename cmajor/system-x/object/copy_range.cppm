// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.copy.range;

import std.core;

export namespace cmajor::systemx::object {

class Section;

class CopyRange
{
public:
    CopyRange(Section* fromSection_, Section* toSection_, int64_t startPos_, int64_t length_, uint8_t alignment_);
    Section* FromSection() const { return fromSection; }
    Section* ToSection() const { return toSection; }
    int64_t StartPos() const { return startPos; }
    int64_t Length() const { return length; }
    void SetLength(int64_t length_) { length = length_; }
    int8_t Alignment() const { return alignment; }
private:
    Section* fromSection;
    Section* toSection;
    int64_t startPos;
    int64_t length;
    int8_t alignment;
};

} // namespace cmajor::systemx::object
