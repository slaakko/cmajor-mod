// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.themes;

import cmcode.config;
import cmcode.configuration;
import cmcode.solution.explorer;
import cmcode.tool.bar;
import cmajor.view;
import wing;

namespace cmcode {

void MakeLightTheme()
{
    std::unique_ptr<wing::Theme> lightTheme(new wing::Theme("light", "light-theme.xml"));

    lightTheme->SetColor("window.background", wing::Color(238, 238, 242));
    lightTheme->SetColor("window.title.bar", wing::Color(238, 238, 242));
    lightTheme->SetColor("window.title.text", wing::Color(0, 0, 0));
    lightTheme->SetColor("window.title.disabled.text", wing::Color(67, 67, 67));
    lightTheme->SetColor("window.sys.button", wing::Color(30, 30, 30));
    lightTheme->SetColor("window.sys.button.selected", wing::Color(0, 0, 0));
    lightTheme->SetColor("window.sys.button.hover.background", wing::Color(252, 252, 253));

    lightTheme->SetColor("scroll.bar.background", wing::Color(241, 241, 241));

    lightTheme->SetColor("cmajor.editor.background", wing::Color(255, 255, 255));
    lightTheme->SetColor("resource.editor.background", wing::Color(255, 255, 255));
    lightTheme->SetColor("resource.editor.text", wing::Color::Black);
    lightTheme->SetColor("text.editor.background", wing::Color(255, 255, 255));
    lightTheme->SetColor("text.editor.text", wing::Color::Black);
    lightTheme->SetColor("debug.strip.background", cmajor::view::DefaultDebugStripBackgroundColor());
    lightTheme->SetColor("debug.strip.break.point", cmajor::view::DefaultBreakpointColor());

    lightTheme->SetColor("solution.explorer.background", DefaultSolutionExplorerBackgroundColor());
    lightTheme->SetColor("solution.explorer.text", wing::Color::Black);
    lightTheme->SetColor("solution.explorer.state.indicator", wing::Color::Red);
    lightTheme->SetColor("solution.explorer.node.selected", wing::Color(201, 222, 245));

    lightTheme->SetColor("error.view.background", wing::Color::White);
    lightTheme->SetColor("error.view.text", wing::Color::Black);
    lightTheme->SetColor("error.view.node.selected", wing::Color(201, 222, 245));
    lightTheme->SetColor("error.view.state.indicator", wing::Color::Red);

    lightTheme->SetColor("warnings.view.background", wing::Color::White);
    lightTheme->SetColor("warnings.view.text", wing::Color::Black);
    lightTheme->SetColor("warnings.view.node.selected", wing::Color(201, 222, 245));
    lightTheme->SetColor("warnings.view.state.indicator", wing::Color::Red);

    lightTheme->SetColor("search.results.background", wing::Color::White);
    lightTheme->SetColor("search.results.text", wing::Color::Black);
    lightTheme->SetColor("search.results.node.selected", wing::Color(201, 222, 245));
    lightTheme->SetColor("search.results.state.indicator", wing::Color::Red);

    lightTheme->SetColor("locals.background", wing::Color::White);
    lightTheme->SetColor("locals.text", wing::Color::Black);
    lightTheme->SetColor("locals.node.selected", wing::Color(201, 222, 245));
    lightTheme->SetColor("locals.state.indicator", wing::Color::Red);

    lightTheme->SetColor("call.stack.background", wing::Color::White);
    lightTheme->SetColor("call.stack.text", wing::Color::Black);
    lightTheme->SetColor("call.stack.node.selected", wing::Color(201, 222, 245));
    lightTheme->SetColor("call.stack.state.indicator", wing::Color::Red);

    lightTheme->SetColor("port.map.background", wing::Color::White);
    lightTheme->SetColor("port.map.text", wing::Color::Black);
    lightTheme->SetColor("port.map.node.selected", wing::Color(201, 222, 245));
    lightTheme->SetColor("port.map.state.indicator", wing::Color::Red);

    lightTheme->SetColor("tree.view.active.node.text", wing::Color::Black);

    lightTheme->SetColor("tool.bar.background", wing::DefaultControlBackgroundColor());
    lightTheme->SetColor("tool.bar.frame", DefaultToolBarBorderColor());
    lightTheme->SetColor("tool.bar.tool.tip", DefaultToolBarToolTipColor());
    lightTheme->SetColor("tool.bar.tool.button.dark", wing::DefaultToolButtonDarkColor());
    lightTheme->SetColor("tool.bar.tool.button.light", wing::DefaultToolButtonLightColor());
    lightTheme->SetColor("tool.bar.tool.button.medium.light", wing::DefaultToolButtonMediumLightColor());
    lightTheme->SetColor("tool.bar.tool.button.medium.dark", wing::DefaultToolButtonMediumDarkColor());
    lightTheme->SetColor("tool.bar.tool.button.text", wing::DefaultToolButtonTextColor());
    lightTheme->SetColor("tool.bar.tool.button.disabled", wing::DefaultToolButtonDisabledColor());
    lightTheme->SetColor("tool.bar.tool.button.separator1", wing::DefaultToolButtonSeparatorColor1());
    lightTheme->SetColor("tool.bar.tool.button.separator2", wing::DefaultToolButtonSeparatorColor2());

    lightTheme->SetColor("tab.page", wing::DefaultTabNormalBackgroundColor());

    lightTheme->SetColor("code.tab.control.background", wing::DefaultTabControlBackgroundColor());
    lightTheme->SetColor("code.tab.control.frame", wing::DefaultTabControlFrameColor());
    lightTheme->SetColor("code.tab.control.text", wing::DefaultTabControlTextColor());
    lightTheme->SetColor("code.tab.control.tab.normal.background", wing::DefaultTabNormalBackgroundColor());
    lightTheme->SetColor("code.tab.control.tab.selected.background", wing::DefaultTabSelectedBackgroundColor());
    lightTheme->SetColor("code.tab.control.close.box.selected.background", wing::DefaultTabControlCloseBoxSelectedColor());

    lightTheme->SetColor("output.tab.control.background", wing::DefaultTabControlBackgroundColor());
    lightTheme->SetColor("output.tab.control.frame", wing::DefaultTabControlFrameColor());
    lightTheme->SetColor("output.tab.control.text", wing::DefaultTabControlTextColor());
    lightTheme->SetColor("output.tab.control.tab.normal.background", wing::DefaultTabNormalBackgroundColor());
    lightTheme->SetColor("output.tab.control.tab.selected.background", wing::DefaultTabSelectedBackgroundColor());
    lightTheme->SetColor("output.tab.control.close.box.selected.background", wing::DefaultTabControlCloseBoxSelectedColor());

    lightTheme->SetColor("output.log.background", wing::Color::White);
    lightTheme->SetColor("output.log.text", wing::Color::Black);

    lightTheme->SetColor("log.view.background", wing::Color::White);
    lightTheme->SetColor("log.view.text", wing::Color::Black);

    lightTheme->SetColor("splitContainer.background", wing::DefaultControlBackgroundColor());
    lightTheme->SetColor("splitter.background", wing::DefaultControlBackgroundColor());
    lightTheme->SetColor("splitter.edge", wing::DefaultSplitterEdgeColor());

    lightTheme->SetColor("selection.background", wing::DefaultSelectionBackgroundColor());

    lightTheme->SetColor("plain.text", wing::Color::Black);
    lightTheme->SetColor("space.text", wing::Color::Black);
    lightTheme->SetColor("keyword.text", wing::Color::Blue);
    lightTheme->SetColor("identifier.text", wing::Color::Black);
    lightTheme->SetColor("string.text", wing::Color(163, 21, 21));
    lightTheme->SetColor("character.text", wing::Color(163, 21, 21));
    lightTheme->SetColor("number.text", wing::Color::Black);
    lightTheme->SetColor("comment.text", wing::Color(0, 128, 0));
    lightTheme->SetColor("lineNumber.text", wing::Color(43, 145, 175));
    lightTheme->SetColor("beginBlock.text", wing::Color::Black);
    lightTheme->SetColor("endBlock.text", wing::Color::Black);

    lightTheme->SetColor("debug.location.background", wing::Color(255, 241, 129));
    lightTheme->SetColor("debug.location.text", wing::Color::Black);
    lightTheme->SetColor("text", wing::Color::Black);

    lightTheme->SetColor("menu.background", wing::DefaultMenuBackgroundColor());
    lightTheme->SetColor("menu.text", wing::DefaultMenuTextColor());
    lightTheme->SetColor("menu.disabled.text", wing::DefaultDisabledMenuTextColor());
    lightTheme->SetColor("menu.mouse.over.background", wing::DefaultMenuMouseOverColor());
    lightTheme->SetColor("menu.open.background", wing::DefaultMenuOpenColor());
    lightTheme->SetColor("menu.shadow", wing::DefaultMenuShadowColor());
    lightTheme->SetColor("menu.child.indicator", wing::Color::Black);
    lightTheme->SetColor("menu.frame", wing::Color::Black);
    lightTheme->SetColor("menu.item.separator", wing::Color::DarkGray);

    lightTheme->SetColor("status.bar.background", wing::DefaultControlBackgroundColor());
    lightTheme->SetColor("status.bar.text", wing::DefaultTextColor());
    lightTheme->SetColor("status.bar.top.line", wing::DefaultTopLineColor());
    lightTheme->SetColor("status.bar.sunken.border.outer.top.left", wing::DefaultSunkenBorderOuterTopLeftColor());
    lightTheme->SetColor("status.bar.sunken.border.inner.top.left", wing::DefaultSunkenBorderInnerTopLeftColor());
    lightTheme->SetColor("status.bar.sunken.border.outer.right.bottom", wing::DefaultSunkenBorderOuterRightBottomColor());
    lightTheme->SetColor("status.bar.sunken.border.inner.right.bottom", wing::DefaultSunkenBorderInnerRightBottomColor());
    lightTheme->SetColor("status.bar.raised.border.outer.top.left", wing::DefaultRaisedBorderOuterTopLeftColor());
    lightTheme->SetColor("status.bar.raised.border.outer.right.bottom", wing::DefaultRaisedBorderOuterRightBottomColor());
    lightTheme->SetColor("status.bar.raised.border.inner.top.left", wing::DefaultRaisedBorderInnerTopLeftColor());
    lightTheme->SetColor("status.bar.raised.border.inner.right.bottom", wing::DefaultRaisedBorderInnerRightBottomColor());

    lightTheme->SetBitmapName("prev", "cmcode.prev.light.bitmap");
    lightTheme->SetBitmapName("next", "cmcode.next.light.bitmap");
    lightTheme->SetBitmapName("save", "cmcode.save.light.bitmap");
    lightTheme->SetBitmapName("save.all", "cmcode.save.all.light.bitmap");
    lightTheme->SetBitmapName("build.solution", "cmcode.build.solution.light.bitmap");
    lightTheme->SetBitmapName("build.project", "cmcode.build.project.light.bitmap");
    lightTheme->SetBitmapName("cancel.build", "cmcode.cancel.build.light.bitmap");
    lightTheme->SetBitmapName("debug.start", "cmcode.debug.start.light.bitmap");
    lightTheme->SetBitmapName("debug.stop", "cmcode.debug.stop.light.bitmap");
    lightTheme->SetBitmapName("show.next.statement", "cmcode.debug.show.next.statement.light.bitmap");
    lightTheme->SetBitmapName("step.over", "cmcode.debug.step.over.light.bitmap");
    lightTheme->SetBitmapName("step.into", "cmcode.debug.step.into.light.bitmap");
    lightTheme->SetBitmapName("step.out", "cmcode.debug.step.out.light.bitmap");

    lightTheme->SetBitmapName("file", "file.light.bitmap");
    lightTheme->SetBitmapName("xmlfile", "xml_file.light.bitmap");
    lightTheme->SetBitmapName("project", "cmproject.light.bitmap");
    lightTheme->SetBitmapName("solution", "cmsolution.light.bitmap");

    lightTheme->SetCursorName("horizontal.splitter.cursor", "horizontal.splitter.light.wing.cursor");
    lightTheme->SetCursorName("vertical.splitter.cursor", "vertical.splitter.light.wing.cursor");

    wing::AddTheme(lightTheme.release());
}

void MakeDarkTheme()
{
    std::unique_ptr<wing::Theme> darkTheme(new wing::Theme("dark", "dark-theme.xml"));

    darkTheme->SetColor("window.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("window.title.bar", wing::Color(37, 37, 38));
    darkTheme->SetColor("window.title.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("window.title.disabled.text", wing::Color(67, 67, 67));
    darkTheme->SetColor("window.sys.button", wing::Color(214, 214, 214));
    darkTheme->SetColor("window.sys.button.selected", wing::Color(255, 255, 255));
    darkTheme->SetColor("window.sys.button.hover.background", wing::Color(51, 51, 51));

    darkTheme->SetColor("scroll.bar.background", wing::Color(46, 46, 46));

    darkTheme->SetColor("cmajor.editor.background", wing::Color(30, 30, 30));
    darkTheme->SetColor("resource.editor.background", wing::Color(30, 30, 30));
    darkTheme->SetColor("resource.editor.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("text.editor.background", wing::Color(30, 30, 30));
    darkTheme->SetColor("text.editor.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("debug.strip.background", wing::Color(67, 67, 67));
    darkTheme->SetColor("debug.strip.break.point", cmajor::view::DefaultBreakpointColor());

    darkTheme->SetColor("solution.explorer.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("solution.explorer.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("solution.explorer.state.indicator", wing::Color(122, 193, 255));
    darkTheme->SetColor("solution.explorer.node.selected", wing::Color(38, 79, 120));

    darkTheme->SetColor("error.view.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("error.view.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("error.view.node.selected", wing::Color(38, 79, 120));
    darkTheme->SetColor("error.view.state.indicator", wing::Color(122, 193, 255));

    darkTheme->SetColor("warnings.view.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("warnings.view.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("warnings.view.node.selected", wing::Color(38, 79, 120));
    darkTheme->SetColor("warnings.view.state.indicator", wing::Color(122, 193, 255));

    darkTheme->SetColor("search.results.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("search.results.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("search.results.node.selected", wing::Color(38, 79, 120));
    darkTheme->SetColor("search.results.state.indicator", wing::Color(122, 193, 255));

    darkTheme->SetColor("locals.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("locals.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("locals.node.selected", wing::Color(38, 79, 120));
    darkTheme->SetColor("locals.state.indicator", wing::Color(122, 193, 255));

    darkTheme->SetColor("call.stack.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("call.stack.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("call.stack.node.selected", wing::Color(38, 79, 120));
    darkTheme->SetColor("call.stack.state.indicator", wing::Color(122, 193, 255));

    darkTheme->SetColor("port.map.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("port.map.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("port.map.node.selected", wing::Color(38, 79, 120));
    darkTheme->SetColor("port.map.state.indicator", wing::Color(122, 193, 255));

    darkTheme->SetColor("tree.view.active.node.text", wing::Color::White);

    darkTheme->SetColor("tool.bar.background", wing::Color(45, 45, 48));
    darkTheme->SetColor("tool.bar.frame", wing::Color(63, 63, 70));
    darkTheme->SetColor("tool.bar.tool.tip", DefaultToolBarToolTipColor());
    darkTheme->SetColor("tool.bar.tool.button.dark", wing::Color(25, 25, 25));
    darkTheme->SetColor("tool.bar.tool.button.light", wing::Color(80, 80, 80));
    darkTheme->SetColor("tool.bar.tool.button.medium.light", wing::Color(53, 53, 55));
    darkTheme->SetColor("tool.bar.tool.button.medium.dark", wing::Color(30, 30, 30));
    darkTheme->SetColor("tool.bar.tool.button.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("tool.bar.tool.button.disabled", wing::Color(67, 67, 67));
    darkTheme->SetColor("tool.bar.tool.button.separator1", wing::Color(34, 34, 34));
    darkTheme->SetColor("tool.bar.tool.button.separator2", wing::Color(70, 70, 74));

    darkTheme->SetColor("tab.page", wing::Color(45, 45, 45));

    darkTheme->SetColor("code.tab.control.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("code.tab.control.frame", wing::Color(60, 60, 60));
    darkTheme->SetColor("code.tab.control.text", wing::Color(150, 150, 150));
    darkTheme->SetColor("code.tab.control.tab.normal.background", wing::Color(45, 45, 45));
    darkTheme->SetColor("code.tab.control.tab.selected.background", wing::Color(30, 30, 30));
    darkTheme->SetColor("code.tab.control.close.box.selected.background", wing::Color(49, 50, 50));

    darkTheme->SetColor("output.tab.control.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("output.tab.control.frame", wing::Color(60, 60, 60));
    darkTheme->SetColor("output.tab.control.text", wing::Color(150, 150, 150));
    darkTheme->SetColor("output.tab.control.tab.normal.background", wing::Color(45, 45, 45));
    darkTheme->SetColor("output.tab.control.tab.selected.background", wing::Color(30, 30, 30));
    darkTheme->SetColor("output.tab.control.close.box.selected.background", wing::Color(49, 50, 50));

    darkTheme->SetColor("output.log.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("output.log.text", wing::Color(241, 241, 241));

    darkTheme->SetColor("log.view.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("log.view.text", wing::Color(241, 241, 241));

    darkTheme->SetColor("splitContainer.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("splitter.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("splitter.edge", wing::Color(60, 60, 60));

    darkTheme->SetColor("selection.background", wing::Color(38, 79, 120));

    darkTheme->SetColor("plain.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("space.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("keyword.text", wing::Color(86, 156, 214));
    darkTheme->SetColor("identifier.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("string.text", wing::Color(214, 157, 133));
    darkTheme->SetColor("character.text", wing::Color(214, 157, 133));
    darkTheme->SetColor("number.text", wing::Color(181, 206, 168));
    darkTheme->SetColor("comment.text", wing::Color(87, 166, 74));
    darkTheme->SetColor("lineNumber.text", wing::Color(43, 145, 175));
    darkTheme->SetColor("beginBlock.text", wing::Color(220, 220, 220));
    darkTheme->SetColor("endBlock.text", wing::Color(220, 220, 220));

    darkTheme->SetColor("debug.location.background", wing::Color(197, 200, 112));
    darkTheme->SetColor("debug.location.text", wing::Color::Black);
    darkTheme->SetColor("text", wing::Color(241, 241, 241));

    darkTheme->SetColor("menu.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("menu.text", wing::Color(181, 181, 181));
    darkTheme->SetColor("menu.disabled.text", wing::Color(67, 67, 67));
    darkTheme->SetColor("menu.mouse.over.background", wing::Color(80, 80, 80));
    darkTheme->SetColor("menu.open.background", wing::Color(37, 37, 38));
    darkTheme->SetColor("menu.shadow", wing::Color(36, 36, 37));
    darkTheme->SetColor("menu.child.indicator", wing::Color(166, 166, 166));
    darkTheme->SetColor("menu.frame", wing::Color(60, 60, 60));
    darkTheme->SetColor("menu.item.separator", wing::Color(91, 91, 92));

    darkTheme->SetColor("status.bar.background", wing::Color(60, 60, 60));
    darkTheme->SetColor("status.bar.text", wing::Color(181, 181, 181));
    darkTheme->SetColor("status.bar.top.line", wing::Color(63, 63, 70));
    darkTheme->SetColor("status.bar.sunken.border.outer.top.left", wing::Color(25, 25, 25));
    darkTheme->SetColor("status.bar.sunken.border.inner.top.left", wing::Color(30, 30, 30));
    darkTheme->SetColor("status.bar.sunken.border.outer.right.bottom", wing::Color(53, 53, 55));
    darkTheme->SetColor("status.bar.sunken.border.inner.right.bottom", wing::Color(45, 45, 48));
    darkTheme->SetColor("status.bar.raised.border.outer.top.left", wing::Color(45, 45, 48));
    darkTheme->SetColor("status.bar.raised.border.outer.right.bottom", wing::Color(25, 25, 25));
    darkTheme->SetColor("status.bar.raised.border.inner.top.left", wing::Color(53, 53, 55));
    darkTheme->SetColor("status.bar.raised.border.inner.right.bottom", wing::Color(30, 30, 30));

    darkTheme->SetBitmapName("prev", "cmcode.prev.dark.bitmap");
    darkTheme->SetBitmapName("next", "cmcode.next.dark.bitmap");
    darkTheme->SetBitmapName("save", "cmcode.save.dark.bitmap");
    darkTheme->SetBitmapName("save.all", "cmcode.save.all.dark.bitmap");
    darkTheme->SetBitmapName("build.solution", "cmcode.build.solution.dark.bitmap");
    darkTheme->SetBitmapName("build.project", "cmcode.build.project.dark.bitmap");
    darkTheme->SetBitmapName("cancel.build", "cmcode.cancel.build.dark.bitmap");
    darkTheme->SetBitmapName("debug.start", "cmcode.debug.start.dark.bitmap");
    darkTheme->SetBitmapName("debug.stop", "cmcode.debug.stop.dark.bitmap");
    darkTheme->SetBitmapName("show.next.statement", "cmcode.debug.show.next.statement.dark.bitmap");
    darkTheme->SetBitmapName("step.over", "cmcode.debug.step.over.dark.bitmap");
    darkTheme->SetBitmapName("step.into", "cmcode.debug.step.into.dark.bitmap");
    darkTheme->SetBitmapName("step.out", "cmcode.debug.step.out.dark.bitmap");

    darkTheme->SetBitmapName("file", "file.dark.bitmap");
    darkTheme->SetBitmapName("xmlfile", "xml_file.dark.bitmap");
    darkTheme->SetBitmapName("project", "cmproject.dark.bitmap");
    darkTheme->SetBitmapName("solution", "cmsolution.dark.bitmap");

    darkTheme->SetCursorName("horizontal.splitter.cursor", "horizontal.splitter.dark.wing.cursor");
    darkTheme->SetCursorName("vertical.splitter.cursor", "vertical.splitter.dark.wing.cursor");

    AddTheme(darkTheme.release());
}

void MakeThemes()
{
    MakeLightTheme();
    MakeDarkTheme();
}

void InitThemes()
{
    if (!wing::ThemesFileExists())
    {
        MakeThemes();
        wing::SaveThemes();
    }
    wing::LoadThemes();
    const Options& options = GetOptions();
    wing::SetTheme(options.theme);
}

} // namespace cmcode
