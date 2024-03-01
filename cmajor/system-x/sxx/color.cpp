// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.executor.color;

import util;

namespace cmajor::systemx::executor {

std::u32string SetColors(ConsoleColor fgColor, ConsoleColor bgColor)
{
    std::u32string colors;
    switch (fgColor)
    {
    case ConsoleColor::black:
    {
        colors.append(util::ToUtf32(util::fgBlack));
        break;
    }
    case ConsoleColor::darkBlue:
    {
        colors.append(util::ToUtf32(util::fgDarkBlue));
        break;
    }
    case ConsoleColor::darkGreen:
    {
        colors.append(util::ToUtf32(util::fgDarkGreen));
        break;
    }
    case ConsoleColor::darkCyan:
    {
        colors.append(util::ToUtf32(util::fgDarkCyan));
        break;
    }
    case ConsoleColor::darkRed:
    {
        colors.append(util::ToUtf32(util::fgDarkRed));
        break;
    }
    case ConsoleColor::darkYellow:
    {
        colors.append(util::ToUtf32(util::fgDarkYellow));
        break;
    }
    case ConsoleColor::gray:
    {
        colors.append(util::ToUtf32(util::fgGray));
        break;
    }
    case ConsoleColor::darkGray:
    {
        colors.append(util::ToUtf32(util::fgGray));
        break;
    }
    case ConsoleColor::blue:
    {
        colors.append(util::ToUtf32(util::fgBlue));
        break;
    }
    case ConsoleColor::green:
    {
        colors.append(util::ToUtf32(util::fgGreen));
        break;
    }
    case ConsoleColor::cyan:
    {
        colors.append(util::ToUtf32(util::fgCyan));
        break;
    }
    case ConsoleColor::red:
    {
        colors.append(util::ToUtf32(util::fgRed));
        break;
    }
    case ConsoleColor::magenta:
    {
        colors.append(util::ToUtf32(util::fgMagenta));
        break;
    }
    case ConsoleColor::yellow:
    {
        colors.append(util::ToUtf32(util::fgYellow));
        break;
    }
    case ConsoleColor::white:
    {
        colors.append(util::ToUtf32(util::fgWhite));
        break;
    }
    }
    switch (bgColor)
    {
    case ConsoleColor::black:
    {
        colors.append(util::ToUtf32(util::bgBlack));
        break;
    }
    case ConsoleColor::darkBlue:
    {
        colors.append(util::ToUtf32(util::bgDarkBlue));
        break;
    }
    case ConsoleColor::darkGreen:
    {
        colors.append(util::ToUtf32(util::bgDarkGreen));
        break;
    }
    case ConsoleColor::darkCyan:
    {
        colors.append(util::ToUtf32(util::bgDarkCyan));
        break;
    }
    case ConsoleColor::darkRed:
    {
        colors.append(util::ToUtf32(util::bgDarkRed));
        break;
    }
    case ConsoleColor::darkYellow:
    {
        colors.append(util::ToUtf32(util::bgDarkYellow));
        break;
    }
    case ConsoleColor::gray:
    {
        colors.append(util::ToUtf32(util::bgGray));
        break;
    }
    case ConsoleColor::darkGray:
    {
        colors.append(util::ToUtf32(util::bgGray));
        break;
    }
    case ConsoleColor::blue:
    {
        colors.append(util::ToUtf32(util::bgBlue));
        break;
    }
    case ConsoleColor::green:
    {
        colors.append(util::ToUtf32(util::bgGreen));
        break;
    }
    case ConsoleColor::cyan:
    {
        colors.append(util::ToUtf32(util::bgCyan));
        break;
    }
    case ConsoleColor::red:
    {
        colors.append(util::ToUtf32(util::bgRed));
        break;
    }
    case ConsoleColor::magenta:
    {
        colors.append(util::ToUtf32(util::bgMagenta));
        break;
    }
    case ConsoleColor::yellow:
    {
        colors.append(util::ToUtf32(util::bgYellow));
        break;
    }
    case ConsoleColor::white:
    {
        colors.append(util::ToUtf32(util::bgWhite));
        break;
    }
    }
    return colors;
}

std::u32string ResetColors()
{
    return util::ToUtf32(util::reset);
}

} // namespace cmajor::systemx::executor
