// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.logview;

import wing.text.view;
import std.core;

export namespace wing {

class LogView : public TextView
{
public:
    LogView(TextViewCreateParams& createParams);
    virtual void WriteLine(const std::string& text);
};

} // wing
