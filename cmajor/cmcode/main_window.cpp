// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef min
#undef max
#undef MessageBox

module cmcode.main.window;

import cmcode.solution;
import cmcode.tool.bar;
import cmcode.configuration;
import cmcode.config;
import cmcode.startup.dialog;
import cmcode.action;
import cmcode.build;
import cmcode.debug;
import cmcode.run;
import cmcode.options.dialog;
import cmcode.add_new_project.dialog;
import cmcode.add_new_source_file.dialog;
import cmcode.add_new_resource_file.dialog;
import cmcode.add_new_text_file.dialog;
import cmcode.build.settings.dialog;
import cmcode.program.arguments.dialog;
import cmcode.project.references.dialog;
import cmcode.new_project.dialog;
import cmcode.about.dialog;
import cmcode.go.to.line.dialog;
import cmcode.search.dialog;
import util;
import std.filesystem;

namespace cmcode {

const char* cmajorCodeVersion = "5.1.0";

bool CmCodeLogFlagExists()
{
    std::string cmcodeLogFlagFilePath = util::Path::Combine(cmajor::service::CmajorConfigDir(), "cmcode.log.flag");
    return std::filesystem::exists(cmcodeLogFlagFilePath);
}

struct ShowDialogGuard
{
    ShowDialogGuard(bool& showingDialog_) : showingDialog(showingDialog_)
    {
        showingDialog = true;
    }
    ~ShowDialogGuard()
    {
        showingDialog = false;
    }
    bool& showingDialog;
};

bool IsProgramTarget(cmajor::ast::Target target)
{
    return target == cmajor::ast::Target::program || target == cmajor::ast::Target::winapp || target == cmajor::ast::Target::winguiapp;
}

ExpressionEvaluateRequest::ExpressionEvaluateRequest(const std::string& expression_, const wing::Point& screenLoc_) : expression(expression_), screenLoc(screenLoc_)
{
}

MainWindow::MainWindow(const std::string& filePath) : 
    wing::Window(wing::WindowCreateParams().BackgroundColor(wing::GetColor("window.background")).
        Text("Cmajor Code").WindowClassName("cmajor.code." + std::to_string(util::GetPid()))),
    newProjectMenuItem(nullptr),
    openProjectMenuItem(nullptr),
    closeSolutionMenuItem(nullptr),
    saveMenuItem(nullptr),
    saveAllMenuItem(nullptr),
    exitMenuItem(nullptr),
    copyMenuItem(nullptr),
    cutMenuItem(nullptr),
    pasteMenuItem(nullptr),
    undoMenuItem(nullptr),
    redoMenuItem(nullptr),
    gotoMenuItem(nullptr),
    searchMenuItem(nullptr),
    optionsMenuItem(nullptr),
    callStackMenuItem(nullptr),
    localsMenuItem(nullptr),
    errorsMenuItem(nullptr),
    searchResultsMenuItem(nullptr),
    portMapMenuItem(nullptr),
    buildSolutionMenuItem(nullptr),
    rebuildSolutionMenuItem(nullptr),
    cleanSolutionMenuItem(nullptr),
    buildActiveProjectMenuItem(nullptr),
    rebuildActiveProjectMenuItem(nullptr),
    cleanActiveProjectMenuItem(nullptr),
    buildSettingsMenuItem(nullptr),
    startDebuggingMenuItem(nullptr),
    startWithoutDebuggingMenuItem(nullptr),
    terminateProcessMenuItem(nullptr),
    stopDebuggingMenuItem(nullptr),
    showNextStatementMenuItem(nullptr),
    stepOverMenuItem(nullptr),
    stepIntoMenuItem(nullptr),
    stepOutMenuItem(nullptr),
    toggleBreakpointMenuItem(nullptr),
    programArgumentsMenuItem(nullptr),
    closeAllTabsMenuItem(nullptr),
    closeExternalTabsMenuItem(nullptr),
    homepageMenuItem(nullptr),
    localDocumentationMenuItem(nullptr),
    aboutMenuItem(nullptr),
    prevToolButton(nullptr),
    nextToolButton(nullptr),
    saveToolButton(nullptr),
    saveAllToolButton(nullptr),
    cppToolButton(nullptr),
    llvmToolButton(nullptr),
    masmToolButton(nullptr),
    cmToolButton(nullptr),
    sbinToolButton(nullptr),
    debugToolButton(nullptr),
    releaseToolButton(nullptr),
    optLevelZeroToolButton(nullptr),
    optLevelOneToolButton(nullptr),
    optLevelTwoToolButton(nullptr),
    optLevelThreeToolButton(nullptr),
    buildSolutionToolButton(nullptr),
    buildActiveProjectToolButton(nullptr),
    stopBuildToolButton(nullptr),
    startDebuggingToolButton(nullptr),
    stopDebuggingToolButton(nullptr),
    showNextStatementToolButton(nullptr),
    stepOverToolButton(nullptr),
    stepIntoToolButton(nullptr),
    stepOutToolButton(nullptr),
    verticalSplitContainer(nullptr),
    horizontalSplitContainer(nullptr),
    codeTabControl(nullptr),
    outputTabControl(nullptr),
    outputTabPage(nullptr),
    outputLogView(nullptr),
    errorTabPage(nullptr),
    errorView(nullptr),
    logTabPage(nullptr),
    log(nullptr),
    consoleTabPage(nullptr),
    console(nullptr),
    debugTabPage(nullptr),
    debugLog(nullptr),
    statusBar(nullptr),
    searchResultsTabPage(nullptr),
    searchResultsView(nullptr),
    callStackTabPage(nullptr),
    callStackView(nullptr),
    localsTabPage(nullptr),
    localsView(nullptr),
    buildIndicatorStatuBarItem(nullptr),
    editorReadWriteIndicatorStatusBarItem(nullptr),
    editorDirtyIndicatorStatusBarItem(nullptr),
    sourceFilePathStatusBarItem(nullptr),
    //codeCompletionStatusBarItem(nullptr),
    lineStatusBarItem(nullptr),
    columnStatusBarItem(nullptr),
    buildProgressCounter(0),
    buildProgressTimerRunning(false),
    setMaximizedSplitterDistance(false),
    sizeChanged(false),
    verticalSplitContainerFactor(0),
    horizontalSplitContainerFactor(0),
    showingDialog(false),
    state(MainWindowState::idle),
    programRunning(false),
    startDebugging(false),
    signalReceived(false),
    callStackDepth(-1),
    callStackOpen(false),
    localsViewOpen(false),
    backend("llvm"),
    config("debug"),
    optLevel(2),
    pid(util::GetPid()),
    // ccState(CCState::idle),
    cmajorCodeFormat("cmajor.code"),
    locations(this),
    toolTipWindow(new wing::ToolTip(wing::ToolTipCreateParams().Defaults()))
    // codeCompletionListView(nullptr),
    // paramHelpView(nullptr)
    // ccTimerRunning(false),
    // editModuleLoaded(false),
    // ccCanSelect(false)
{
    SetBackgroundItemName("window.background");
    buildIndicatorTexts.push_back("|");
    buildIndicatorTexts.push_back("/");
    buildIndicatorTexts.push_back(util::ToUtf8(std::u32string(1, char32_t(0x2015))));
    buildIndicatorTexts.push_back("\\");
    std::unique_ptr<wing::MenuBar> menuBar(new wing::MenuBar());
    std::unique_ptr<wing::MenuItem> fileMenuItem(new wing::MenuItem("&File"));
    std::unique_ptr<wing::MenuItem> newProjectMenuItemPtr(new wing::MenuItem("&New Project..."));
    newProjectMenuItem = newProjectMenuItemPtr.get();
    newProjectMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::n);
    newProjectMenuItem->Click().AddHandler(this, &MainWindow::NewProjectClick);
    fileMenuItem->AddMenuItem(newProjectMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> openProjectMenuItemPtr(new wing::MenuItem("&Open Project/Solution..."));
    openProjectMenuItem = openProjectMenuItemPtr.get();
    openProjectMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::o);
    openProjectMenuItem->Click().AddHandler(this, &MainWindow::OpenProjectClick);
    fileMenuItem->AddMenuItem(openProjectMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> closeSolutionMenuItemPtr(new wing::MenuItem("Close Solution"));
    closeSolutionMenuItem = closeSolutionMenuItemPtr.get();
    closeSolutionMenuItem->Click().AddHandler(this, &MainWindow::CloseSolutionClick);
    fileMenuItem->AddMenuItem(closeSolutionMenuItemPtr.release());
    fileMenuItem->AddMenuItem(new wing::MenuItemSeparator());
    std::unique_ptr<wing::MenuItem> saveMenuItemPtr(new wing::MenuItem("&Save"));
    saveMenuItem = saveMenuItemPtr.get();
    saveMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::s);
    saveMenuItem->Click().AddHandler(this, &MainWindow::SaveClick);
    fileMenuItem->AddMenuItem(saveMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> saveAllMenuItemPtr(new wing::MenuItem("Save A&ll"));
    saveAllMenuItem = saveAllMenuItemPtr.get();
    saveAllMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::shiftModifier | wing::Keys::s);
    saveAllMenuItem->Click().AddHandler(this, &MainWindow::SaveAllClick);
    fileMenuItem->AddMenuItem(saveAllMenuItemPtr.release());
    fileMenuItem->AddMenuItem(new wing::MenuItemSeparator());
    std::unique_ptr<wing::MenuItem> exitMenuItemPtr(new wing::MenuItem("E&xit"));
    exitMenuItem = exitMenuItemPtr.get();
    exitMenuItem->SetShortcut(wing::Keys::altModifier | wing::Keys::f4);
    exitMenuItem->Click().AddHandler(this, &MainWindow::ExitClick);
    fileMenuItem->AddMenuItem(exitMenuItemPtr.release());
    menuBar->AddMenuItem(fileMenuItem.release());
    std::unique_ptr<wing::MenuItem> editMenuItem(new wing::MenuItem("&Edit"));
    std::unique_ptr<wing::MenuItem> copyMenuItemPtr(new wing::MenuItem("&Copy"));
    copyMenuItem = copyMenuItemPtr.get();
    copyMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::c);
    copyMenuItem->Click().AddHandler(this, &MainWindow::CopyClick);
    editMenuItem->AddMenuItem(copyMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> cutMenuItemPtr(new wing::MenuItem("C&ut"));
    cutMenuItem = cutMenuItemPtr.get();
    cutMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::x);
    cutMenuItem->Click().AddHandler(this, &MainWindow::CutClick);
    editMenuItem->AddMenuItem(cutMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> pasteMenuItemPtr(new wing::MenuItem("&Paste"));
    pasteMenuItem = pasteMenuItemPtr.get();
    pasteMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::v);
    pasteMenuItem->Click().AddHandler(this, &MainWindow::PasteClick);
    editMenuItem->AddMenuItem(pasteMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> undoMenuItemPtr(new wing::MenuItem("U&ndo"));
    undoMenuItem = undoMenuItemPtr.get();
    undoMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::z);
    undoMenuItem->Click().AddHandler(this, &MainWindow::UndoClick);
    editMenuItem->AddMenuItem(undoMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> redoMenuItemPtr(new wing::MenuItem("&Redo"));
    redoMenuItem = redoMenuItemPtr.get();
    redoMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::y);
    redoMenuItem->Click().AddHandler(this, &MainWindow::RedoClick);
    editMenuItem->AddMenuItem(redoMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> gotoMenuItemPtr(new wing::MenuItem("&Go To Line..."));
    gotoMenuItem = gotoMenuItemPtr.get();
    gotoMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::g);
    gotoMenuItem->Click().AddHandler(this, &MainWindow::GotoClick);
    editMenuItem->AddMenuItem(gotoMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> searchMenuItemPtr(new wing::MenuItem("&Search..."));
    searchMenuItem = searchMenuItemPtr.get();
    searchMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::f);
    searchMenuItem->Click().AddHandler(this, &MainWindow::SearchClick);
    editMenuItem->AddMenuItem(searchMenuItemPtr.release());
    editMenuItem->AddMenuItem(new wing::MenuItemSeparator());
    std::unique_ptr<wing::MenuItem> optionsMenuItemPtr(new wing::MenuItem("&Options..."));
    optionsMenuItem = optionsMenuItemPtr.get();
    optionsMenuItem->Click().AddHandler(this, &MainWindow::OptionsClick);
    editMenuItem->AddMenuItem(optionsMenuItemPtr.release());
    menuBar->AddMenuItem(editMenuItem.release());
    std::unique_ptr<wing::MenuItem> viewMenuItem(new wing::MenuItem("&View"));
    std::unique_ptr<wing::MenuItem> debugWindowsMenuItem(new wing::MenuItem("&Debug Windows"));
    std::unique_ptr<wing::MenuItem> callStackMenuItemPtr(new wing::MenuItem("&Call Stack"));
    callStackMenuItem = callStackMenuItemPtr.get();
    callStackMenuItem->Click().AddHandler(this, &MainWindow::CallStackClick);
    debugWindowsMenuItem->AddMenuItem(callStackMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> localsMenuItemPtr(new wing::MenuItem("&Locals"));
    localsMenuItem = localsMenuItemPtr.get();
    localsMenuItem->Click().AddHandler(this, &MainWindow::LocalsClick);
    debugWindowsMenuItem->AddMenuItem(localsMenuItemPtr.release());
    viewMenuItem->AddMenuItem(debugWindowsMenuItem.release());
    std::unique_ptr<wing::MenuItem> errorsMenuItemPtr(new wing::MenuItem("&Errors"));
    errorsMenuItem = errorsMenuItemPtr.get();
    errorsMenuItem->Click().AddHandler(this, &MainWindow::ErrorsClick);
    viewMenuItem->AddMenuItem(errorsMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> searchResultsMenuItemPtr(new wing::MenuItem("&Search Results"));
    searchResultsMenuItem = searchResultsMenuItemPtr.get();
    searchResultsMenuItem->Click().AddHandler(this, &MainWindow::SearchResultsClick);
    viewMenuItem->AddMenuItem(searchResultsMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> portMapMenuItemPtr(new wing::MenuItem("&Port Map"));
    portMapMenuItem = portMapMenuItemPtr.get();
    portMapMenuItem->Click().AddHandler(this, &MainWindow::PortMapClick);
    viewMenuItem->AddMenuItem(portMapMenuItemPtr.release());
    menuBar->AddMenuItem(viewMenuItem.release());
    std::unique_ptr<wing::MenuItem> buildMenuItem(new wing::MenuItem("&Build"));
    std::unique_ptr<wing::MenuItem> buildSolutionMenuItemPtr(new wing::MenuItem("&Build Solution"));
    buildSolutionMenuItem = buildSolutionMenuItemPtr.get();
    buildSolutionMenuItem->SetShortcut(wing::Keys::f7);
    buildSolutionMenuItem->Click().AddHandler(this, &MainWindow::BuildSolutionClick);
    buildMenuItem->AddMenuItem(buildSolutionMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> rebuildSolutionMenuItemPtr(new wing::MenuItem("&Rebuild Solution"));
    rebuildSolutionMenuItem = rebuildSolutionMenuItemPtr.get();
    rebuildSolutionMenuItem->SetShortcut(wing::Keys::altModifier | wing::Keys::f7);
    rebuildSolutionMenuItem->Click().AddHandler(this, &MainWindow::RebuildSolutionClick);
    buildMenuItem->AddMenuItem(rebuildSolutionMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> cleanSolutionMenuItemPtr(new wing::MenuItem("&Clean Solution"));
    cleanSolutionMenuItem = cleanSolutionMenuItemPtr.get();
    cleanSolutionMenuItem->Click().AddHandler(this, &MainWindow::CleanSolutionClick);
    buildMenuItem->AddMenuItem(cleanSolutionMenuItemPtr.release());
    buildMenuItem->AddMenuItem(new wing::MenuItemSeparator());
    std::unique_ptr<wing::MenuItem> buildActiveProjectMenuItemPtr(new wing::MenuItem("Build &Active Project"));
    buildActiveProjectMenuItem = buildActiveProjectMenuItemPtr.get();
    buildActiveProjectMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::b);
    buildActiveProjectMenuItem->Click().AddHandler(this, &MainWindow::BuildActiveProjectClick);
    buildMenuItem->AddMenuItem(buildActiveProjectMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> rebuildActiveProjectMenuItemPtr(new wing::MenuItem("Reb&uild Active Project"));
    rebuildActiveProjectMenuItem = rebuildActiveProjectMenuItemPtr.get();
    rebuildActiveProjectMenuItem->Click().AddHandler(this, &MainWindow::RebuildActiveProjectClick);
    buildMenuItem->AddMenuItem(rebuildActiveProjectMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> cleanActiveProjectMenuItemPtr(new wing::MenuItem("C&lean Active Project"));
    cleanActiveProjectMenuItem = cleanActiveProjectMenuItemPtr.get();
    cleanActiveProjectMenuItem->Click().AddHandler(this, &MainWindow::CleanActiveProjectClick);
    buildMenuItem->AddMenuItem(cleanActiveProjectMenuItemPtr.release());
    buildMenuItem->AddMenuItem(new wing::MenuItemSeparator());
    std::unique_ptr<wing::MenuItem> buildSettingsMenuItemPtr(new wing::MenuItem("&Settings..."));
    buildSettingsMenuItem = buildSettingsMenuItemPtr.get();
    buildSettingsMenuItem->Click().AddHandler(this, &MainWindow::BuildSettingsClick);
    buildMenuItem->AddMenuItem(buildSettingsMenuItemPtr.release());
    menuBar->AddMenuItem(buildMenuItem.release());
    std::unique_ptr<wing::MenuItem> debugMenuItem(new wing::MenuItem("&Debug"));
    std::unique_ptr<wing::MenuItem> startDebuggingMenuItemPtr(new wing::MenuItem("&Start Debugging"));
    startDebuggingMenuItem = startDebuggingMenuItemPtr.get();
    startDebuggingMenuItem->SetShortcut(wing::Keys::f5);
    startDebuggingMenuItem->Click().AddHandler(this, &MainWindow::StartDebuggingClick);
    debugMenuItem->AddMenuItem(startDebuggingMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> startWithoutDebuggingMenuItemPtr(new wing::MenuItem("Start &Without Debugging"));
    startWithoutDebuggingMenuItem = startWithoutDebuggingMenuItemPtr.get();
    startWithoutDebuggingMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::f5);
    startWithoutDebuggingMenuItem->Click().AddHandler(this, &MainWindow::StartWithoutDebuggingClick);
    debugMenuItem->AddMenuItem(startWithoutDebuggingMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> terminateProcessMenuItemPtr(new wing::MenuItem("&Terminate Process"));
    terminateProcessMenuItem = terminateProcessMenuItemPtr.get();
    terminateProcessMenuItem->Click().AddHandler(this, &MainWindow::TerminateProcessClick);
    debugMenuItem->AddMenuItem(terminateProcessMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> stopDebuggingMenuItemPtr(new wing::MenuItem("Sto&p Debugging"));
    stopDebuggingMenuItem = stopDebuggingMenuItemPtr.get();
    stopDebuggingMenuItem->SetShortcut(wing::Keys::shiftModifier | wing::Keys::f5);
    stopDebuggingMenuItem->Click().AddHandler(this, &MainWindow::StopDebuggingClick);
    debugMenuItem->AddMenuItem(stopDebuggingMenuItemPtr.release());
    debugMenuItem->AddMenuItem(new wing::MenuItemSeparator());
    std::unique_ptr<wing::MenuItem> showNextStatementMenuItemPtr(new wing::MenuItem("S&how Next Statement"));
    showNextStatementMenuItem = showNextStatementMenuItemPtr.get();
    showNextStatementMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::j);
    showNextStatementMenuItem->Click().AddHandler(this, &MainWindow::ShowNextStatementClick);
    debugMenuItem->AddMenuItem(showNextStatementMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> stepOverMenuItemPtr(new wing::MenuItem("Step &Over"));
    stepOverMenuItem = stepOverMenuItemPtr.get();
    stepOverMenuItem->SetShortcut(wing::Keys::f12);
    stepOverMenuItem->Click().AddHandler(this, &MainWindow::StepOverClick);
    debugMenuItem->AddMenuItem(stepOverMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> stepIntoMenuItemPtr(new wing::MenuItem("Step &Into"));
    stepIntoMenuItem = stepIntoMenuItemPtr.get();
    stepIntoMenuItem->SetShortcut(wing::Keys::f11);
    stepIntoMenuItem->Click().AddHandler(this, &MainWindow::StepIntoClick);
    debugMenuItem->AddMenuItem(stepIntoMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> stepOutMenuItemPtr(new wing::MenuItem("Step O&ut"));
    stepOutMenuItem = stepOutMenuItemPtr.get();
    stepOutMenuItem->SetShortcut(wing::Keys::shiftModifier | wing::Keys::f11);
    stepOutMenuItem->Click().AddHandler(this, &MainWindow::StepOutClick);
    debugMenuItem->AddMenuItem(stepOutMenuItemPtr.release());
    debugMenuItem->AddMenuItem(new wing::MenuItemSeparator());
    std::unique_ptr<wing::MenuItem> toggleBreakpointMenuItemPtr(new wing::MenuItem("&Toggle Breakpoint"));
    toggleBreakpointMenuItem = toggleBreakpointMenuItemPtr.get();
    toggleBreakpointMenuItem->SetShortcut(wing::Keys::f9);
    toggleBreakpointMenuItem->Click().AddHandler(this, &MainWindow::ToggleBreakpointClick);
    debugMenuItem->AddMenuItem(toggleBreakpointMenuItemPtr.release());
    debugMenuItem->AddMenuItem(new wing::MenuItemSeparator());
    std::unique_ptr<wing::MenuItem> programArgumentsMenuItemPtr(new wing::MenuItem("Program &Arguments..."));
    programArgumentsMenuItem = programArgumentsMenuItemPtr.get();
    programArgumentsMenuItem->Click().AddHandler(this, &MainWindow::ProgramArgumentsClick);
    debugMenuItem->AddMenuItem(programArgumentsMenuItemPtr.release());
    menuBar->AddMenuItem(debugMenuItem.release());
    std::unique_ptr<wing::MenuItem> windowMenuItem(new wing::MenuItem("&Window"));
    std::unique_ptr<wing::MenuItem> closeAllTabsMenuItemPtr(new wing::MenuItem("&Close All Tabs"));
    closeAllTabsMenuItem = closeAllTabsMenuItemPtr.get();
    closeAllTabsMenuItem->Click().AddHandler(this, &MainWindow::CloseAllTabsClick);
    windowMenuItem->AddMenuItem(closeAllTabsMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> closeExternalTabsMenuItemPtr(new wing::MenuItem("Close &External Tabs"));
    closeExternalTabsMenuItem = closeExternalTabsMenuItemPtr.get();
    closeExternalTabsMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::w);
    closeExternalTabsMenuItem->Click().AddHandler(this, &MainWindow::CloseExternalTabsClick);
    windowMenuItem->AddMenuItem(closeExternalTabsMenuItemPtr.release());
    menuBar->AddMenuItem(windowMenuItem.release());
    std::unique_ptr<wing::MenuItem> helpMenuItem(new wing::MenuItem("&Help"));
    std::unique_ptr<wing::MenuItem> homepageMenuItemPtr(new wing::MenuItem("&Homepage"));
    homepageMenuItem = homepageMenuItemPtr.get();
    homepageMenuItem->Click().AddHandler(this, &MainWindow::HomepageClick);
    helpMenuItem->AddMenuItem(homepageMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> localDocumentationMenuItemPtr(new wing::MenuItem("&Local Documentation"));
    localDocumentationMenuItem = localDocumentationMenuItemPtr.get();
    localDocumentationMenuItem->Click().AddHandler(this, &MainWindow::LocalDocumentationClick);
    helpMenuItem->AddMenuItem(localDocumentationMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> aboutMenuItemPtr(new wing::MenuItem("&About..."));
    aboutMenuItem = aboutMenuItemPtr.get();
    aboutMenuItem->Click().AddHandler(this, &MainWindow::AboutClick);
    helpMenuItem->AddMenuItem(aboutMenuItemPtr.release());
    menuBar->AddMenuItem(helpMenuItem.release());
    AddChild(menuBar.release());

    std::unique_ptr<wing::ToolBar> toolBarPtr(MakeToolBar());
    toolBar = toolBarPtr.get();

    std::unique_ptr<wing::BorderedControl> borderedToolBar(new wing::BorderedControl(wing::BorderedControlCreateParams(toolBarPtr.release()).SetBorderStyle(wing::BorderStyle::single).
        NormalSingleBorderColor(wing::GetColor("tool.bar.frame")).FocusedSingleBorderColor(wing::GetColor("tool.bar.frame")).SetSize(toolBar->GetSize()).SetDock(wing::Dock::top)));
    borderedToolBar->SetFrameItemName("tool.bar.frame");

    AddToolButtons();

    AddChild(borderedToolBar.release());

    wing::SplitContainerCreateParams verticalSplitContainerCreateParams(wing::SplitterOrientation::vertical);
    verticalSplitContainerCreateParams.BackgroundColor(wing::GetColor("splitContainer.background"));
    verticalSplitContainerCreateParams.Pane1BackgroundColor(wing::GetColor("splitContainer.background")).Pane2BackgroundColor(wing::GetColor("splitContainer.background")).
        BackgroundColor(wing::GetColor("splitContainer.background")).SplitterBackgroundColor(wing::GetColor("splitter.background")).SplitterEdgeColor(wing::GetColor("splitter.edge")).SplitterDistance(0).SetDock(wing::Dock::fill);
    std::unique_ptr<wing::SplitContainer> verticalSplitContainerPtr(new wing::SplitContainer(verticalSplitContainerCreateParams));
    verticalSplitContainer = verticalSplitContainerPtr.get();
    verticalSplitContainer->SplitterDistanceChanged().AddHandler(this, &MainWindow::VerticalSplitContainerSplitterDistanceChanged);

    wing::SplitContainerCreateParams horizontalSplitContainerCreateParams(wing::SplitterOrientation::horizontal);
    horizontalSplitContainerCreateParams.BackgroundColor(wing::GetColor("splitContainer.background"));
    horizontalSplitContainerCreateParams.Pane1BackgroundColor(wing::GetColor("splitContainer.background")).Pane2BackgroundColor(wing::GetColor("splitContainer.background")).
        BackgroundColor(wing::GetColor("splitContainer.background")).SplitterBackgroundColor(wing::GetColor("splitter.background")).SplitterEdgeColor(wing::GetColor("splitter.edge")).SplitterDistance(0).SetDock(wing::Dock::fill);
    std::unique_ptr<wing::SplitContainer> horizontalSplitContainerPtr(new wing::SplitContainer(horizontalSplitContainerCreateParams));
    horizontalSplitContainer = horizontalSplitContainerPtr.get();
    horizontalSplitContainer->SplitterDistanceChanged().AddHandler(this, &MainWindow::HorizontalSplitContainerSplitterDistanceChanged);
    std::unique_ptr<wing::TabControl> codeTabControlPtr(
        new wing::TabControl(wing::TabControlCreateParams().
            BackgroundColor(wing::GetColor("code.tab.control.background")).
            FrameColor(wing::GetColor("code.tab.control.frame")).
            TextColor(wing::GetColor("code.tab.control.text")).
            NormalBackgroundColor(wing::GetColor("code.tab.control.tab.normal.background")).
            SelectedBackgroundColor(wing::GetColor("code.tab.control.tab.selected.background")).
            CloseBoxSelectedColor(wing::GetColor("code.tab.control.close.box.selected.background")).
            SetDock(wing::Dock::fill)));
    codeTabControl = codeTabControlPtr.get();
    codeTabControl->SetBackgroundItemName("code.tab.control.background");
    codeTabControl->SetFrameItemName("code.tab.control.frame");
    codeTabControl->SetTextItemName("code.tab.control.text");
    codeTabControl->SetTabNormalBackgroundItemName("code.tab.control.tab.normal.background");
    codeTabControl->SetTabSelectedBackgroundItemName("code.tab.control.tab.selected.background");
    codeTabControl->SetCloseBoxSelectedBackgroundItemName("code.tab.control.close.box.selected.background");
    codeTabControl->TabPageSelected().AddHandler(this, &MainWindow::CodeTabPageSelected);
    codeTabControl->ControlRemoved().AddHandler(this, &MainWindow::CodeTabPageRemoved);
    std::unique_ptr<Control> paddedCodeTabControl(new wing::PaddedControl(wing::PaddedControlCreateParams(codeTabControlPtr.release()).Defaults()));
    std::unique_ptr<Control> borderedCodeTabControl(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedCodeTabControl.release()).SetBorderStyle(wing::BorderStyle::single).
        NormalSingleBorderColor(wing::GetColor("code.tab.control.frame")).FocusedSingleBorderColor(wing::GetColor("code.tab.control.frame")).SetDock(wing::Dock::fill)));
    borderedCodeTabControl->SetFrameItemName("code.tab.control.frame");
    horizontalSplitContainer->Pane1Container()->AddChild(borderedCodeTabControl.release());
    SolutionExplorerCreateParams solutionExplorerCreateParams;
    solutionExplorerCreateParams.BackgroundColor(wing::GetColor("solution.explorer.background"));
    solutionExplorerCreateParams.treeViewCreateParams.BackgroundColor(wing::GetColor("solution.explorer.background"));
    solutionExplorerCreateParams.SetDock(wing::Dock::fill);
    std::unique_ptr<SolutionExplorer> solutionExplorerPtr(new SolutionExplorer(solutionExplorerCreateParams, this));
    solutionExplorer = solutionExplorerPtr.get();
    solutionExplorer->SetBackgroundItemName("solution.explorer.background");
    solutionExplorer->SetImageList(&imageList);
    horizontalSplitContainer->Pane2Container()->AddChild(solutionExplorerPtr.release());
    verticalSplitContainer->Pane1Container()->AddChild(horizontalSplitContainerPtr.release());
    std::unique_ptr<wing::TabControl> outputTabControlPtr(new wing::TabControl(wing::TabControlCreateParams().
        BackgroundColor(wing::GetColor("output.tab.control.background")).
        FrameColor(wing::GetColor("output.tab.control.frame")).
        TextColor(wing::GetColor("output.tab.control.text")).
        NormalBackgroundColor(wing::GetColor("output.tab.control.tab.normal.background")).
        SelectedBackgroundColor(wing::GetColor("output.tab.control.tab.selected.background")).
        CloseBoxSelectedColor(wing::GetColor("output.tab.control.close.box.selected.background"))
    ));
    outputTabControl = outputTabControlPtr.get();
    outputTabControl->SetBackgroundItemName("output.tab.control.background");
    outputTabControl->SetFrameItemName("output.tab.control.frame");
    outputTabControl->SetTextItemName("output.tab.control.text");
    outputTabControl->SetTabNormalBackgroundItemName("output.tab.control.tab.normal.background");
    outputTabControl->SetTabSelectedBackgroundItemName("output.tab.control.tab.selected.background");
    outputTabControl->SetCloseBoxSelectedBackgroundItemName("output.tab.control.close.box.selected.background");
    outputTabControl->ControlRemoved().AddHandler(this, &MainWindow::OutputTabControlTabPageRemoved);
    outputTabControl->TabPageSelected().AddHandler(this, &MainWindow::OutputTabControlTabPageSelected);
    std::unique_ptr<Control> paddedOutputTabControl(new wing::PaddedControl(wing::PaddedControlCreateParams(outputTabControlPtr.release()).Defaults()));
    std::unique_ptr<Control> borderedOutptTabControl(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedOutputTabControl.release()).SetBorderStyle(wing::BorderStyle::single).
        NormalSingleBorderColor(wing::GetColor("output.tab.control.frame")).FocusedSingleBorderColor(wing::GetColor("output.tab.control.frame")).SetDock(wing::Dock::fill)));
    borderedOutptTabControl->SetFrameItemName("output.tab.control.frame");
    std::unique_ptr<wing::TabPage> outputTabPagePtr(new wing::TabPage("Output", "output"));
    outputTabPage = outputTabPagePtr.get();
    std::unique_ptr<wing::LogView> outputLogViewPtr(
        new wing::LogView(wing::TextViewCreateParams().SelectionBackgroundColor(wing::GetColor("selection.background")).BackgroundColor(wing::GetColor("output.log.background")).TextColor(wing::GetColor("output.log.text"))));
    outputLogView = outputLogViewPtr.get();
    outputLogView->SetFlag(wing::ControlFlags::scrollSubject);
    outputLogView->SetDoubleBuffered();
    outputLogView->SetBackgroundItemName("output.log.background");
    outputLogView->SetTextItemName("output.log.text");
    std::unique_ptr<Control> scrollableOutputLogView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(outputLogViewPtr.release()).SetDock(wing::Dock::fill)));
    outputTabPage->AddChild(scrollableOutputLogView.release());
    outputTabControl->AddTabPage(outputTabPagePtr.release());
    if (CmCodeLogFlagExists())
    {
        std::unique_ptr<wing::TabPage> logTabPagePtr(new wing::TabPage("Log", "log"));
        logTabPage = logTabPagePtr.get();
        std::unique_ptr<wing::LogView> logViewPtr(new wing::LogView(wing::TextViewCreateParams().BackgroundColor(wing::GetColor("log.view.background")).TextColor(wing::GetColor("log.view.text"))));
        log = logViewPtr.get();
        log->SetFlag(wing::ControlFlags::scrollSubject);
        log->SetDoubleBuffered();
        std::unique_ptr<Control> scrollableLogView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(logViewPtr.release()).SetDock(wing::Dock::fill)));
        logTabPage->AddChild(scrollableLogView.release());
        wing::Application::SetLogView(log);
        outputTabControl->AddTabPage(logTabPagePtr.release());
    }
    verticalSplitContainer->Pane2Container()->AddChild(borderedOutptTabControl.release());
    AddChild(verticalSplitContainerPtr.release());

    std::unique_ptr<wing::StatusBar> statusBarPtr(new wing::StatusBar(wing::StatusBarCreateParams().
        BackgroundColor(wing::GetColor("status.bar.background")).
        TextColor(wing::GetColor("status.bar.text")).
        TopLineColor(wing::GetColor("status.bar.top.line")).
        SunkenBorderOuterTopLeftColor(wing::GetColor("status.bar.sunken.border.outer.top.left")).
        SunkenBorderInnerTopLeftColor(wing::GetColor("status.bar.sunken.border.inner.top.left")).
        SunkenBorderOuterRightBottomColor(wing::GetColor("status.bar.sunken.border.outer.right.bottom")).
        SunkenBorderInnerRightBottomColor(wing::GetColor("status.bar.sunken.border.inner.right.bottom")).
        RaisedBorderOuterTopLeftColor(wing::GetColor("status.bar.raised.border.outer.top.left")).
        RaisedBorderOuterRightBottomColor(wing::GetColor("status.bar.raised.border.outer.right.bottom")).
        RaisedBorderInnerTopLeftColor(wing::GetColor("status.bar.raised.border.inner.top.left")).
        RaisedBorderInnerRightBottomColor(wing::GetColor("status.bar.raised.border.inner.right.bottom"))));
    statusBar = statusBarPtr.get();
    statusBar->SetBackgroundItemName("status.bar.background");
    statusBar->SetTextItemName("status.bar.text");
    statusBar->SetTopLineItemName("status.bar.top.line");
    statusBar->SetSunkenBorderOuterTopLeftItemName("status.bar.sunken.border.outer.top.left");
    statusBar->SetSunkenBorderInnerTopLeftItemName("status.bar.sunken.border.inner.top.left");
    statusBar->SetSunkenBorderOuterRightBottomItemName("status.bar.sunken.border.outer.right.bottom");
    statusBar->SetSunkenBorderInnerRightBottomItemName("status.bar.sunken.border.inner.right.bottom");
    statusBar->SetRaisedBorderOuterTopLeftItemName("status.bar.raised.border.outer.top.left");
    statusBar->SetRaisedBorderOuterRightBottomItemName("status.bar.raised.border.outer.right.bottom");
    statusBar->SetRaisedBorderInnerTopLeftItemName("status.bar.raised.border.inner.top.left");
    statusBar->SetRaisedBorderInnerRightBottomItemName("status.bar.raised.border.inner.right.bottom");
    std::unique_ptr<wing::StatusBarTextItem> buildIndicatorStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(1).BorderStyle(wing::StatusBarItemBorderStyle::sunken)));
    buildIndicatorStatuBarItem = buildIndicatorStatusBarItemPtr.get();
    statusBar->AddItem(buildIndicatorStatusBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> editorReadWriteIndicatorStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(3).BorderStyle(wing::StatusBarItemBorderStyle::sunken)));
    editorReadWriteIndicatorStatusBarItem = editorReadWriteIndicatorStatusBarItemPtr.get();
    statusBar->AddItem(editorReadWriteIndicatorStatusBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> editorDirtyIndicatorStausBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(1).BorderStyle(wing::StatusBarItemBorderStyle::sunken)));
    editorDirtyIndicatorStatusBarItem = editorDirtyIndicatorStausBarItemPtr.get();
    statusBar->AddItem(editorDirtyIndicatorStausBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> sourceFilePathStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(0).BorderStyle(wing::StatusBarItemBorderStyle::sunken)));
    sourceFilePathStatusBarItem = sourceFilePathStatusBarItemPtr.get();
    statusBar->AddItem(sourceFilePathStatusBarItemPtr.release());
    statusBar->AddItem(new wing::StatusBarSpringItem());
/*
    std::unique_ptr<wing::StatusBarTextItem> codeCompletionLabelStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(11).Text("Code Completion Status:").BorderStyle(wing::StatusBarItemBorderStyle::flat)));
    statusBar->AddItem(codeCompletionLabelStatusBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> codeCompletionStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(5).BorderStyle(wing::StatusBarItemBorderStyle::sunken)));
    codeCompletionStatusBarItem = codeCompletionStatusBarItemPtr.get();
    statusBar->AddItem(codeCompletionStatusBarItemPtr.release());
*/
    std::unique_ptr<wing::StatusBarTextItem> lineLabelStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(5).Text("Line:").BorderStyle(wing::StatusBarItemBorderStyle::flat)));
    statusBar->AddItem(lineLabelStatusBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> lineStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(5).BorderStyle(wing::StatusBarItemBorderStyle::sunken)));
    lineStatusBarItem = lineStatusBarItemPtr.get();
    statusBar->AddItem(lineStatusBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> columnLabelStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(4).Text("Col:").BorderStyle(wing::StatusBarItemBorderStyle::flat)));
    statusBar->AddItem(columnLabelStatusBarItemPtr.release());
    std::unique_ptr<wing::StatusBarTextItem> columnStatusBarItemPtr(new wing::StatusBarTextItem(wing::StatusBarTextItemCreateParams().MaxTextLength(5).BorderStyle(wing::StatusBarItemBorderStyle::sunken)));
    columnStatusBarItem = columnStatusBarItemPtr.get();
    statusBar->AddItem(columnStatusBarItemPtr.release());
    AddChild(statusBarPtr.release());

    AddChild(toolTipWindow);

/*
    CodeCompletionHelpView* codeCompletionHelpView = new CodeCompletionHelpView(CCHelpCreateParams().Defaults(), CodeCompletionHelpViewCreateParams().Defaults());
    codeCompletionHelpView->Hide();
    AddChild(codeCompletionHelpView);
    codeCompletionListView = new CodeCompletionListView(CCListCreateParams().Defaults(), CodeCompletionListViewCreateParams().Defaults(), codeCompletionHelpView);
    codeCompletionListView->Hide();
    AddChild(codeCompletionListView);
    paramHelpView = new CodeCompletionParamHelpView(ParamHelpCreateParams().Defaults(), CodeCompletionParamHelpViewCreateParams().Defaults());
    paramHelpView->Hide();
    AddChild(paramHelpView);
*/

    imageList.AddImage("file.light.bitmap");
    imageList.AddImage("file.dark.bitmap");
    imageList.AddImage("xml_file.light.bitmap");
    imageList.AddImage("xml_file.dark.bitmap");
    imageList.AddImage("cmproject.light.bitmap");
    imageList.AddImage("cmproject.dark.bitmap");
    imageList.AddImage("cmsolution.light.bitmap");
    imageList.AddImage("cmsolution.dark.bitmap");

    locations.SetToolButtons(prevToolButton, nextToolButton);

    cmajor::service::SetServiceMessageHandlerView(this);
    cmajor::service::StartRequestDispatcher();

    SetState(MainWindowState::idle);
    LoadConfigurationSettings();

    AddClipboardListener();

    const Options& options = GetOptions();
    if (filePath.empty() && options.showStartupDialog)
    {
        const std::vector<RecentSolution>& recentSolutions = GetRecentSolutions();
        if (!recentSolutions.empty())
        {
            SetTimer(startupDialogTimer, startupDialogTimerDelay);
        }
    }

    SetTimer(configurationSaveTimerId, configurationSavePeriod);

/*
    if (options.codeCompletion)
    {
        codeCompletionStatusBarItem->SetText("On");
        StartCodeCompletion();
    }
    else
    {
        codeCompletionStatusBarItem->SetText("Off");
    }
*/

    if (!filePath.empty())
    {
        OpenProject(filePath);
    }
}

MainWindow::~MainWindow()
{
    cmajor::service::SetServiceMessageHandlerView(nullptr);
    if (state == MainWindowState::debugging)
    {
        cmajor::service::StopDebugService(); 
    }
    else if (state == MainWindowState::running)
    {
        cmajor::service::StopRunService();
    }
    //StopCodeCompletion(false);
    cmajor::service::StopBuildService();
    cmajor::service::StopRequestDispatcher();
}

void MainWindow::ShowStartupDialog()
{
    StartupDialog dialog;
    if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
    {
        const RecentSolution& selectedSolution = dialog.GetSelectedSolution();
        OpenProject(selectedSolution.filePath);
    }
}

void MainWindow::OnWindowClosing(wing::CancelArgs& args)
{
    try
    {
        wing::Window::OnWindowClosing(args);
        if (!CloseSolution())
        {
            args.cancel = true;
        }
        else
        {
            SaveConfigurationSettings();
            SaveProjectData();
            SaveSolutionData();
            this->RemoveClipboardListener();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::OnKeyDown(wing::KeyEventArgs& args)
{
    try
    {
        wing::Window::OnKeyDown(args);
        if (!args.handled)
        {
            switch (args.key)
            {
            case wing::Keys::escape:
            {
                RemoveContextMenu();
                args.handled = true;
                break;
            }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::MouseUpNotification(wing::MouseEventArgs& args)
{
    try
    {
        if (args.buttons == wing::MouseButtons::lbutton)
        {
            if (!showingDialog)
            {
                RemoveContextMenu();
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::OnTimer(wing::TimerEventArgs& args)
{
    try
    {
        wing::Window::OnTimer(args);
        if (args.timerId == startupDialogTimer)
        {
            KillTimer(startupDialogTimer);
            ShowStartupDialog();
        }
        else if (args.timerId == buildProgressTimerId)
        {
            ShowBuildProgress();
        }
        else if (args.timerId == configurationSaveTimerId)
        {
            SaveConfigurationSettings();
            SaveProjectData();
            SaveSolutionData();
        }
        else if (args.timerId == toolTipTimerId)
        {
            KillTimer(toolTipTimerId);
            toolTipWindow->Hide();
        }
/*
        else if (args.timerId == ccTimerId)
        {
            ParseSource();
        }
*/
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::OnGotFocus()
{
    wing::Window::OnGotFocus();
    SetFocusToEditor();
}

void MainWindow::OnClipboardUpdate()
{
    try
    {
        wing::Window::OnClipboardUpdate();
        wing::Clipboard clipboard(Handle());
        if (clipboard.IsClipboardFormatAvailable(cmajorCodeFormat))
        {
            clipboardData = clipboard.GetStringData(cmajorCodeFormat);
            pasteMenuItem->Enable();
        }
        else
        {
            wing::ClipboardFormat unicodeText(wing::unicodeTextClipboardFormat);
            if (clipboard.IsClipboardFormatAvailable(unicodeText))
            {
                clipboardData = clipboard.GetStringData(unicodeText);
                pasteMenuItem->Enable();
            }
            else
            {
                wing::ClipboardFormat ansiText(wing::textClipboardFormat);
                if (clipboard.IsClipboardFormatAvailable(ansiText))
                {
                    clipboardData = clipboard.GetStringData(ansiText);
                    pasteMenuItem->Enable();
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), "Could not open clipboard, another application window may have clipboard open: " + std::string(ex.what()));
    }
}

bool MainWindow::ProcessMessage(wing::Message& msg)
{
    switch (msg.message)
    {
    case cmajor::service::SM_SERVICE_MESSAGE_AVAILABLE:
    {
        HandleServiceMessage();
        msg.result = 0;
        return true;
    }
    default:
    {
        return wing::Window::ProcessMessage(msg);
    }
    }
}

void MainWindow::OnWindowStateChanged()
{
    wing::Window::OnWindowStateChanged();
    const WindowSettings& windowSettings = GetWindowSettings();
    if (windowSettings.defined)
    {
        if (GetWindowState() == wing::WindowState::normal)
        {
            SetLocation(wing::Point(windowSettings.location.x, windowSettings.location.y));
            SetSize(wing::Size(windowSettings.size.width, windowSettings.size.height));
            horizontalSplitContainer->SetSplitterDistance(windowSettings.normalHorizontalSplitterDistance);
            verticalSplitContainer->SetSplitterDistance(windowSettings.normalVerticalSplitterDistance);
        }
        else if (GetWindowState() == wing::WindowState::maximized)
        {
            setMaximizedSplitterDistance = true;
        }
    }
}

void MainWindow::OnNCPaint(wing::NCPaintEventArgs& args)
{
    if (wing::GetTheme() == "dark")
    {
        wing::Window::HandleNCPaint(args);
    }
    wing::Window::OnNCPaint(args);
}

void MainWindow::OnSizeChanged()
{
    wing::Window::OnSizeChanged();
    if (GetWindowState() == wing::WindowState::maximized && setMaximizedSplitterDistance)
    {
        setMaximizedSplitterDistance = false;
        const WindowSettings& windowSettings = GetWindowSettings();
        if (windowSettings.defined)
        {
            horizontalSplitContainer->SetSplitterDistance(windowSettings.maximizedHorizontalSplitterDistance);
            verticalSplitContainer->SetSplitterDistance(windowSettings.maximizedVerticalSplitterDistance);
        }
    }
    statusBar->SetChanged();
    statusBar->Invalidate();
    if (GetWindowState() == wing::WindowState::normal)
    {
        sizeChanged = true;
        if (verticalSplitContainerFactor != 0)
        {
            int splitterDistance = static_cast<int>(verticalSplitContainerFactor * verticalSplitContainer->GetSize().Height);
            verticalSplitContainer->SetSplitterDistance(splitterDistance);
        }
        if (horizontalSplitContainerFactor != 0)
        {
            int splitterDistance = static_cast<int>(horizontalSplitContainerFactor * horizontalSplitContainer->GetSize().Width);
            horizontalSplitContainer->SetSplitterDistance(splitterDistance);
        }
        sizeChanged = false;
    }
}

void MainWindow::LoadConfigurationSettings()
{
    const WindowSettings& windowSettings = GetWindowSettings();
    if (windowSettings.defined)
    {
        SetWindowState(wing::WindowState(windowSettings.windowState));
        if (GetWindowState() == wing::WindowState::normal)
        {
            SetLocation(wing::Point(windowSettings.location.x, windowSettings.location.y));
            SetSize(wing::Size(windowSettings.size.width, windowSettings.size.height));
            horizontalSplitContainer->SetSplitterDistance(windowSettings.normalHorizontalSplitterDistance);
            verticalSplitContainer->SetSplitterDistance(windowSettings.normalVerticalSplitterDistance);
        }
        else if (GetWindowState() == wing::WindowState::maximized)
        {
            setMaximizedSplitterDistance = true;
        }
    }
}

void MainWindow::AddToolButtons()
{
    toolBar->ClearToolButtons();

    std::unique_ptr<wing::ToolButton> prevToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("prev")).SetPadding(wing::Padding(8, 8, 8, 8)).SetToolTip("Go To Previous Location")));
    prevToolButton = prevToolButtonPtr.get();
    prevToolButton->Click().AddHandler(this, &MainWindow::GotoPreviousLocationClick);
    prevToolButton->Disable();
    toolBar->AddToolButton(prevToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> nextToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("next")).SetPadding(wing::Padding(8, 8, 8, 8)).SetToolTip("Go To Next Location")));
    nextToolButton = nextToolButtonPtr.get();
    nextToolButton->Click().AddHandler(this, &MainWindow::GotoNextLocationClick);
    nextToolButton->Disable();
    toolBar->AddToolButton(nextToolButtonPtr.release());

    toolBar->AddToolButton(new wing::ToolButtonSeparator());

    std::unique_ptr<wing::ToolButton> saveToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("save")).SetPadding(wing::Padding(8, 8, 8, 8)).SetToolTip("Save (Ctrl+S)")));
    saveToolButton = saveToolButtonPtr.get();
    saveToolButton->Click().AddHandler(this, &MainWindow::SaveClick);
    saveToolButton->Disable();
    toolBar->AddToolButton(saveToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> saveAllToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("save.all")).SetPadding(wing::Padding(6, 6, 6, 6)).SetToolTip("Save All (Ctrl+Shift+S)")));
    saveAllToolButton = saveAllToolButtonPtr.get();
    saveAllToolButton->Click().AddHandler(this, &MainWindow::SaveAllClick);
    saveAllToolButton->Disable();
    toolBar->AddToolButton(saveAllToolButtonPtr.release());

    toolBar->AddToolButton(new wing::ToolButtonSeparator());

    wing::Size backEndTextButtonSize(56, 12);
    wing::Size configTextButtonSize(36, 12);
    wing::Size optLevelTextButtonSize(12, 12);

    std::unique_ptr<wing::ToolButton> llvmToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("LLVM").Style(wing::ToolButtonStyle::manual).SetSize(backEndTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using LLVM Backend")));
    llvmToolButton = llvmToolButtonPtr.get();
    llvmToolButton->Click().AddHandler(this, &MainWindow::LlvmButtonClick);
    llvmToolButton->Disable();
    toolBar->AddToolButton(llvmToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> cppToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("C++").Style(wing::ToolButtonStyle::manual).SetSize(backEndTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using C++ Backend")));
    cppToolButton = cppToolButtonPtr.get();
    cppToolButton->Click().AddHandler(this, &MainWindow::CppButtonClick);
    cppToolButton->Disable();
    toolBar->AddToolButton(cppToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> masmToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("MASM").Style(wing::ToolButtonStyle::manual).SetSize(backEndTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using MASM Backend")));
    masmToolButton = masmToolButtonPtr.get();
    masmToolButton->Click().AddHandler(this, &MainWindow::MasmButtonClick);
    masmToolButton->Disable();
    toolBar->AddToolButton(masmToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> cmToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("CM/MASM").Style(wing::ToolButtonStyle::manual).SetSize(backEndTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using CM/MASM Backend")));
    cmToolButton = cmToolButtonPtr.get();
    cmToolButton->Click().AddHandler(this, &MainWindow::CmButtonClick);
    cmToolButton->Disable();
    toolBar->AddToolButton(cmToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> sbinToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("SBIN").Style(wing::ToolButtonStyle::manual).SetSize(backEndTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using SBIN Backend")));
    sbinToolButton = sbinToolButtonPtr.get();
    sbinToolButton->Click().AddHandler(this, &MainWindow::SBinButtonClick);
    sbinToolButton->Disable();
    toolBar->AddToolButton(sbinToolButtonPtr.release());

    toolBar->AddToolButton(new wing::ToolButtonSeparator());

    std::unique_ptr<wing::ToolButton> debugToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("Debug").Style(wing::ToolButtonStyle::manual).SetSize(configTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using Debug Configuration")));
    debugToolButton = debugToolButtonPtr.get();
    debugToolButton->Click().AddHandler(this, &MainWindow::DebugButtonClick);
    debugToolButton->Disable();
    toolBar->AddToolButton(debugToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> releaseToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("Release").Style(wing::ToolButtonStyle::manual).SetSize(configTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using Release Configuration")));
    releaseToolButton = releaseToolButtonPtr.get();
    releaseToolButton->Click().AddHandler(this, &MainWindow::ReleaseButtonClick);
    releaseToolButton->Disable();
    toolBar->AddToolButton(releaseToolButtonPtr.release());

    toolBar->AddToolButton(new wing::ToolButtonSeparator());

    std::unique_ptr<wing::ToolButton> optLevelZeroToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("0").Style(wing::ToolButtonStyle::manual).SetSize(optLevelTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using optimization level 0")));
    optLevelZeroToolButton = optLevelZeroToolButtonPtr.get();
    optLevelZeroToolButton->Click().AddHandler(this, &MainWindow::ZeroButtonClick);
    optLevelZeroToolButton->Disable();
    toolBar->AddToolButton(optLevelZeroToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> optLevelOneToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("1").Style(wing::ToolButtonStyle::manual).SetSize(optLevelTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using optimization level 1")));
    optLevelOneToolButton = optLevelOneToolButtonPtr.get();
    optLevelOneToolButton->Click().AddHandler(this, &MainWindow::OneButtonClick);
    optLevelOneToolButton->Disable();
    toolBar->AddToolButton(optLevelOneToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> optLevelTwoToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("2").Style(wing::ToolButtonStyle::manual).SetSize(optLevelTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using optimization level 2")));
    optLevelTwoToolButton = optLevelTwoToolButtonPtr.get();
    optLevelTwoToolButton->Click().AddHandler(this, &MainWindow::TwoButtonClick);
    optLevelTwoToolButton->Disable();
    toolBar->AddToolButton(optLevelTwoToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> optLevelThreeToolButtonPtr(new wing::TextToolButton(wing::TextToolButtonCreateParams("3").Style(wing::ToolButtonStyle::manual).SetSize(optLevelTextButtonSize).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Compile using optimization level 3")));
    optLevelThreeToolButton = optLevelThreeToolButtonPtr.get();
    optLevelThreeToolButton->Click().AddHandler(this, &MainWindow::ThreeButtonClick);
    optLevelThreeToolButton->Disable();
    toolBar->AddToolButton(optLevelThreeToolButtonPtr.release());

    toolBar->AddToolButton(new wing::ToolButtonSeparator());

    std::unique_ptr<wing::ToolButton> buildSolutionToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("build.solution")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Build Solution (F7)")));
    buildSolutionToolButton = buildSolutionToolButtonPtr.get();
    buildSolutionToolButton->Click().AddHandler(this, &MainWindow::BuildSolutionClick);
    buildSolutionToolButton->Disable();
    toolBar->AddToolButton(buildSolutionToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> buildActiveProjectToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("build.project")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Build Active Project (Ctrl+B)")));
    buildActiveProjectToolButton = buildActiveProjectToolButtonPtr.get();
    buildActiveProjectToolButton->Click().AddHandler(this, &MainWindow::BuildActiveProjectClick);
    buildActiveProjectToolButton->Disable();
    toolBar->AddToolButton(buildActiveProjectToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> stopBuildToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("cancel.build")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Stop Build")));
    stopBuildToolButton = stopBuildToolButtonPtr.get();
    stopBuildToolButton->Click().AddHandler(this, &MainWindow::StopBuildClick);
    stopBuildToolButton->Disable();
    toolBar->AddToolButton(stopBuildToolButtonPtr.release());

    toolBar->AddToolButton(new wing::ToolButtonSeparator());

    std::unique_ptr<wing::ToolButton> startDebuggingToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("debug.start")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Start Debugging (F5)")));
    startDebuggingToolButton = startDebuggingToolButtonPtr.get();
    startDebuggingToolButton->Click().AddHandler(this, &MainWindow::StartDebuggingClick);
    startDebuggingToolButton->Disable();
    toolBar->AddToolButton(startDebuggingToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> stopDebuggingToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("debug.stop")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Stop Debugging (Shift+F5)")));
    stopDebuggingToolButton = stopDebuggingToolButtonPtr.get();
    stopDebuggingToolButton->Click().AddHandler(this, &MainWindow::StopDebuggingClick);
    stopDebuggingToolButton->Disable();
    toolBar->AddToolButton(stopDebuggingToolButtonPtr.release());

    toolBar->AddToolButton(new wing::ToolButtonSeparator());

    std::unique_ptr<wing::ToolButton> showNextStatementToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("show.next.statement")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Show Next Statement (Ctrl+J)")));
    showNextStatementToolButton = showNextStatementToolButtonPtr.get();
    showNextStatementToolButton->Click().AddHandler(this, &MainWindow::ShowNextStatementClick);
    showNextStatementToolButton->Disable();
    toolBar->AddToolButton(showNextStatementToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> stepOverToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("step.over")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Step Over (F12)")));
    stepOverToolButton = stepOverToolButtonPtr.get();
    stepOverToolButton->Click().AddHandler(this, &MainWindow::StepOverClick);
    stepOverToolButton->Disable();
    toolBar->AddToolButton(stepOverToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> stepIntoToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("step.into")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Step Into (F11)")));
    stepIntoToolButton = stepIntoToolButtonPtr.get();
    stepIntoToolButton->Click().AddHandler(this, &MainWindow::StepIntoClick);
    stepIntoToolButton->Disable();
    toolBar->AddToolButton(stepIntoToolButtonPtr.release());

    std::unique_ptr<wing::ToolButton> stepOutToolButtonPtr(new wing::ToolButton(wing::ToolButtonCreateParams().ToolBitMapName(wing::GetBitmapName("step.out")).SetPadding(wing::Padding(8, 8, 8, 8)).
        SetToolTip("Step Out (Shift+F11)")));
    stepOutToolButton = stepOutToolButtonPtr.get();
    stepOutToolButton->Click().AddHandler(this, &MainWindow::StepOutClick);
    stepOutToolButton->Disable();
    toolBar->AddToolButton(stepOutToolButtonPtr.release());
}

void MainWindow::SaveConfigurationSettings()
{
    wing::WindowState windowState = GetWindowState();
    WindowSettings& windowSettings = GetWindowSettings();
    windowSettings.defined = true;
    windowSettings.windowState = static_cast<int>(windowState);
    if (windowState == wing::WindowState::normal)
    {
        wing::Point location = Location();
        WindowLocation windowLocation;
        windowLocation.x = location.X;
        windowLocation.y = location.Y;
        windowSettings.location = windowLocation;
        wing::Size size = GetSize();
        WindowSize windowSize;
        windowSize.width = size.Width;
        windowSize.height = size.Height;
        windowSettings.size = windowSize;
        windowSettings.normalHorizontalSplitterDistance = horizontalSplitContainer->SplitterDistance();
        windowSettings.normalVerticalSplitterDistance = verticalSplitContainer->SplitterDistance();
    }
    else if (windowState == wing::WindowState::maximized)
    {
        windowSettings.maximizedHorizontalSplitterDistance = horizontalSplitContainer->SplitterDistance();
        windowSettings.maximizedVerticalSplitterDistance = verticalSplitContainer->SplitterDistance();
    }
    cmcode::SaveConfiguration();
}

void MainWindow::SaveProjectData()
{
    if (!solutionData) return;
    for (const auto& projectData : solutionData->Projects())
    {
        if (projectData->Changed())
        {
            std::string projectSettingsFilePath = projectData->GetProject()->FilePath();
            projectSettingsFilePath.append(".settings.xml");
            projectData->Save(projectSettingsFilePath);
        }
    }
}

void MainWindow::SaveSolutionData()
{
    if (!solutionData) return;
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        solutionData->SetCurrentOpenFile(editor->FilePath());
        wing::TextView* textView = editor->GetTextView();
        if (textView)
        {
            solutionData->SetCurrentCursorLine(textView->CaretLine());
        }
    }
    solutionData->SetCallStackOpen(callStackOpen);
    solutionData->SetLocalsViewOpen(localsViewOpen);
    if (solutionData->Changed())
    {
        std::string solutionSettingsFilePath = solutionData->GetSolution()->FilePath();
        solutionSettingsFilePath.append(".settings.xml");
        solutionData->Save(solutionSettingsFilePath);
    }
}

void MainWindow::AddClipboardListener()
{
    try
    {
        clipboardListener.reset(new wing::ClipboardListener(Handle()));
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::RemoveClipboardListener()
{
    clipboardListener.reset();
}

void MainWindow::StartBuilding()
{
    SaveAllClick();
    SetEditorsReadOnly();
    SetState(MainWindowState::building);
    GetErrorView()->Clear();
    GetWarningsView()->Clear();
    ClearOutput();
    ResetEditModuleCache();
    buildProgressCounter = 0;
    buildProgressTimerRunning = true;
    SetTimer(buildProgressTimerId, buildProgressTimerPeriod);
}

void MainWindow::StopBuilding()
{
    if (buildProgressTimerRunning)
    {
        buildProgressTimerRunning = false;
        KillTimer(buildProgressTimerId);
    }
    buildIndicatorStatuBarItem->SetText(std::string());
    SetState(MainWindowState::idle);
    SetEditorsReadWrite();
}

void MainWindow::StartDebugging()
{
    ResetDebugLocations();
    if (state == MainWindowState::debugging) return;
    expressionEvaluateRequests.clear();
    savedLocation = cmajor::info::db::Location();
    SetEditorsReadOnly();
    startDebugging = true;
    signalReceived = false;
    SetState(MainWindowState::debugging);
    ClearOutput();
    GetConsole()->Clear();
    ClearCallStack();
    ClearLocals();
    cmajor::ast::Solution* solution = solutionData->GetSolution();
    cmajor::ast::Project* activeProject = solution->ActiveProject();
    ProjectData* projectData = solutionData->GetProjectDataByProject(activeProject);
    if (!projectData)
    {
        throw std::runtime_error("active project has no data");
    }
    const std::string& programArguments = projectData->ProgramArguments();
    std::vector<cmajor::debugger::Breakpoint*> breakpoints = solutionData->GetBreakpoints();
    StartDebugService(backend, config, activeProject, programArguments, breakpoints); 
}

void MainWindow::StopDebugging()
{
    ResetDebugLocations();
    ResetBreakpoints();
    savedLocation = cmajor::info::db::Location();
    startDebuggingMenuItem->SetText("Start Debugging");
    startDebuggingToolButton->SetToolTip("Start Debugging (F5)");
    SetState(MainWindowState::idle);
    SetEditorsReadWrite();
    cmajor::service::PutOutputServiceMessage("debugging stopped");
}

void MainWindow::StartRunning()
{
    cmajor::ast::Solution* solution = solutionData->GetSolution();
    cmajor::ast::Project* activeProject = solution->ActiveProject();
    if (activeProject->GetTarget() != cmajor::ast::Target::program && activeProject->GetTarget() != cmajor::ast::Target::winapp && activeProject->GetTarget() != cmajor::ast::Target::winguiapp)
    {
        throw std::runtime_error("project '" + util::ToUtf8(activeProject->Name()) + "' is a library project");
    }
    ProjectData* projectData = solutionData->GetProjectDataByProject(activeProject);
    if (!projectData)
    {
        throw std::runtime_error("active project has no data");
    }
    SetEditorsReadOnly();
    SetState(MainWindowState::running);
    ClearOutput();
    GetConsole()->Clear();
    const std::string& programArguments = projectData->ProgramArguments();
    RunProgram(backend, config, optLevel, activeProject, programArguments);
}

void MainWindow::StopRunning()
{
    SetState(MainWindowState::idle);
    SetEditorsReadWrite();
    cmajor::service::PutOutputServiceMessage("run service stopped");
}

void MainWindow::StartCodeCompletion()
{
    try
    {
        // StartCodeCompletionService(pid); todo
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::StopCodeCompletion(bool log)
{
    editModuleLoaded = false;
    // StopCodeCompletionService(log); todo
}

void MainWindow::LoadEditModule()
{
    if (!solutionData) return;
    cmajor::ast::Solution* sln = solutionData->GetSolution();
    if (!sln) return;
    editModuleLoaded = true;
    LoadEditModule(sln->ActiveProject());
}

void MainWindow::LoadEditModule(cmajor::ast::Project* project)
{
    if (!project) return;
    // cmcode::LoadEditModule(project->FilePath(), backend, config); todo
}

void MainWindow::LoadEditModuleForCurrentFile()
{
    LoadEditModule(CurrentProject());
}

void MainWindow::ResetEditModuleCache()
{
/*  TODO
    editModuleLoaded = false;
    ccState = CCState::idle;
    cmcode::ResetEditModuleCache();
*/
}

void MainWindow::ParseSource()
{
/*  TODO
    cmajor::ast::Project* project = CurrentProject();
    if (!project) return;
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    if (ccState != CCState::editModuleLoaded) return;
    cmajor::view::Editor* editor = CurrentEditor();
    if (!editor) return;
    if (!editor->IsCmajorEditor()) return;
    if (!editor->IsCCDirty()) return;
    cmajor::view::CmajorEditor* cmajorEditor = static_cast<cmajor::view::CmajorEditor*>(editor);
    CmajorSourceCodeView* cmajorSourceCodeView = cmajorEditor->SourceCodeView();
    if (cmajorSourceCodeView->CCLineNumber() == cmajorSourceCodeView->CaretLine()) return;
    cmajorSourceCodeView->SetCCLineNumber(cmajorSourceCodeView->CaretLine());
    cmajorSourceCodeView->ResetCCDirty();
    if (!editModuleLoaded)
    {
        LoadEditModule();
    }
    cmcode::ParseSource(project->FilePath(), backend, config, cmajorSourceCodeView->FilePath(), cmajorSourceCodeView->GetCursorText());
*/
}

/*  TODO
void MainWindow::GetParamHelpList(int symbolIndex)
{
    cmajor::ast::Project* project = CurrentProject();
    if (!project) return;
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    if (ccState != CCState::editModuleLoaded) return;
    cmajor::view::Editor* editor = CurrentEditor();
    if (!editor) return;
    if (!editor->IsCmajorEditor()) return;
    cmcode::GetParamHelpList(project->FilePath(), backend, config, editor->FilePath(), symbolIndex);
}
*/

void MainWindow::ShowBuildProgress()
{
    if (state == MainWindowState::building)
    {
        buildIndicatorStatuBarItem->SetText(buildIndicatorTexts[buildProgressCounter % 4]);
        ++buildProgressCounter;
    }
}

void MainWindow::HandleServiceMessage()
{
    std::unique_ptr<cmajor::service::ServiceMessage> serviceMessage = cmajor::service::GetServiceMessage();
    if (serviceMessage)
    {
        switch (serviceMessage->Kind())
        {
        case cmajor::service::ServiceMessageKind::clearOutput:
        {
            ClearOutput();
            break;
        }
        case cmajor::service::ServiceMessageKind::outputMessage:
        {
            cmajor::service::OutputServiceMessage* message = static_cast<cmajor::service::OutputServiceMessage*>(serviceMessage.get());
            WriteOutput(message->Text());
            break;
        }
        case cmajor::service::ServiceMessageKind::buildResult:
        {
            cmajor::service::BuildResultMessage* message = static_cast<cmajor::service::BuildResultMessage*>(serviceMessage.get());
            HandleBuildResult(message->Result());
            break;
        }
        case cmajor::service::ServiceMessageKind::buildStoppedMessage:
        {
            HandleBuildStopped();
            break;
        }
        case cmajor::service::ServiceMessageKind::getDefinitionReply:
        {
            cmajor::service::GetDefinitionReplyServiceMessage* message = static_cast<cmajor::service::GetDefinitionReplyServiceMessage*>(serviceMessage.get());
            HandleGetDefinitionReply(message->Reply());
            break;
        }
        case cmajor::service::ServiceMessageKind::startDebugReply:
        {
            cmajor::service::StartDebugServiceReplyServiceMessage* message = static_cast<cmajor::service::StartDebugServiceReplyServiceMessage*>(serviceMessage.get());
            HandleStartDebugReply(message->Reply());
            break;
        }
        case cmajor::service::ServiceMessageKind::startError:
        {
            cmajor::service::StartDebugServiceErrorServiceMessage* message = static_cast<cmajor::service::StartDebugServiceErrorServiceMessage*>(serviceMessage.get());
            HandleStartDebugError(message->ErrorMessage()); 
            break;
        }
        case cmajor::service::ServiceMessageKind::execReply:
        {
            cmajor::service::ExecDebugServiceReplyServiceMessage* message = static_cast<cmajor::service::ExecDebugServiceReplyServiceMessage*>(serviceMessage.get());
            HandleExecReply(message->Reply());
            break;
        }
        case cmajor::service::ServiceMessageKind::debugError:
        {
            cmajor::service::DebugErrorServiceMessage* message = static_cast<cmajor::service::DebugErrorServiceMessage*>(serviceMessage.get());
            HandleDebugError(message->ErrorMessage());
            break;
        }
        case cmajor::service::ServiceMessageKind::breakReply:
        {
            //cmajor::service::BreakReplyServiceMessage* message = static_cast<cmajor::service::BreakReplyServiceMessage*>(serviceMessage.get());
            //HandleBreakReply(message->GetBreakReply()); TODO
            break;
        }
        case cmajor::service::ServiceMessageKind::deleteReply:
        {
            //cmajor::service::DeleteReplyServiceMessage* message = static_cast<cmajor::service::DeleteReplyServiceMessage*>(serviceMessage.get());
            //HandleDeleteReply(message->GetDeleteReply()); TODO
            break;
        }
        case cmajor::service::ServiceMessageKind::depthReply:
        {
            cmajor::service::DepthDebugServiceReplyServiceMessage* message = static_cast<cmajor::service::DepthDebugServiceReplyServiceMessage*>(serviceMessage.get());
            HandleDepthReply(message->DepthReply());
            break;
        }
        case cmajor::service::ServiceMessageKind::framesReply:
        {
            cmajor::service::FramesDebugServiceReplyServiceMessage* message = static_cast<cmajor::service::FramesDebugServiceReplyServiceMessage*>(serviceMessage.get());
            HandleFramesReply(message->FramesReply()); 
            break;
        }
        case cmajor::service::ServiceMessageKind::countReply:
        {
            cmajor::service::CountDebugServiceReplyServiceMessage* message = static_cast<cmajor::service::CountDebugServiceReplyServiceMessage*>(serviceMessage.get());
            HandleCountReply(message->CountReply()); 
            break;
        }
        case cmajor::service::ServiceMessageKind::evaluateChildReply:
        {
            cmajor::service::EvaluateChildDebugServiceReplyServiceMessage* message = static_cast<cmajor::service::EvaluateChildDebugServiceReplyServiceMessage*>(serviceMessage.get());
            HandleEvaluateChildReply(message->Reply()); 
            break;
        }
        case cmajor::service::ServiceMessageKind::evaluateReply:
        {
            cmajor::service::EvaluateDebugServiceReplyServiceMessage* message = static_cast<cmajor::service::EvaluateDebugServiceReplyServiceMessage*>(serviceMessage.get());
            HandleEvaluateReply(message->Reply(), message->RequestId());
            break;
        }
        case cmajor::service::ServiceMessageKind::targetRunning:
        {
            cmajor::service::TargetRunningServiceMessage* message = static_cast<cmajor::service::TargetRunningServiceMessage*>(serviceMessage.get());
            HandleTargetRunning(); 
            break;
        }
        case cmajor::service::ServiceMessageKind::targetInput:
        {
            cmajor::service::TargetInputServiceMessage* message = static_cast<cmajor::service::TargetInputServiceMessage*>(serviceMessage.get());
            HandleTargetInput(); 
            break;
        }
        case cmajor::service::ServiceMessageKind::targetOutput:
        {
            cmajor::service::TargetOutputServiceMessage* message = static_cast<cmajor::service::TargetOutputServiceMessage*>(serviceMessage.get());
            HandleTargetOutputRequest(message->GetOutputRequest());
            break;
        }
        case cmajor::service::ServiceMessageKind::debugServiceStopped:
        {
            cmajor::service::DebugServiceStoppedServiceMessage* message = static_cast<cmajor::service::DebugServiceStoppedServiceMessage*>(serviceMessage.get());
            HandleDebugServiceStopped(); 
            break;
        }
        case cmajor::service::ServiceMessageKind::processTerminated:
        {
            HandleProcessTerminated(); 
            break;
        }
        case cmajor::service::ServiceMessageKind::runServiceStopped:
        {
            HandleRunServiceStopped();
            break;
        }
/*
        case cmajor::service::ServiceMessageKind::loadEditModuleReply:
        {
            cmajor::service::LoadEditModuleReplyServiceMessage* message = static_cast<cmajor::service::LoadEditModuleReplyServiceMessage*>(serviceMessage.get());
            HandleLoadEditModuleReply(message->Reply());
            break;
        }
        case cmajor::service::ServiceMessageKind::loadEditModuleError:
        {
            cmajor::service::LoadEditModuleErrorServiceMessage* message = static_cast<cmajor::service::LoadEditModuleErrorServiceMessage*>(serviceMessage.get());
            HandleLoadEditModuleError(message->Error());
            break;
        }
        case cmajor::service::ServiceMessageKind::resetEditModuleCacheReply:
        {
            cmajor::service::ResetEditModuleCacheReplyServiceMessage* message = static_cast<cmajor::service::ResetEditModuleCacheReplyServiceMessage*>(serviceMessage.get());
            HandleResetEditModuleCacheReply(message->Reply());
            break;
        }
        case cmajor::service::ServiceMessageKind::resetEditModuleCacheError:
        {
            cmajor::service::ResetEditModuleCacheErrorServiceMessage* message = static_cast<cmajor::service::ResetEditModuleCacheErrorServiceMessage*>(serviceMessage.get());
            HandleResetEditModuleCacheError(message->Error());
            break;
        }
        case cmajor::service::ServiceMessageKind::parseSourceReply:
        {
            cmajor::service::ParseSourceReplyServiceMessage* message = static_cast<cmajor::service::ParseSourceReplyServiceMessage*>(serviceMessage.get());
            HandleParseSourceReply(message->Reply());
            break;
        }
        case cmajor::service::ServiceMessageKind::parseSourceError:
        {
            cmajor::service::ParseSourceErrorServiceMessage* message = static_cast<cmajor::service::ParseSourceErrorServiceMessage*>(serviceMessage.get());
            HandleParseSourceError(message->Error());
            break;
        }
        case cmajor::service::ServiceMessageKind::getCCListReply:
        {
            cmajor::service::GetCCListReplyServiceMessage* message = static_cast<cmajor::service::GetCCListReplyServiceMessage*>(serviceMessage.get());
            HandleGetCCListReply(message->Reply());
            break;
        }
        case cmajor::service::ServiceMessageKind::getCCListError:
        {
            cmajor::service::GetCCListErrorServiceMessage* message = static_cast<cmajor::service::GetCCListErrorServiceMessage*>(serviceMessage.get());
            HandleGetCCListError(message->Error());
            break;
        }
        case cmajor::service::ServiceMessageKind::getParamHelpListReply:
        {
            cmajor::service::GetParamHelpListReplyServiceMessage* message = static_cast<cmajor::service::GetParamHelpListReplyServiceMessage*>(serviceMessage.get());
            HandleGetParamHelpListReply(message->Reply());
            break;
        }
        case cmajor::service::ServiceMessageKind::getParamHelpListError:
        {
            cmajor::service::GetParamHelpListErrorServiceMessage* message = static_cast<cmajor::service::GetParamHelpListErrorServiceMessage*>(serviceMessage.get());
            HandleGetParamHelpListError(message->Error());
            break;
        }
*/
        case cmajor::service::ServiceMessageKind::clearDebugLog:
        {
            GetDebugLog()->Clear();
            break;
        }
        case cmajor::service::ServiceMessageKind::debugLogMessage:
        {
            cmajor::service::DebugLogMessage* message = static_cast<cmajor::service::DebugLogMessage*>(serviceMessage.get());
            GetDebugLog()->WriteLine(message->Text());
            break;
        }
        }
    }
}

void MainWindow::ClearOutput()
{
    GetOutputLogView()->Clear();
}

void MainWindow::WriteOutput(const std::string& text)
{
    GetOutputLogView()->WriteLine(text);
}

void MainWindow::OpenProject(const std::string& filePath)
{
    try
    {
        std::vector<std::unique_ptr<std::u32string>> contents;
        std::unique_ptr<cmajor::ast::Solution> solution;
        if (!filePath.empty() && std::filesystem::exists(filePath))
        {
            std::string ext = util::Path::GetExtension(filePath);
            if (ext == ".cms")
            {
                solution = ReadSolution(filePath, contents);
            }
            else if (ext == ".cmp")
            {
                std::string solutionFilePath = util::Path::ChangeExtension(filePath, ".cms");
                if (std::filesystem::exists(solutionFilePath))
                {
                    solution = ReadSolution(solutionFilePath, contents);
                }
                else
                {
                    std::unique_ptr<cmajor::ast::Project> project = ReadProject(filePath, contents);
                    solution.reset(new cmajor::ast::Solution(project->Name(), solutionFilePath));
                    solution->SetActiveProject(project.get());
                    solution->AddProject(std::move(project));
                    solution->Save();
                }
            }
            else
            {
                throw std::runtime_error("file path '" + filePath + "' has invalid extension (not .cmp or .cms)");
            }
        }
        else
        {
            throw std::runtime_error("file path '" + filePath + "' is empty or does not exist");
        }
        codeTabControl->CloseAllTabPages();
        solutionData.reset(new SolutionData(std::move(solution), solutionExplorer));
        SetIDEState();
        cmajor::ast::Solution* sln = solutionData->GetSolution();
        if (sln)
        {
            AddRecentSolution(util::ToUtf8(sln->Name()), sln->FilePath());
            SaveConfiguration();
            LoadEditModule();
            backend = cmajor::ast::BackEndStr(sln->ActiveBackEnd());
            config = sln->ActiveConfig();
            optLevel = sln->ActiveOptLevel();
        }
        SetState(MainWindowState::idle);
    }
    catch (const std::exception& ex)
    {
        SetState(MainWindowState::idle);
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::SetIDEState()
{
    try
    {
        for (const std::string& openFile : solutionData->OpenFiles())
        {
            AddEditor(openFile);
        }
        if (!solutionData->CurrentOpenFile().empty())
        {
            wing::TabPage* tabPage = codeTabControl->GetTabPageByKey(solutionData->CurrentOpenFile());
            if (tabPage)
            {
                tabPage->Select();
            }
            else
            {
                AddEditor(solutionData->CurrentOpenFile());
            }
        }
        if (solutionData->CurrentCursorLine() > 0)
        {
            wing::TabPage* tabPage = codeTabControl->GetTabPageByKey(solutionData->CurrentOpenFile());
            if (tabPage)
            {
                cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
                if (editor)
                {
                    wing::TextView* textView = editor->GetTextView();
                    if (textView)
                    {
                        int line = solutionData->CurrentCursorLine();
                        if (line > 0 && line <= textView->Lines().size())
                        {
                            textView->SetCaretLineCol(line, 1 + textView->LineNumberFieldLength());
                            textView->ScrollToCaret();
                            textView->EnsureLineVisible(line);
                        }
                    }
                }
            }
        }
        for (const std::string& expandedProject : solutionData->ExpandedProjects())
        {
            wing::TreeViewNode* projectNode = solutionData->GetProjectNodeByName(expandedProject);
            if (projectNode)
            {
                projectNode->Expand();
            }
        }
        callStackOpen = solutionData->CallStackOpen();
        if (callStackOpen)
        {
            GetCallStackView();
        }
        localsViewOpen = solutionData->LocalsViewOpen();
        if (localsViewOpen)
        {
            GetLocalsView();
        }
        outputTabPage->Select();
        SetFocusToEditor();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::AddEditor(const std::string& filePath)
{
    if (!std::filesystem::exists(filePath)) return;
    SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(filePath);
    if (data)
    {
        std::string ext = util::Path::GetExtension(data->fileName);
        if (ext == ".cm")
        {
            AddCmajorEditor(data->fileName, data->key, data->filePath, data->project);
        }
        else if (ext == ".xml")
        {
            AddResourceFileEditor(data->fileName, data->key, data->filePath, data->project);
        }
        else
        {
            AddTextFileEditor(data->fileName, data->key, data->filePath, data->project);
        }
    }
    else
    {
        std::string ext = util::Path::GetExtension(filePath);
        if (ext == ".cm")
        {
            AddCmajorEditor(util::Path::GetFileName(filePath), filePath, filePath, nullptr);
        }
        else if (ext == ".xml")
        {
            AddResourceFileEditor(util::Path::GetFileName(filePath), filePath, filePath, nullptr);
        }
        else
        {
            AddTextFileEditor(util::Path::GetFileName(filePath), filePath, filePath, nullptr);
        }
    }
}

void MainWindow::HandleBuildResult(const cmajor::info::bs::BuildResult& buildResult)
{
    StopBuilding();
    if (buildResult.success)
    {
        if (!buildResult.warnings.empty())
        {
            cmajor::view::WarningsView* view = GetWarningsView();
            view->Clear();
            view->SetWarnings(std::move(buildResult.warnings));
        }
        if (debugRequest)
        {
            StartDebugging();
            cmajor::service::PutRequest(debugRequest.release());
        }
        else if (getDefinitionRequest)
        {
            cmajor::service::PutRequest(getDefinitionRequest.release());
        }
    }
    else
    {
        debugRequest.reset();
        cmajor::service::PutOutputServiceMessage("build unsuccessful");
        if (!buildResult.errors.empty())
        {
            cmajor::view::ErrorView* view = GetErrorView();
            view->Clear();
            view->SetErrors(std::move(buildResult.errors));
        }
    }
    SetFocusToEditor();
}

void MainWindow::HandleBuildStopped()
{
    StopBuilding();
    SetFocusToEditor();
    cmajor::service::PutOutputServiceMessage("build stopped");
}

void MainWindow::HandleStartDebugReply(cmajor::debugger::Reply* reply)
{
    UpdateCurrentDebugStrip();
    startDebugging = false;
    startDebuggingMenuItem->SetText("Continue");
    startDebuggingToolButton->SetToolTip("Continue (F5)");
    cmajor::service::PutOutputServiceMessage("debugging started");
    if (localsView)
    {
        ClearLocals();
        UpdateLocals();
    }
}

void MainWindow::HandleStartDebugError(const std::string& error)
{
    UpdateCurrentDebugStrip();
    startDebugging = false;
    cmajor::service::PutOutputServiceMessage("failed to start debugging: " + error);
    cmajor::service::PutRequest(new cmajor::service::StopDebugServiceRequest());
}

void MainWindow::HandleExecReply(cmajor::debugger::Reply* reply)
{
    cmajor::debugger::StopReason stopReason = reply->GetStopReason();
    if (stopReason != cmajor::debugger::StopReason::unknown)
    {
        if (stopReason == cmajor::debugger::StopReason::exited)
        {
            cmajor::service::PutOutputServiceMessage(cmajor::debugger::StopReasonStr(stopReason) + " with code " + std::to_string(reply->ExitCode()));
        }
        else
        {
            cmajor::service::PutOutputServiceMessage(cmajor::debugger::StopReasonStr(stopReason));
        }
        switch (stopReason)
        {
            case cmajor::debugger::StopReason::exitedNormally:
            case cmajor::debugger::StopReason::exited:
            case cmajor::debugger::StopReason::exitedSignaled:
            {
                programRunning = false;
                SetState(MainWindowState::debugging);
                cmajor::service::PutRequest(new cmajor::service::StopDebugServiceRequest());
                break;
            }
            default:
            {
                SetState(MainWindowState::debugging);
                UpdateCallStack(true);
                break;
            }
        }
        cmajor::info::db::Location loc = reply->GetLocation();
        if (loc.IsValid())
        {
            HandleLocation(loc, true, false);
        }
    }
    UpdateLocals();
}

void MainWindow::HandleDebugError(const std::string& error)
{
    cmajor::service::PutOutputServiceMessage("command failed: " + error);
}

void MainWindow::HandleBreakReply(const cmajor::info::db::BreakReply& breakReply)
{
    UpdateCurrentDebugStrip();
    cmajor::service::PutOutputServiceMessage("breakpoint added");
}

void MainWindow::HandleDeleteReply(const cmajor::info::db::DeleteReply& deleteReply)
{
    UpdateCurrentDebugStrip();
    cmajor::service::PutOutputServiceMessage("breakpoint removed");
}

void MainWindow::HandleDepthReply(const cmajor::info::db::DepthReply& depthReply)
{
    callStackDepth = depthReply.depth;
    UpdateCallStack(false);
}

void MainWindow::HandleFramesReply(const cmajor::info::db::FramesReply& framesReply)
{
    GetCallStackView()->SetFrameRange(framesReply.frames);
    if (signalReceived)
    {
        GetOutputLogView()->Select();
    }
}

void MainWindow::HandleEvaluateReply(const cmajor::info::db::EvaluateReply& evaluateReply, int requestId)
{
    if (evaluateReply.success)
    {
        if (requestId >= 0 && requestId < expressionEvaluateRequests.size())
        {
            const ExpressionEvaluateRequest& evaluateRequest = expressionEvaluateRequests[requestId];
            toolTipWindow->Hide();
            std::string typeName;
            if (!evaluateReply.result.dynamicType.name.empty())
            {
                typeName = "[" + evaluateReply.result.dynamicType.name + "]";
            }
            else if (!evaluateReply.result.staticType.name.empty())
            {
                typeName = evaluateReply.result.staticType.name;
            }
            toolTipWindow->SetText(evaluateRequest.expression + " : " + typeName + " = " + evaluateReply.result.value);
            toolTipWindow->MeasureExtent();
            wing::Point loc = evaluateRequest.screenLoc;
            loc = ScreenToClient(loc);
            loc.Y = loc.Y + toolTipWindow->GetSize().Height;
            toolTipWindow->SetLocation(loc);
            toolTipWindow->BringToFront();
            toolTipWindow->Show();
            toolTipWindow->Invalidate();
            toolTipWindow->Update();
            SetTimer(toolTipTimerId, toolTipShowPeriod);
        }
        else
        {
            cmajor::service::PutOutputServiceMessage("invalid evaluate request id");
        }
    }
    else
    {
        cmajor::service::PutOutputServiceMessage("evaluate request failed: " + evaluateReply.error);
    }
}

void MainWindow::HandleCountReply(const cmajor::info::db::CountReply& countReply)
{
    cmajor::view::LocalsView* view = GetLocalsView();
    if (view)
    {
        view->SetLocalCount(countReply.count);
        UpdateLocals();
    }
}

void MainWindow::HandleEvaluateChildReply(const cmajor::info::db::EvaluateChildReply& evaluateChildReply)
{
    if (evaluateChildReply.success)
    {
        cmajor::view::LocalsView* view = GetLocalsView();
        if (view)
        {
            view->SetChildResults(evaluateChildReply.results);
            UpdateLocals();
        }
    }
    else
    {
        cmajor::service::PutOutputServiceMessage("evaluate child request failed: " + evaluateChildReply.error);
    }
}

void MainWindow::HandleLocation(const cmajor::info::db::Location& location, bool saveLocation, bool setSelection)
{
    try
    {
        if (location.line > 0)
        {
            lineStatusBarItem->SetText(std::to_string(location.line));
        }
        else
        {
            lineStatusBarItem->SetText(std::string());
        }
        columnStatusBarItem->SetText(std::string());
        if (saveLocation)
        {
            savedLocation = location;
        }
        if (location.file.empty()) return;
        cmajor::view::Editor* editor = nullptr;
        const std::string& filePath = location.file;
        wing::TabPage* tabPage = codeTabControl->GetTabPageByKey(filePath); 
        if (tabPage)
        {
            tabPage->Select();
            editor = GetEditorByTabPage(tabPage);
        }
        else
        {
            SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(filePath);
            if (data)
            {
                editor = AddCmajorEditor(data->fileName, data->key, data->filePath, data->project);
            }
        }
        if (!editor)
        {
            editor = AddCmajorEditor(util::Path::GetFileName(filePath), filePath, filePath, nullptr);
        }
        if (editor->IsCmajorEditor())
        {
            cmajor::view::CmajorEditor* cmajorEditor = static_cast<cmajor::view::CmajorEditor*>(editor);
            cmajor::view::DebugStrip* debugStrip = cmajorEditor->GetDebugStrip();
            wing::SourceSpan debugLocation;
            debugLocation.line = location.line;
            debugLocation.scol = location.scol; 
            debugLocation.ecol = location.ecol; 
            if (!setSelection)
            {
                debugStrip->SetDebugLocation(debugLocation);
            }
        }
        wing::TextView* textView = editor->GetTextView();
        if (textView)
        {
            textView->EnsureLineVisible(location.line);
            textView->SetCaretLineCol(location.line, location.scol);
            if (setSelection)
            {
                ResetSelections();
                soul::ast::SourcePos start(location.line, location.scol); 
                soul::ast::SourcePos end(location.line, location.ecol); 
                wing::Selection selection;
                selection.start = start; 
                selection.end = end; 
                textView->SetSelection(selection);
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::HandleTargetState(const cmajor::debugger::TargetState& state)
{
    programRunning = false;
    SetState(MainWindowState::debugging);
    if (state.stopReason == "exited-normally")
    {
        cmajor::service::PutRequest(new cmajor::service::StopDebugServiceRequest());
        cmajor::service::PutOutputServiceMessage("process exited normally");
    }
    else if (state.stopReason == "exited")
    {
        std::string message = "process exited";
        message.append(", exit code=" + state.exitCode);
        cmajor::service::PutRequest(new cmajor::service::StopDebugServiceRequest());
        cmajor::service::PutOutputServiceMessage(message);
    }
    else if (state.stopReason == "signal-received")
    {
        std::string message = "process received signal";
        message.append(", signal=" + state.signalName + ", meaning=" + state.signalMeaning);
        signalReceived = true;
        cmajor::service::PutOutputServiceMessage(message);
        GetCallStackView();
        UpdateCallStack(true);
    }
    else if (state.stopReason == "breakpoint-hit")
    {
        UpdateCallStack(true);
        if (!state.breakpointId.empty())
        {
            std::string message = "breakpoint hit";
            message.append(", breakpoint=" + state.breakpointId);
            cmajor::service::PutOutputServiceMessage(message);
        }
    }
    else if (state.stopReason == "end-stepping-range")
    {
        UpdateCallStack(true);
    }
}

void MainWindow::HandleTargetRunning()
{
    ResetDebugLocations();
    programRunning = true;
    SetState(MainWindowState::debugging);
}

void MainWindow::HandleTargetInput()
{
    GetConsole()->StartReadLine();
}

void MainWindow::HandleTargetOutputRequest(const cmajor::debugger::OutputRequest& outputRequest)
{
    GetConsole()->Write(outputRequest.handle, outputRequest.output);
}

void MainWindow::ConsoleInputReady()
{
    wing::Console* console = GetConsole();
    if (state == MainWindowState::debugging)
    {
        std::u32string inputLine = console->GetInputLine();
        if (inputLine.empty())
        {
            if (console->Eof())
            {
                cmajor::service::PutRequest(new cmajor::service::SetDebugServiceProgramEofRequest());
                return;
            }
        }
        cmajor::service::PutRequest(new cmajor::service::PutDebugServiceProgramInputLineRequest(util::ToUtf8(inputLine))); 
    }
    else if (state == MainWindowState::running)
    {
        std::u32string inputLine = console->GetInputLine();
        if (inputLine.empty())
        {
            if (console->Eof())
            {
                cmajor::service::PutRequest(new cmajor::service::SetRunServiceProgramEofRequest());
                return;
            }
        }
        cmajor::service::PutRequest(new cmajor::service::PutRunServiceProgramInputLineRequest(util::ToUtf8(inputLine)));
    }
}

void MainWindow::HandleDebugServiceStopped()
{
    StopDebugging();
    SetFocusToEditor();
}

void MainWindow::HandleProcessTerminated()
{
    cmajor::service::PutRequest(new cmajor::service::StopRunServiceRequest()); 
}

void MainWindow::HandleRunServiceStopped()
{
    StopRunning();
    SetFocusToEditor();
}

/*
void MainWindow::HandleLoadEditModuleReply(const LoadEditModuleReply& loadEditModuleReply)
{
    try
    {
        if (loadEditModuleReply.result == "error")
        {
            PutOutputServiceMessage("edit module loading error: " + loadEditModuleReply.error);
            ccState = CCState::error;
            if (ccTimerRunning)
            {
                KillTimer(ccTimerId);
            }
        }
        else
        {
            PutOutputServiceMessage("edit module " + loadEditModuleReply.key + " " + loadEditModuleReply.result);
            ccState = CCState::editModuleLoaded;
            const Options& options = GetOptions();
            if (options.codeCompletion)
            {
                ccTimerRunning = true;
                SetTimer(ccTimerId, options.parsingFrequency);
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::HandleLoadEditModuleError(const std::string& error)
{
    ccState = CCState::error;
}

void MainWindow::HandleResetEditModuleCacheReply(const ResetEditModuleCacheReply& resetEditModuleCacherReply)
{
    if (resetEditModuleCacherReply.result == "error")
    {
        PutOutputServiceMessage("error resetting edit module cache: " + resetEditModuleCacherReply.error);
    }
    else
    {
        PutOutputServiceMessage("edit module cache reset");
    }
}

void MainWindow::HandleResetEditModuleCacheError(const std::string& error)
{
    PutOutputServiceMessage("error resetting edit module cache: " + error);
}

void MainWindow::HandleParseSourceReply(const ParseSourceReply& parseSourceReply)
{
    if (parseSourceReply.ok)
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            textView->SetCCCat(std::string());
            ruleContext = parseSourceReply.ruleContext;
        }
    }
    else
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            textView->ResetCCActive();
            textView->ResetCCOpen();
        }
        codeCompletionListView->Hide();
    }
}

void MainWindow::HandleParseSourceError(const std::string& error)
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        wing::TextView* textView = editor->GetTextView();
        textView->ResetCCActive();
        textView->ResetCCOpen();
    }
    codeCompletionListView->Hide();
}

void MainWindow::HandleGetCCListReply(const GetCCListReply& getCCListReply)
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            if (getCCListReply.ok)
            {
                std::vector<CCEntry> entries = ParseCCList(getCCListReply.ccList);
                if (entries.empty())
                {
                    codeCompletionListView->Hide();
                    ccCanSelect = false;
                }
                else
                {
                    wing::Point ccPos = textView->CCPos();
                    wing::Point screenPos = textView->ClientToScreen(ccPos);
                    wing::Point ccListPos = ScreenToClient(screenPos);
                    codeCompletionListView->SetLocation(ccListPos);
                    codeCompletionListView->SetContent(entries);
                    codeCompletionListView->Show();
                    codeCompletionListView->BringToFront();
                    codeCompletionListView->Invalidate();
                    codeCompletionListView->Update();
                    codeCompletionListView->SetDefaultSize();
                    textView->SetCCOpen();
                    ccCanSelect = true;
                }
            }
            else
            {
                ccCanSelect = false;
                PutOutputServiceMessage("code completion error: " + getCCListReply.error);
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::HandleGetCCListError(const std::string& error)
{
    PutOutputServiceMessage("code completion error: " + error);
}

void MainWindow::HandleGetParamHelpListReply(const GetParamHelpListReply& getParamHelpListReply)
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            if (getParamHelpListReply.ok)
            {
                std::vector<std::string> paramHelpList = ParseParamHelpList(getParamHelpListReply.list);
                if (paramHelpList.empty())
                {
                    paramHelpView->Hide();
                }
                else
                {
                    wing::Point ccPos = textView->CCPos();
                    wing::Point screenPos = textView->ClientToScreen(ccPos);
                    wing::Point paramHelpListPos = ScreenToClient(screenPos);
                    paramHelpView->SetLocation(paramHelpListPos);
                    paramHelpView->SetContent(paramHelpList);
                    paramHelpView->Show();
                    paramHelpView->BringToFront();
                    paramHelpView->Invalidate();
                    paramHelpView->Update();
                    textView->SetParamHelpOpen();
                }
            }
            else
            {
                PutOutputServiceMessage("code completion error: " + getParamHelpListReply.error);
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::HandleGetParamHelpListError(const std::string& error)
{
    PutOutputServiceMessage("code completion error: " + error);
}
*/

void MainWindow::HandleGetDefinitionReply(const cmajor::info::bs::GetDefinitionReply& getDefinitionReply)
{
    if (getDefinitionReply.succeeded)
    {
        cmajor::info::bs::DefinitionSourceLocation currentLocation = CurrentLocation();
        locations.AddLocation(currentLocation);
        locations.AddLocation(getDefinitionReply.definitionLocation);
        locations.GotoPreviousLocation(currentLocation);
    }
    else
    {
        cmajor::service::PutOutputServiceMessage(getDefinitionReply.error);
    }
}

void MainWindow::HandleGetDefinitionError(const std::string& getDefinitionError)
{
    StopBuilding();
    cmajor::service::PutOutputServiceMessage("goto definition command unsuccessful");
}

void MainWindow::SetState(MainWindowState state_)
{
    state = state_;
    newProjectMenuItem->Disable();
    openProjectMenuItem->Disable();
    closeSolutionMenuItem->Disable();
    saveMenuItem->Disable();
    saveAllMenuItem->Disable();
    exitMenuItem->Disable();
    copyMenuItem->Disable();
    cutMenuItem->Disable();
    pasteMenuItem->Disable();
    undoMenuItem->Disable();
    redoMenuItem->Disable();
    gotoMenuItem->Disable();
    searchMenuItem->Disable();
    optionsMenuItem->Disable();
    callStackMenuItem->Disable();
    localsMenuItem->Disable();
    errorsMenuItem->Disable();
    searchResultsMenuItem->Disable();
    portMapMenuItem->Disable();
    buildSolutionMenuItem->Disable();
    rebuildSolutionMenuItem->Disable();
    cleanSolutionMenuItem->Disable();
    buildActiveProjectMenuItem->Disable();
    rebuildActiveProjectMenuItem->Disable();
    cleanActiveProjectMenuItem->Disable();
    startDebuggingMenuItem->Disable();
    startWithoutDebuggingMenuItem->Disable();
    terminateProcessMenuItem->Disable();
    stopDebuggingMenuItem->Disable();
    showNextStatementMenuItem->Disable();
    stepOverMenuItem->Disable();
    stepIntoMenuItem->Disable();
    stepOutMenuItem->Disable();
    toggleBreakpointMenuItem->Disable();
    programArgumentsMenuItem->Disable();
    closeAllTabsMenuItem->Disable();
    closeExternalTabsMenuItem->Disable();
    homepageMenuItem->Disable();
    localDocumentationMenuItem->Disable();
    aboutMenuItem->Disable();
    prevToolButton->Disable();
    nextToolButton->Disable();
    saveToolButton->Disable();
    saveAllToolButton->Disable();
    cppToolButton->Disable();
    llvmToolButton->Disable();
    masmToolButton->Disable();
    cmToolButton->Disable();
    sbinToolButton->Disable();
    debugToolButton->Disable();
    releaseToolButton->Disable();
    optLevelZeroToolButton->Disable();
    optLevelOneToolButton->Disable();
    optLevelTwoToolButton->Disable();
    optLevelThreeToolButton->Disable();
    buildSolutionToolButton->Disable();
    buildActiveProjectToolButton->Disable();
    stopBuildToolButton->Disable();
    startDebuggingToolButton->Disable();
    stopDebuggingToolButton->Disable();
    showNextStatementToolButton->Disable();
    stepOverToolButton->Disable();
    stepIntoToolButton->Disable();
    stepOutToolButton->Disable();

    if (backend == "llvm")
    {
        llvmToolButton->SetState(wing::ToolButtonState::pressed);
        cppToolButton->SetState(wing::ToolButtonState::normal);
        masmToolButton->SetState(wing::ToolButtonState::normal);
        cmToolButton->SetState(wing::ToolButtonState::normal);
        sbinToolButton->SetState(wing::ToolButtonState::normal);
    }
    if (backend == "cpp")
    {
        cppToolButton->SetState(wing::ToolButtonState::pressed);
        llvmToolButton->SetState(wing::ToolButtonState::normal);
        masmToolButton->SetState(wing::ToolButtonState::normal);
        cmToolButton->SetState(wing::ToolButtonState::normal);
        sbinToolButton->SetState(wing::ToolButtonState::normal);
    }
    if (backend == "masm")
    {
        llvmToolButton->SetState(wing::ToolButtonState::normal);
        cppToolButton->SetState(wing::ToolButtonState::normal);
        masmToolButton->SetState(wing::ToolButtonState::pressed);
        cmToolButton->SetState(wing::ToolButtonState::normal);
        sbinToolButton->SetState(wing::ToolButtonState::normal);
    }
    if (backend == "cm")
    {
        llvmToolButton->SetState(wing::ToolButtonState::normal);
        cppToolButton->SetState(wing::ToolButtonState::normal);
        masmToolButton->SetState(wing::ToolButtonState::normal);
        cmToolButton->SetState(wing::ToolButtonState::pressed);
        sbinToolButton->SetState(wing::ToolButtonState::normal);
    }
    if (backend == "sbin")
    {
        llvmToolButton->SetState(wing::ToolButtonState::normal);
        cppToolButton->SetState(wing::ToolButtonState::normal);
        masmToolButton->SetState(wing::ToolButtonState::normal);
        cmToolButton->SetState(wing::ToolButtonState::normal);
        sbinToolButton->SetState(wing::ToolButtonState::pressed);
    }

    if (config == "debug")
    {
        debugToolButton->SetState(wing::ToolButtonState::pressed);
        releaseToolButton->SetState(wing::ToolButtonState::normal);
        optLevelZeroToolButton->SetState(wing::ToolButtonState::pressed);
        optLevelOneToolButton->SetState(wing::ToolButtonState::normal);
        optLevelTwoToolButton->SetState(wing::ToolButtonState::normal);
        optLevelThreeToolButton->SetState(wing::ToolButtonState::normal);
    }
    if (config == "release")
    {
        releaseToolButton->SetState(wing::ToolButtonState::pressed);
        debugToolButton->SetState(wing::ToolButtonState::normal);
        switch (optLevel)
        {
            case 0:
            {
                optLevelZeroToolButton->SetState(wing::ToolButtonState::pressed);
                optLevelOneToolButton->SetState(wing::ToolButtonState::normal);
                optLevelTwoToolButton->SetState(wing::ToolButtonState::normal);
                optLevelThreeToolButton->SetState(wing::ToolButtonState::normal);
                break;
            }
            case 1:
            {
                optLevelOneToolButton->SetState(wing::ToolButtonState::pressed);
                optLevelZeroToolButton->SetState(wing::ToolButtonState::normal);
                optLevelTwoToolButton->SetState(wing::ToolButtonState::normal);
                optLevelThreeToolButton->SetState(wing::ToolButtonState::normal);

                break;
            }
            case 2:
            {
                optLevelTwoToolButton->SetState(wing::ToolButtonState::pressed);
                optLevelZeroToolButton->SetState(wing::ToolButtonState::normal);
                optLevelOneToolButton->SetState(wing::ToolButtonState::normal);
                optLevelThreeToolButton->SetState(wing::ToolButtonState::normal);
                break;
            }
            case 3:
            {
                optLevelThreeToolButton->SetState(wing::ToolButtonState::pressed);
                optLevelZeroToolButton->SetState(wing::ToolButtonState::normal);
                optLevelOneToolButton->SetState(wing::ToolButtonState::normal);
                optLevelTwoToolButton->SetState(wing::ToolButtonState::normal);
                break;
            }
        }
    }

    bool solutionOpen = solutionData.get() != nullptr;
    cmajor::ast::Project* activeProject = nullptr;
    if (solutionOpen)
    {
        activeProject = solutionData->GetSolution()->ActiveProject();
    }

    // always on:

    exitMenuItem->Enable();
    optionsMenuItem->Enable();
    searchResultsMenuItem->Enable();
    callStackMenuItem->Enable();
    localsMenuItem->Enable();
    portMapMenuItem->Enable();
    closeAllTabsMenuItem->Enable();
    closeExternalTabsMenuItem->Enable();
    homepageMenuItem->Enable();
    localDocumentationMenuItem->Enable();
    aboutMenuItem->Enable();

    switch (state)
    {
    case MainWindowState::idle:
    {
        newProjectMenuItem->Enable();
        openProjectMenuItem->Enable();

        bool buildInProgress = cmajor::service::BuildInProgress();

        if (solutionOpen)
        {
            closeSolutionMenuItem->Enable();
            saveAllMenuItem->Enable();
            searchMenuItem->Enable();
            errorsMenuItem->Enable();
            buildSolutionMenuItem->Enable();
            rebuildSolutionMenuItem->Enable();
            cleanSolutionMenuItem->Enable();
            buildActiveProjectMenuItem->Enable();
            rebuildActiveProjectMenuItem->Enable();
            cleanActiveProjectMenuItem->Enable();
            startWithoutDebuggingMenuItem->Enable();
            if (activeProject && IsProgramTarget(activeProject->GetTarget()))
            {
                programArgumentsMenuItem->Enable();
            }
            saveAllToolButton->Enable();
            cppToolButton->Enable();
            llvmToolButton->Enable();
            masmToolButton->Enable();
            cmToolButton->Enable();
            sbinToolButton->Enable();
            debugToolButton->Enable();
            releaseToolButton->Enable();
            if (config == "release")
            {
                optLevelZeroToolButton->Enable();
                optLevelOneToolButton->Enable();
                optLevelTwoToolButton->Enable();
                optLevelThreeToolButton->Enable();
            }
            buildSolutionToolButton->Enable();
            buildActiveProjectToolButton->Enable();
            toggleBreakpointMenuItem->Enable();
            if (config == "debug" && backend == "cpp")
            {
                startDebuggingMenuItem->Enable();
                startDebuggingToolButton->Enable();
                stepOverMenuItem->Enable();
                stepOverToolButton->Enable();
                stepIntoMenuItem->Enable();
                stepIntoToolButton->Enable();
            }
            if (buildInProgress)
            {
                stopBuildToolButton->Enable();
            }
        }
        break;
    }
    case MainWindowState::building:
    {
        stopBuildToolButton->Enable();
        break;
    }
    case MainWindowState::debugging:
    {
        stopDebuggingMenuItem->Enable();
        stopDebuggingToolButton->Enable();
        if (programRunning || startDebugging)
        {
            startDebuggingMenuItem->Disable();
            startDebuggingToolButton->Disable();
            stepOverMenuItem->Disable();
            stepOverToolButton->Disable();
            stepIntoMenuItem->Disable();
            stepIntoToolButton->Disable();
            stepOutMenuItem->Disable();
            stepOutToolButton->Disable();
            toggleBreakpointMenuItem->Disable();
        }
        else
        {
            startDebuggingMenuItem->Enable();
            startDebuggingToolButton->Enable();
            stepOverMenuItem->Enable();
            stepOverToolButton->Enable();
            stepIntoMenuItem->Enable();
            stepIntoToolButton->Enable();
            stepOutMenuItem->Enable();
            stepOutToolButton->Enable();
            toggleBreakpointMenuItem->Enable();
        }
        if (!savedLocation.file.empty())
        {
            showNextStatementMenuItem->Enable();
            showNextStatementToolButton->Enable();
        }
        else
        {
            showNextStatementMenuItem->Disable();
            showNextStatementToolButton->Disable();
        }
        break;
    }
    case MainWindowState::running:
    {
        terminateProcessMenuItem->Enable();
        break;
    }
    }
    SetEditorState();
}

void MainWindow::SetEditorState()
{
    bool editorOpen = !codeTabControl->TabPages().IsEmpty();
    if (editorOpen)
    {
        gotoMenuItem->Enable();
        cmajor::view::Editor* currentEditor = CurrentEditor();
        if (currentEditor)
        {
            wing::TextView* textView = currentEditor->GetTextView();
            if (textView)
            {
                std::string editorReadWriteText;
                if (textView->IsReadOnly())
                {
                    editorReadWriteText = "R/O";
                }
                else
                {
                    editorReadWriteText = "R/W";
                }
                editorReadWriteIndicatorStatusBarItem->SetText(editorReadWriteText);
                std::string editorDirtyText;
                if (textView->IsDirty())
                {
                    editorDirtyText = "*";
                }
                else
                {
                    editorDirtyText = std::string();
                }
                editorDirtyIndicatorStatusBarItem->SetText(editorDirtyText);
                sourceFilePathStatusBarItem->SetText(textView->FilePath());
                if (!textView->IsSelectionEmpty())
                {
                    copyMenuItem->Enable();
                    cutMenuItem->Enable();
                }
                else
                {
                    copyMenuItem->Disable();
                    cutMenuItem->Disable();
                }
                if (textView->CaretLine() > 0)
                {
                    lineStatusBarItem->SetText(std::to_string(textView->CaretLine()));
                }
                else
                {
                    lineStatusBarItem->SetText(std::string());
                }
                if (textView->CaretColumn() > 0)
                {
                    columnStatusBarItem->SetText(std::to_string(textView->CaretColumn()));
                }
                else
                {
                    columnStatusBarItem->SetText(std::string());
                }
            }
        }
    }
    else
    {
        editorReadWriteIndicatorStatusBarItem->SetText(std::string());
        editorDirtyIndicatorStatusBarItem->SetText(std::string());
        sourceFilePathStatusBarItem->SetText(std::string());
        lineStatusBarItem->SetText(std::string());
        columnStatusBarItem->SetText(std::string());
    }
}

void MainWindow::SetEditorsReadOnly()
{
    Component* child = codeTabControl->TabPages().FirstChild();
    while (child)
    {
        if (child->IsTabPage())
        {
            wing::TabPage* tabPage = static_cast<wing::TabPage*>(child);
            cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
            if (editor)
            {
                wing::TextView* textView = editor->GetTextView();
                if (textView)
                {
                    textView->SetReadOnly();
                }
            }
        }
        child = child->NextSibling();
    }
}

void MainWindow::ResetDebugLocations()
{
    Component* child = codeTabControl->TabPages().FirstChild();
    while (child)
    {
        if (child->IsTabPage())
        {
            wing::TabPage* tabPage = static_cast<wing::TabPage*>(child);
            cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
            if (editor->IsCmajorEditor())
            {
                cmajor::view::CmajorEditor* cmajorEditor = static_cast<cmajor::view::CmajorEditor*>(editor);
                cmajor::view::DebugStrip* debugStrip = cmajorEditor->GetDebugStrip();
                if (debugStrip)
                {
                    debugStrip->ResetDebugLocation();
                }
            }
        }
        child = child->NextSibling();
    }
}

void MainWindow::ResetBreakpoints()
{
    std::vector<cmajor::debugger::Breakpoint*> breakpoints = solutionData->GetBreakpoints(); 
    for (auto breakpoint : breakpoints)
    {
        breakpoint->disabled = false;
        breakpoint->location.line = 0;
    }
    UpdateCurrentDebugStrip();
}

void MainWindow::SetEditorsReadWrite()
{
    Component* child = codeTabControl->TabPages().FirstChild();
    while (child)
    {
        if (child->IsTabPage())
        {
            wing::TabPage* tabPage = static_cast<wing::TabPage*>(child);
            cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
            if (editor)
            {
                wing::TextView* textView = editor->GetTextView();
                if (textView)
                {
                    textView->SetReadWrite();
                }
            }
        }
        child = child->NextSibling();
    }
}

void MainWindow::SetFocusToEditor()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        wing::TextView* textView = editor->GetTextView();
        if (textView)
        {
            textView->SetFocus();
        }
    }
}

void MainWindow::EditorReadOnlyChanged()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        wing::TextView* textView = editor->GetTextView();
        if (textView)
        {
            std::string editorReadWriteText;
            if (textView->IsReadOnly())
            {
                editorReadWriteText = "R/O";
            }
            else
            {
                editorReadWriteText = "R/W";
            }
            editorReadWriteIndicatorStatusBarItem->SetText(editorReadWriteText);
        }
    }
}

void MainWindow::EditorDirtyChanged()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        wing::TextView* textView = editor->GetTextView();
        if (textView)
        {
            std::string editorDirtyText;
            if (textView->IsDirty())
            {
                saveToolButton->Enable();
                saveMenuItem->Enable();
                editorDirtyText = "*";
            }
            else
            {
                saveToolButton->Disable();
                saveMenuItem->Disable();
                editorDirtyText = std::string();
            }
            editorDirtyIndicatorStatusBarItem->SetText(editorDirtyText);
        }
    }
}

void MainWindow::EditorCCDirtyChanged()
{
    // todo
}

void MainWindow::EditorCaretPosChanged()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        wing::TextView* textView = editor->GetTextView();
        if (textView)
        {
            if (textView->CaretLine() > 0)
            {
                lineStatusBarItem->SetText(std::to_string(textView->CaretLine()));
            }
            else
            {
                lineStatusBarItem->SetText(std::string());
            }
            if (textView->CaretColumn() > 0)
            {
                columnStatusBarItem->SetText(std::to_string(textView->CaretColumn()));
            }
            else
            {
                columnStatusBarItem->SetText(std::string());
            }
        }
    }
}

void MainWindow::EditorSelectionChanged()
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            if (textView)
            {
                if (!textView->IsSelectionEmpty())
                {
                    copyMenuItem->Enable();
                    cutMenuItem->Enable();
                }
                else
                {
                    copyMenuItem->Disable();
                    cutMenuItem->Disable();
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::EditorRightClick(wing::RightClickEventArgs& args)
{
    try
    {
        clickActions.clear();
        Control* control = args.control;
        if (control->IsTextView())
        {
            wing::TextView* textView = static_cast<wing::TextView*>(control);
            std::unique_ptr<wing::ContextMenu> contextMenu(new wing::ContextMenu());
            if (state == MainWindowState::idle)
            {
                std::string identifier;
                cmajor::info::bs::DefinitionSourceLocation sourceLocation;
                if (GetDefinitionSourceLocationAt(args.location, textView, identifier, sourceLocation))
                {
                    std::unique_ptr<wing::MenuItem> gotoDefinitionMenuItem(new wing::MenuItem("Go To Definition"));
                    cmajor::ast::Project* project = CurrentProject();
                    if (!project)
                    {
                        throw std::runtime_error("current project not deduced");
                    }
                    clickActions.push_back(std::unique_ptr<wing::ClickAction>(new GotoDefinitionAction(gotoDefinitionMenuItem.get(), this, project, identifier, sourceLocation)));
                    contextMenu->AddMenuItem(gotoDefinitionMenuItem.release());
                }
            }
            else if (state == MainWindowState::debugging)
            {
                cmajor::info::db::SourceLoc sourceLocation;
                sourceLocation.path = textView->FilePath();
                sourceLocation.line = textView->CaretLine();
                std::unique_ptr<wing::MenuItem> gotoCursorMenuItem(new wing::MenuItem("Go To Cursor"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new GotoCursorAction(gotoCursorMenuItem.get(), this, sourceLocation)));
                contextMenu->AddMenuItem(gotoCursorMenuItem.release());
            }
            if (contextMenu->HasMenuItems())
            {
                wing::Point loc = args.location;
                textView->TranslateContentLocationInternal(loc);
                wing::Point screenLoc = textView->ClientToScreen(loc);
                ShowContextMenu(contextMenu.release(), screenLoc);
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

bool MainWindow::GetDefinitionSourceLocationAt(const wing::Point& loc, wing::TextView* textView, std::string& identifier, cmajor::info::bs::DefinitionSourceLocation& sourceLocation)
{
    int line = 0;
    int column = 0;
    textView->GetLineColumn(loc, line, column);
    if (line >= 1 && line <= textView->Lines().size())
    {
        if (column >= 1 && column <= textView->GetLineLength(line))
        {
            char32_t c = textView->GetCharAt(line, column);
            if (util::IsIdStart(c) || util::IsIdCont(c))
            {
                while (column > 1 && (util::IsIdStart(c) || util::IsIdCont(c)))
                {
                    --column;
                    c = textView->GetCharAt(line, column);
                }
                if (util::IsWhiteSpace(c) || wing::IsWordSeparator(c))
                {
                    ++column;
                }
                c = textView->GetCharAt(line, column);
                std::u32string id;
                int scol = column;
                while (column <= textView->GetLineLength(line) && (util::IsIdStart(c) || util::IsIdCont(c)))
                {
                    id.append(1, c);
                    ++column;
                    if (column <= textView->GetLineLength(line))
                    {
                        c = textView->GetCharAt(line, column);
                    }
                }
                identifier = util::ToUtf8(id);
                sourceLocation.file = textView->FilePath();
                sourceLocation.line = line;
                sourceLocation.scol = scol;
                return true;
            }
        }
    }
    return false;
}

int MainWindow::GetEndColumn(wing::TextView* textView, const cmajor::info::bs::DefinitionSourceLocation& sourceLocation) const
{
    char32_t c = textView->GetCharAt(sourceLocation.line, sourceLocation.scol);
    int line = sourceLocation.line;
    int ecol = sourceLocation.scol;
    while (ecol <= textView->GetLineLength(line) && (util::IsIdStart(c) || util::IsIdCont(c)))
    {
        ++ecol;
        if (ecol <= textView->GetLineLength(line))
        {
            c = textView->GetCharAt(line, ecol);
        }
    }
    return ecol;
}

cmajor::ast::Project* MainWindow::CurrentProject()
{
    if (!solutionData) return nullptr;
    cmajor::ast::Project* project = nullptr;
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(editor->FilePath());
        if (data)
        {
            project = data->project;
        }
    }
    if (!project)
    {
        project = solutionData->GetSolution()->ActiveProject();
    }
    return project;
}

void MainWindow::GotoDefinition(cmajor::ast::Project* project, const std::string& identifier, const cmajor::info::bs::DefinitionSourceLocation& sourceLocation)
{
    try
    {
        cmajor::info::bs::GetDefinitionRequest request;
        request.backend = backend;
        request.config = config;
        request.optimizationLevel = std::to_string(optLevel);
        request.projectName = util::ToUtf8(project->Name());
        request.projectFilePath = project->FilePath();
        request.identifier = identifier;
        request.identifierLocation = sourceLocation;
        getDefinitionRequest.reset(new cmajor::service::GetDefinitionRequest(request));
        if (!BuildActiveProject())
        {
            getDefinitionRequest.reset();
        }

    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::GotoCursor(const cmajor::info::db::SourceLoc& sourceLocation)
{
    //cmajor::service::PutRequest(new cmajor::service::UntilDebugServiceRequest(sourceLocation));
}

void MainWindow::GotoLocation(const cmajor::info::bs::DefinitionSourceLocation& location)
{
    wing::TabPage* tabPage = codeTabControl->GetTabPageByKey(location.file);
    cmajor::view::CmajorEditor* cmajorEditor = nullptr;
    if (tabPage)
    {
        cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
        if (editor->IsCmajorEditor())
        {
            cmajorEditor = static_cast<cmajor::view::CmajorEditor*>(editor);
        }
        tabPage->Select();
    }
    if (!cmajorEditor)
    {
        SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(location.file);
        if (data)
        {
            cmajorEditor = AddCmajorEditor(data->fileName, data->filePath, data->filePath, data->project);
        }
        else
        {
            cmajorEditor = AddCmajorEditor(util::Path::GetFileName(location.file), location.file, location.file, nullptr);
        }
    }
    if (cmajorEditor)
    {
        wing::TextView* textView = cmajorEditor->GetTextView();
        if (textView)
        {
            int line = location.line;
            int scol = location.scol;
            int ecol = GetEndColumn(textView, location);
            wing::Selection selection;
            selection.start.line = line;
            selection.start.col = scol;
            selection.end.line = line;
            selection.end.col = ecol;
            textView->EnsureLineVisible(line);
            textView->SetSelection(selection);
            textView->SetCaretLineCol(line, scol);
        }
    }
}

void MainWindow::SetProjectReferences(cmajor::ast::Project* project)
{
    try
    {
        if (!solutionData) return;
        cmajor::ast::Solution* solution = solutionData->GetSolution();
        ProjectReferencesDialog dialog(project, solution);
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            std::vector<cmajor::ast::Project*> referencedProjects = dialog.ReferencedProjects();
            project->SetReferencedProjects(referencedProjects);
            project->Save();
            LoadEditModule(project);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

cmajor::info::bs::DefinitionSourceLocation MainWindow::CurrentLocation() const
{
    cmajor::info::bs::DefinitionSourceLocation currentLocation;
    if (!solutionData) return currentLocation;
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        wing::TextView* textView = editor->GetTextView();
        if (textView)
        {
            currentLocation.file = textView->FilePath();
            int line = textView->CaretLine();
            int column = textView->CaretColumn();
            currentLocation.line = line;
            currentLocation.scol = column;
        }
    }
    return currentLocation;
}

void MainWindow::ChangeBreakpoints(wing::CancelArgs& args)
{
    try
    {
        if (cmajor::service::DebugRequestInProgress())
        {
            args.cancel = true;
            throw std::runtime_error("cannot change breakpoints while debug request is running");
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::BreakpointAdded(cmajor::view::AddBreakpointEventArgs& args)
{
    try
    {
        if (state != MainWindowState::debugging)
        {
            //args.breakpoint->info.success = true; // TODO
            return;
        }
        //cmajor::service::PutRequest(new cmajor::service::BreakDebugServiceRequest(args.breakpoint)); TODO
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::BreakpointRemoved(cmajor::view::RemoveBreakpointEventArgs& args)
{
    try
    {
        if (state != MainWindowState::debugging)
        {
            return;
        }
        //cmajor::service::PutRequest(new cmajor::service::DeleteDebugServiceRequest(args.breakpointId)); TODO
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::VerticalSplitContainerSplitterDistanceChanged()
{
    if (GetWindowState() == wing::WindowState::normal && !sizeChanged)
    {
        verticalSplitContainerFactor = (verticalSplitContainer->GetSize().Height - verticalSplitContainer->Pane2Container()->GetSize().Height) / (1.0f * verticalSplitContainer->GetSize().Height);
    }
}

void MainWindow::HorizontalSplitContainerSplitterDistanceChanged()
{
    if (GetWindowState() == wing::WindowState::normal && !sizeChanged)
    {
        horizontalSplitContainerFactor = (horizontalSplitContainer->GetSize().Width - horizontalSplitContainer->Pane2Container()->GetSize().Width) / (1.0f * horizontalSplitContainer->GetSize().Width);
    }
}

void MainWindow::NewProjectClick()
{
    try
    {
        NewProjectDialog dialog;
        dialog.SetProjectLocation(cmajor::service::CmajorProjectsDir());
        ShowDialogGuard guard(showingDialog);
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            bool sameDir = dialog.PlaceSolutionInSameDirectory();
            std::string solutionFilePath;
            std::string projectFilePath;
            if (!sameDir)
            {
                solutionFilePath = util::Path::Combine(dialog.GetProjectLocation(), util::Path::Combine(dialog.GetSolutionName(), dialog.GetSolutionName() + ".cms"));
                projectFilePath = util::Path::Combine(dialog.GetProjectLocation(), util::Path::Combine(dialog.GetSolutionName(), 
                    util::Path::Combine(dialog.GetProjectName(), dialog.GetProjectName() + ".cmp")));
                std::string solutionDir = util::Path::GetDirectoryName(solutionFilePath);
                std::filesystem::create_directories(solutionDir);
                std::string projectDir = util::Path::GetDirectoryName(projectFilePath);
                std::filesystem::create_directories(projectDir);
            }
            else
            {
                projectFilePath = util::Path::Combine(dialog.GetProjectLocation(), util::Path::Combine(dialog.GetProjectName(), dialog.GetProjectName() + ".cmp"));
                solutionFilePath = util::Path::ChangeExtension(projectFilePath, ".cms");
                std::string projectDir = util::Path::GetDirectoryName(projectFilePath);
                std::filesystem::create_directories(projectDir);
            }
            if (std::filesystem::exists(solutionFilePath))
            {
                wing::MessageBoxResult result = wing::MessageBox::Show("Solution file '" + solutionFilePath + "' already exists. Do you want to overwrite it?", "Question", 
                    this, MB_YESNO);
                if (result == wing::MessageBoxResult::no)
                {
                    return;
                }
            }
            if (std::filesystem::exists(projectFilePath))
            {
                wing::MessageBoxResult result = wing::MessageBox::Show("Project file '" + projectFilePath + "' already exists. Do you want to overwrite it?", "Question", 
                    this, MB_YESNO);
                if (result == wing::MessageBoxResult::no)
                {
                    return;
                }
            }
            std::unique_ptr<cmajor::ast::Solution> solution(new cmajor::ast::Solution(util::ToUtf32(dialog.GetSolutionName()), solutionFilePath));
            std::unique_ptr<cmajor::ast::Project> project(new cmajor::ast::Project(util::ToUtf32(dialog.GetProjectName()), projectFilePath, "debug", 
                cmajor::ast::BackEnd::llvm, optLevel));
            project->SetTarget(dialog.GetProjectType());
            solution->SetActiveProject(project.get());
            solution->AddProject(std::move(project));
            solution->Save();
            if (!CloseSolution())
            {
                return;
            }
            solutionExplorer->MakeView();
            OpenProject(solutionFilePath);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::OpenProjectClick()
{
    try
    {
        std::vector<std::pair<std::string, std::string>> descriptionFilterPairs;
        descriptionFilterPairs.push_back(std::make_pair("Cmajor Solution Files (*.cms)", "*.cms"));
        descriptionFilterPairs.push_back(std::make_pair("Cmajor Project Files (*.cmp)", "*.cmp"));
        std::string initialDirectory = cmajor::service::CmajorProjectsDir();
        std::string filePath;
        std::string currentDirectory;
        std::vector<std::string> fileNames;
        bool selected = wing::OpenFileName(Handle(), descriptionFilterPairs, initialDirectory, std::string(), "cms", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filePath, currentDirectory, fileNames);
        if (selected)
        {
            if (CloseSolution())
            {
                solutionExplorer->MakeView();
                OpenProject(util::GetFullPath(filePath));
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

bool MainWindow::CloseSolution()
{
    try
    {
        if (!solutionData) return true;
        //ccState = CCState::idle;
        callStackOpen = callStackView != nullptr;
        localsViewOpen = localsView != nullptr;
        SaveSolutionData();
        wing::Component* child = codeTabControl->TabPages().FirstChild();
        while (child)
        {
            if (child->IsTabPage())
            {
                wing::TabPage* tabPage = static_cast<wing::TabPage*>(child);
                cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
                if (editor)
                {
                    if (editor->IsDirty())
                    {
                        wing::MessageBoxResult result = wing::MessageBox::Show(editor->FilePath() + " is modified. Save changes?", "Question", this, MB_YESNOCANCEL);
                        if (result == wing::MessageBoxResult::cancel) return false;
                        else if (result == wing::MessageBoxResult::yes)
                        {
                            editor->Save();
                        }
                    }
                }
            }
            child = child->NextSibling();
        }
        codeTabControl->CloseAllTabPages();
        solutionData.reset();
        ResetFocusedControl();
        SetEditorState();
        if (callStackTabPage)
        {
            callStackTabPage->Close();
        }
        if (localsTabPage)
        {
            localsTabPage->Close();
        }
        return true;
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
    return false;
}

void MainWindow::CloseSolutionClick()
{
    if (CloseSolution())
    {
        solutionExplorer->MakeView();
    }
}

void MainWindow::SaveClick()
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            editor->Save();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::SaveAllClick()
{
    try
    {
        if (!solutionData) return;
        cmajor::ast::Solution* solution = solutionData->GetSolution();
        solution->Save();
        SaveSolutionData();
        SaveProjectData();
        Component* child = codeTabControl->TabPages().FirstChild();
        while (child)
        {
            if (child->IsTabPage())
            {
                wing::TabPage* tabPage = static_cast<wing::TabPage*>(child);
                cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
                if (editor)
                {
                    if (editor->IsDirty())
                    {
                        editor->Save();
                    }
                }
            }
            child = child->NextSibling();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::ExitClick()
{
    Close();
}

void MainWindow::CopyClick()
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            if (textView)
            {
                if (textView->IsReadOnly()) return;
                if (!textView->IsSelectionEmpty())
                {
                    wing::SelectionData selection = textView->GetSelection();
                    if (!selection.selectedText.empty())
                    {
                        try
                        {
                            wing::Clipboard clipboard(Handle());
                            clipboard.SetEmpty();
                            clipboard.SetStringData(cmajorCodeFormat, selection.selectedText);
                            std::u32string crlfText = selection.selectedText;
                            std::u32string::size_type lfPos = crlfText.find('\n');
                            while (lfPos != std::u32string::npos)
                            {
                                crlfText.replace(lfPos, 1, U"\r\n");
                                lfPos = crlfText.find('\n', lfPos + 2);
                            }
                            wing::ClipboardFormat unicodeText(wing::unicodeTextClipboardFormat);
                            clipboard.SetStringData(unicodeText, crlfText);
                        }
                        catch (const std::exception& ex)
                        {
                            wing::ShowErrorMessageBox(Handle(), ex.what());
                            clipboardData = selection.selectedText;
                            pasteMenuItem->Enable();
                        }
                    }
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::CutClick()
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            if (textView)
            {
                if (textView->IsReadOnly()) return;
                if (!textView->IsSelectionEmpty())
                {
                    wing::SelectionData selection = textView->GetSelection();
                    if (!selection.selectedText.empty())
                    {
                        try
                        {
                            wing::Clipboard clipboard(Handle());
                            clipboard.SetEmpty();
                            clipboard.SetStringData(cmajorCodeFormat, selection.selectedText);
                            std::u32string crlfText = selection.selectedText;
                            std::u32string::size_type lfPos = crlfText.find('\n');
                            while (lfPos != std::u32string::npos)
                            {
                                crlfText.replace(lfPos, 1, U"\r\n");
                                lfPos = crlfText.find('\n', lfPos + 2);
                            }
                            wing::ClipboardFormat unicodeText(wing::unicodeTextClipboardFormat);
                            clipboard.SetStringData(unicodeText, crlfText);
                            textView->AddRemoveSelectionCommand();
                            textView->RemoveSelection();
                        }
                        catch (const std::exception& ex)
                        {
                            wing::ShowErrorMessageBox(Handle(), ex.what());
                            textView->AddRemoveSelectionCommand();
                            textView->RemoveSelection();
                            clipboardData = selection.selectedText;
                            pasteMenuItem->Enable();
                        }
                    }
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::PasteClick()
{
    try
    {
        if (!clipboardData.empty())
        {
            cmajor::view::Editor* editor = CurrentEditor();
            if (editor)
            {
                wing::TextView* textView = editor->GetTextView();
                if (textView)
                {
                    if (textView->IsReadOnly()) return;
                    int lineIndex = textView->CaretLine() - 1;
                    int columnIndex = textView->CaretColumn() - 1;
                    textView->SetFocus();
                    textView->InsertText(lineIndex, columnIndex, clipboardData);
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::UndoClick()
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            if (textView)
            {
                if (textView->IsReadOnly()) return;
                textView->Undo();
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::RedoClick()
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            wing::TextView* textView = editor->GetTextView();
            if (textView)
            {
                if (textView->IsReadOnly()) return;
                textView->Redo();
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::GotoClick()
{
    try
    {
        GotoLineDialog dialog;
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            cmajor::view::Editor* editor = CurrentEditor();
            if (editor)
            {
                wing::TextView* textView = editor->GetTextView();
                if (textView)
                {
                    int lineNumber = dialog.LineNumber();
                    textView->SetCaretLineCol(std::min(lineNumber, static_cast<int>(textView->Lines().size())), 1 + textView->LineNumberFieldLength());
                    textView->ScrollToCaret();
                    textView->SetFocus();
                    textView->Invalidate();
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::SearchClick()
{
    try
    {
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        cmajor::ast::Solution* solution = solutionData->GetSolution();
        if (searchResultsTabPage)
        {
            searchResultsTabPage->Close();
            searchResultsTabPage = nullptr;
        }
        bool hasCurrentFile = false;
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            hasCurrentFile = true;
        }
        ShowDialogGuard dialogGuard(showingDialog);
        SearchDialog dialog(hasCurrentFile);
        wing::DialogResult result = dialog.ShowDialog(*this);
        if (result == wing::DialogResult::ok)
        {
            std::vector<std::string> files;
            switch (dialog.GetSearchExtent())
            {
            case SearchExtent::currentFile:
            {
                files.push_back(editor->FilePath());
                std::vector<std::u32string> lines;
                for (const auto& line : editor->GetTextView()->Lines())
                {
                    lines.push_back(*line);
                }
                Search(SearchExtent::currentFile, files, lines, dialog.SearchText(), dialog.WholeWords(), dialog.CaseInsensitive(), dialog.RegularExpression());
                break;
            }
            case SearchExtent::activeProject:
            {
                cmajor::ast::Project* activeProject = solution->ActiveProject();
                files = activeProject->SourceFilePaths();
                Search(SearchExtent::activeProject, files, std::vector<std::u32string>(), dialog.SearchText(), dialog.WholeWords(), dialog.CaseInsensitive(), dialog.RegularExpression());
                break;
            }
            case SearchExtent::entireSolution:
            {
                for (const auto& project : solution->Projects())
                {
                    for (const auto& sourceFilePath : project->SourceFilePaths())
                    {
                        files.push_back(sourceFilePath);
                    }
                }
                Search(SearchExtent::entireSolution, files, std::vector<std::u32string>(), dialog.SearchText(), dialog.WholeWords(), dialog.CaseInsensitive(), dialog.RegularExpression());
                break;
            }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::Search(SearchExtent extent, const std::vector<std::string>& files, const std::vector<std::u32string>& lines, const std::string& searchText,
    bool wholeWords, bool caseInsensitive, bool regularExpression)
{
    cmajor::view::SearchResultsView* searchResults = GetSearchResultsView();
    searchResults->Clear();
    std::u32string text = util::ToUtf32(searchText);
    if (caseInsensitive)
    {
        text = util::ToLower(text);
    }
    soul::rex::nfa::Nfa* re = nullptr;
    soul::rex::context::Context context;
    soul::rex::nfa::Nfa nfa;
    if (regularExpression)
    {
        nfa = soul::rex::CompileRegularExpressionPattern(context, U".*(" + text + U").*");
        re = &nfa;
    }
    for (const std::string& fileName : files)
    {
        std::vector<std::u32string> readLines;
        const std::vector<std::u32string>* lns = &readLines;
        if (extent == SearchExtent::currentFile)
        {
            lns = &lines;
        }
        else
        {
            std::string content = util::ReadFile(fileName, true);
            readLines = wing::SplitTextIntoLines(util::ToUtf32(content));
        }
        int n = lns->size();
        for (int i = 0; i < n; ++i)
        {
            const std::u32string* line = &(*lns)[i];
            std::u32string lowerLine;
            if (caseInsensitive)
            {
                lowerLine = util::ToLower(*line);
                line = &lowerLine;
            }
            if (Search(*line, text, wholeWords, re))
            {
                searchResults->Add(new cmajor::view::SearchResult(fileName, i + 1, util::ToUtf8((*lns)[i])));
            }
        }
    }
    searchResults->Invalidate();
}

bool MainWindow::Search(const std::u32string& line, const std::u32string& text, bool wholeWords, soul::rex::nfa::Nfa* re)
{
    if (re)
    {
        return soul::rex::PatternMatch(line, *re);
    }
    else
    {
        std::u32string::size_type pos = line.find(text, 0);
        while (pos != std::u32string::npos)
        {
            bool match = true;
            if (wholeWords)
            {
                if (pos > 0)
                {
                    char32_t prev = line[pos - 1];
                    if (util::IsAlphabetic(prev) || util::IsNumber(prev))
                    {
                        match = false;
                    }
                }
                if (pos + text.length() < line.length())
                {
                    char32_t next = line[pos + text.length()];
                    if (util::IsAlphabetic(next) || util::IsNumber(next))
                    {
                        match = false;
                    }
                }
            }
            if (match)
            {
                return true;
            }
            pos = line.find(text, pos + 1);
        }
    }
    return false;
}

void MainWindow::ViewSearchResult(cmajor::view::ViewSearchResultEventArgs& args)
{
    try
    {
        cmajor::view::SearchResult* searchResult = args.searchResult;
        if (searchResult)
        {
            wing::TabPage* tabPage = codeTabControl->GetTabPageByKey(searchResult->fileName);
            cmajor::view::Editor* editor = nullptr;
            if (tabPage)
            {
                editor = GetEditorByTabPage(tabPage);
                tabPage->Select();
            }
            if (!editor)
            {
                SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(searchResult->fileName);
                if (data)
                {
                    editor = AddCmajorEditor(data->fileName, data->key, data->filePath, data->project);
                }
                else
                {
                    editor = AddCmajorEditor(util::Path::GetFileName(searchResult->fileName), searchResult->fileName, searchResult->fileName, nullptr);
                }
            }
            if (editor)
            {
                wing::TextView* textView = editor->GetTextView();
                if (textView)
                {
                    textView->EnsureLineVisible(searchResult->lineNumber);
                    textView->SetCaretLineCol(std::min(searchResult->lineNumber, static_cast<int>(textView->Lines().size())), 1 + textView->LineNumberFieldLength());
                    textView->ScrollToCaret();
                    textView->SetFocus();
                    textView->Invalidate();
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::OptionsClick()
{
    try
    {
        const Options& prevOptions = GetOptions();
        //bool prevCodeCompletion = prevOptions.codeCompletion;
        OptionsDialog dialog;
        dialog.SetOptionsFrom(GetOptions());
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            const Options& options = dialog.GetOptions();
            SetOptions(options);
            SaveConfiguration();
            wing::SetTheme(options.theme);
/*
            if (prevCodeCompletion != options.codeCompletion)
            {
                if (options.codeCompletion)
                {
                    codeCompletionStatusBarItem->SetText("On");
                    StartCodeCompletion();
                }
                else
                {
                    codeCompletionStatusBarItem->SetText("Off");
                    StopCodeCompletion(true);
                }
            }
*/
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::CallStackClick()
{
    try
    {
        ClearCallStack();
        GetCallStackView()->Invalidate();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::LocalsClick()
{
    try
    {
        GetLocalsView();
        UpdateLocals();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::ErrorsClick()
{
    try
    {
        cmajor::view::ErrorView* view = GetErrorView();
        if (view)
        {
            view->Invalidate();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::SearchResultsClick()
{
    try
    {
        cmajor::view::SearchResultsView* view = GetSearchResultsView();
        if (view)
        {
            view->Invalidate();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::PortMapClick()
{
    try
    {
        util::Process* process = new util::Process("cmbpmv", util::Process::Redirections::none);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::BuildSolutionClick()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, solutionData->GetSolution()->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::build | BuildRequestKind::buildDependencies);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::RebuildSolutionClick()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, solutionData->GetSolution()->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::rebuild | BuildRequestKind::buildDependencies);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::CleanSolutionClick()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, solutionData->GetSolution()->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::clean);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::BuildProject(cmajor::ast::Project* project)
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, project->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::build | BuildRequestKind::buildDependencies);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::RebuildProject(cmajor::ast::Project* project)
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, project->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::rebuild | BuildRequestKind::buildDependencies);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::CleanProject(cmajor::ast::Project* project)
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, project->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::clean | BuildRequestKind::buildDependencies);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::AddNewProject()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        AddNewProjectDialog dialog;
        cmajor::ast::Solution* solution = solutionData->GetSolution();
        std::string solutionDir = util::Path::GetDirectoryName(solution->FilePath());
        dialog.SetProjectLocation(solutionDir);
        ShowDialogGuard guard(showingDialog);
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            std::string projectFilePath = util::Path::Combine(dialog.GetProjectLocation(), util::Path::Combine(dialog.GetProjectName(), dialog.GetProjectName() + ".cmp"));
            std::string projectDir = util::Path::GetDirectoryName(projectFilePath);
            std::filesystem::create_directories(projectDir);
            if (std::filesystem::exists(projectFilePath))
            {
                wing::MessageBoxResult result = wing::MessageBox::Show("Project file '" + projectFilePath + "' already exists. Do you want to overwrite it?", "Question",
                    this, MB_YESNO);
                if (result == wing::MessageBoxResult::no)
                {
                    return;
                }
            }
            std::unique_ptr<cmajor::ast::Project> project(new cmajor::ast::Project(util::ToUtf32(dialog.GetProjectName()), projectFilePath, "debug", 
                cmajor::ast::BackEnd::llvm, optLevel));
            project->SetTarget(dialog.GetProjectType());
            solution->AddProject(std::move(project));
            solution->SortByProjectName();
            solution->Save();
            std::string solutionFilePath = solution->FilePath();
            if (!CloseSolution())
            {
                return;
            }
            solutionExplorer->MakeView();
            OpenProject(solutionFilePath);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::AddExistingProject()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        std::vector<std::pair<std::string, std::string>> descriptionFilterPairs;
        descriptionFilterPairs.push_back(std::pair<std::string, std::string>("Cmajor Project Files (*.cmp)", "*.cmp"));
        std::string initialDirectory = cmajor::service::CmajorProjectsDir();
        std::string filePath;
        std::string currentDirectory;
        std::vector<std::string> fileNames;
        std::string defaultFilePath;
        bool selected = wing::OpenFileName(Handle(), descriptionFilterPairs, initialDirectory, std::string(), "cmp", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filePath, currentDirectory, fileNames);
        if (selected)
        {
            std::vector<std::unique_ptr<std::u32string>> contents;
            std::unique_ptr<cmajor::ast::Project> project = ReadProject(util::GetFullPath(filePath), contents);
            cmajor::ast::Solution* solution = solutionData->GetSolution();
            if (solution->HasProject(project->Name()))
            {
                throw std::runtime_error("solution already has a project with name '" + util::ToUtf8(project->Name()) + "'");
            }
            solution->AddProject(std::move(project));
            solution->SortByProjectName();
            solution->Save();
            std::string solutionFilePath = solution->FilePath();
            if (!CloseSolution())
            {
                return;
            }
            solutionExplorer->MakeView();
            OpenProject(solutionFilePath);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::SetActiveProject(cmajor::ast::Project* project, wing::TreeViewNode* newActiveProjectNode)
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        solutionData->ActiveProjectNode()->ResetActive();
        newActiveProjectNode->SetActive();
        solutionData->SetActiveProjectNode(newActiveProjectNode);
        cmajor::ast::Solution* solution = solutionData->GetSolution();
        solution->SetActiveProject(project);
        solution->Save();
        solutionExplorer->GetTreeView()->Invalidate();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::RemoveProject(cmajor::ast::Project* project)
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        std::string projectName = util::ToUtf8(project->Name());
        solutionData->RemoveProjectNode(projectName);
        solutionData->RemoveExpandedProject(projectName);
        cmajor::ast::Solution* solution = solutionData->GetSolution();
        solution->RemoveProject(project);
        solution->Save();
        std::string solutionFilePath = solution->FilePath();
        if (!CloseSolution())
        {
            return;
        }
        solutionExplorer->MakeView();
        OpenProject(solutionFilePath);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::OpenFileLocation(const std::string& filePath)
{
    try
    {
        std::string directoryLocation = util::Path::GetDirectoryName(filePath);
#pragma warning(disable:4311)
#pragma warning(disable:4302)
        if (reinterpret_cast<int>(ShellExecuteA(Handle(), "open", directoryLocation.c_str(), nullptr, nullptr, SW_SHOWNORMAL)) < 32)
        {
            throw std::runtime_error("shell execute failed");
        }
#pragma warning(default:4311)
#pragma warning(default:4302)
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::AddNewSourceFile(cmajor::ast::Project* project, wing::TreeViewNode* projectNode)
{
    try
    {
        AddNewSourceFileDialog dialog;
        ShowDialogGuard guard(showingDialog);
        wing::DialogResult result = dialog.ShowDialog(*this);
        if (result == wing::DialogResult::ok)
        {
            std::string sourceFileName = dialog.SourceFileName();
            if (!sourceFileName.ends_with(".cm"))
            {
                sourceFileName.append(".cm");
            }
            std::string newSourceFilePath = util::GetFullPath(util::Path::Combine(project->SourceBasePath().generic_string(), sourceFileName));
            if (project->HasSourceFile(newSourceFilePath))
            {
                throw std::runtime_error("source file '" + newSourceFilePath + "' already exists in project '" + util::ToUtf8(project->Name()) + "'");
            }
            project->AddSourceFileName(sourceFileName, newSourceFilePath);
            project->Save();
            LoadEditModule(project);
            AddFilePathsToProject(newSourceFilePath, "", "", project, projectNode);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::AddExistingSourceFile(cmajor::ast::Project* project, wing::TreeViewNode* projectNode)
{
    try
    {
        std::vector<std::pair<std::string, std::string>> descriptionFilterPairs;
        descriptionFilterPairs.push_back(std::make_pair("Cmajor Source Files (*.cm)", "*.cm"));
        std::string initialDirectory = util::Path::GetDirectoryName(project->FilePath());
        std::string currentDirectory;
        std::vector<std::string> fileNames;
        std::string filePath;
        bool selected = wing::OpenFileName(Handle(), descriptionFilterPairs, initialDirectory, std::string(), "cm", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filePath, currentDirectory, fileNames);
        if (selected)
        {
            filePath = util::GetFullPath(filePath);
            if (project->HasSourceFile(filePath))
            {
                throw std::runtime_error("project '" + util::ToUtf8(project->Name()) + "' already has source file '" + filePath + "'");
            }
            std::string fileName = util::Path::GetFileName(filePath);
            project->AddSourceFileName(fileName, filePath);
            project->Save();
            LoadEditModule(project);
            AddFilePathsToProject("", "", "", project, projectNode);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::AddNewResourceFile(cmajor::ast::Project* project, wing::TreeViewNode* projectNode)
{
    try
    {
        AddNewResourceFileDialog dialog;
        ShowDialogGuard guard(showingDialog);
        wing::DialogResult result = dialog.ShowDialog(*this);
        if (result == wing::DialogResult::ok)
        {
            std::string resourceFileName = dialog.ResourceFileName();
            if (!resourceFileName.ends_with(".xml"))
            {
                resourceFileName.append(".xml");
            }
            std::string newResourceFilePath = util::GetFullPath(util::Path::Combine(project->SourceBasePath().generic_string(), resourceFileName));
            if (project->HasResourceFile(newResourceFilePath))
            {
                throw std::runtime_error("resource file '" + newResourceFilePath + "' already exists in project '" + util::ToUtf8(project->Name()) + "'");
            }
            project->AddResourceFileName(resourceFileName, newResourceFilePath);
            project->Save();
            AddFilePathsToProject("", newResourceFilePath, "", project, projectNode);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::AddExistingResourceFile(cmajor::ast::Project* project, wing::TreeViewNode* projectNode)
{
    try
    {
        std::vector<std::pair<std::string, std::string>> descriptionFilterPairs;
        descriptionFilterPairs.push_back(std::make_pair("Cmajor Resource Files (*.xml)", "*.xml"));
        std::string initialDirectory = util::Path::GetDirectoryName(project->FilePath());
        std::string currentDirectory;
        std::vector<std::string> fileNames;
        std::string filePath;
        bool selected = wing::OpenFileName(Handle(), descriptionFilterPairs, initialDirectory, std::string(), "xml", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filePath, currentDirectory, fileNames);
        if (selected)
        {
            filePath = util::GetFullPath(filePath);
            if (project->HasResourceFile(filePath))
            {
                throw std::runtime_error("project '" + util::ToUtf8(project->Name()) + "' already has resource file '" + filePath + "'");
            }
            std::string fileName = util::Path::GetFileName(filePath);
            project->AddResourceFileName(fileName, filePath);
            project->Save();
            AddFilePathsToProject("", "", "", project, projectNode);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::AddNewTextFile(cmajor::ast::Project* project, wing::TreeViewNode* projectNode)
{
    try
    {
        AddNewTextFileDialog dialog;
        ShowDialogGuard guard(showingDialog);
        wing::DialogResult result = dialog.ShowDialog(*this);
        if (result == wing::DialogResult::ok)
        {
            std::string fileName = dialog.FileName();
            std::string filePath = util::GetFullPath(util::Path::Combine(project->SourceBasePath().generic_string(), fileName));
            if (project->HasTextFile(filePath))
            {
                throw std::runtime_error("text file '" + filePath + "' already exists in project '" + util::ToUtf8(project->Name()) + "'");
            }
            project->AddTextFileName(fileName, filePath);
            project->Save();
            AddFilePathsToProject("", "", filePath, project, projectNode);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::AddExistingTextFile(cmajor::ast::Project* project, wing::TreeViewNode* projectNode)
{
    try
    {
        std::vector<std::pair<std::string, std::string>> descriptionFilterPairs;
        descriptionFilterPairs.push_back(std::make_pair("Text Files (*.*)", "*.*"));
        std::string initialDirectory = util::Path::GetDirectoryName(project->FilePath());
        std::string currentDirectory;
        std::vector<std::string> fileNames;
        std::string filePath;
        bool selected = wing::OpenFileName(Handle(), descriptionFilterPairs, initialDirectory, std::string(), "txt", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filePath, currentDirectory, fileNames);
        if (selected)
        {
            filePath = util::GetFullPath(filePath);
            if (project->HasTextFile(filePath))
            {
                throw std::runtime_error("project '" + util::ToUtf8(project->Name()) + "' already has text file '" + filePath + "'");
            }
            std::string fileName = util::Path::GetFileName(filePath);
            project->AddTextFileName(fileName, filePath);
            project->Save();
            AddFilePathsToProject("", "", "", project, projectNode);
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::RemoveFile(cmajor::ast::Project* project, const std::string& filePath, const std::string& fileName, wing::TreeViewNode* fileNode)
{
    try
    {
        wing::TabPage* tabPage = codeTabControl->GetTabPageByKey(filePath);
        if (tabPage)
        {
            cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
            if (editor)
            {
                if (editor->IsDirty())
                {
                    editor->Save();
                }
            }
            tabPage->Close();
        }
        wing::TreeViewNode* projectNode = fileNode->Parent();
        solutionData->RemoveOpenFile(filePath);
        if (solutionData->CurrentOpenFile() == filePath)
        {
            solutionData->SetCurrentOpenFile(std::string());
        }
        project->RemoveFile(filePath, fileName);
        project->Save();
        if (filePath.ends_with(".cm"))
        {
            LoadEditModule(project);
        }
        projectNode->RemoveChild(fileNode);
        Invalidate();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::BuildActiveProjectClick()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        cmajor::ast::Project* activeProject = solutionData->GetSolution()->ActiveProject();
        if (!activeProject)
        {
            throw std::runtime_error("no active project set for the solution");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, activeProject->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::build | BuildRequestKind::buildDependencies);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

bool MainWindow::BuildActiveProject()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        cmajor::ast::Project* activeProject = solutionData->GetSolution()->ActiveProject();
        if (!activeProject)
        {
            throw std::runtime_error("no active project set for the solution");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, activeProject->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::build | BuildRequestKind::buildDependencies);
        return true;
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
    return false;
}

void MainWindow::RebuildActiveProjectClick()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        cmajor::ast::Project* activeProject = solutionData->GetSolution()->ActiveProject();
        if (!activeProject)
        {
            throw std::runtime_error("no active project set for the solution");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, activeProject->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::rebuild | BuildRequestKind::buildDependencies);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::CleanActiveProjectClick()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        cmajor::ast::Project* activeProject = solutionData->GetSolution()->ActiveProject();
        if (!activeProject)
        {
            throw std::runtime_error("no active project set for the solution");
        }
        const BuildSettings& buildSettings = GetBuildSettings();
        StartBuilding();
        StartBuild(backend, config, optLevel, activeProject->FilePath(), buildSettings.disabledWarnings, BuildRequestKind::clean | BuildRequestKind::buildDependencies);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::BuildSettingsClick()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        BuildSettingsDialog dialog;
        dialog.SetValuesFrom(GetBuildSettings());
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            SetBuildSettings(dialog.GetValues());
            SaveConfiguration();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), std::string(ex.what()));
    }
}

void MainWindow::StartDebuggingClick()
{
    try
    {
        ClearCallStack();
        ClearLocals();
        ResetSelections();
        if (state == MainWindowState::debugging)
        {
            StartDebugging();
            cmajor::service::PutRequest(new cmajor::service::ContinueDebugServiceRequest()); 
        }
        else
        {
            debugRequest.reset(new cmajor::service::RunDebugServiceRequest()); 
            if (!BuildActiveProject())
            {
                debugRequest.reset();
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::StartWithoutDebuggingClick()
{
    try
    {
        StartRunning();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::TerminateProcessClick()
{
    try
    {
        cmajor::service::PutRequest(new cmajor::service::TerminateProcessRequest());
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::StopDebuggingClick()
{
    if (state != MainWindowState::debugging) return;
    cmajor::service::PutRequest(new cmajor::service::StopDebugServiceRequest());
}

void MainWindow::ShowNextStatementClick()
{
    try
    {
        HandleLocation(savedLocation, false, false); 
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::StepOverClick()
{
    try
    {
        ClearCallStack();
        ClearLocals();
        ResetSelections();
        if (state == MainWindowState::debugging)
        {
            StartDebugging();
            cmajor::service::PutRequest(new cmajor::service::NextDebugServiceRequest()); 
        }
        else
        {
            debugRequest.reset(new cmajor::service::NextDebugServiceRequest()); 
            if (!BuildActiveProject())
            {
                debugRequest.reset();
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::StepIntoClick()
{
    try
    {
        ClearCallStack();
        ClearLocals();
        ResetSelections();
        if (state == MainWindowState::debugging)
        {
            StartDebugging();
            cmajor::service::PutRequest(new cmajor::service::StepDebugServiceRequest()); 
        }
        else
        {
            debugRequest.reset(new cmajor::service::StepDebugServiceRequest()); 
            if (!BuildActiveProject())
            {
                debugRequest.reset();
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::StepOutClick()
{
    try
    {
        ClearCallStack();
        ClearLocals();
        ResetSelections();
        StartDebugging();
        cmajor::service::PutRequest(new cmajor::service::FinishDebugServiceRequest()); 
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::ToggleBreakpointClick()
{
    try
    {
        cmajor::view::Editor* editor = CurrentEditor();
        if (editor)
        {
            if (editor->IsCmajorEditor())
            {
                cmajor::view::CmajorEditor* cmajorEditor = static_cast<cmajor::view::CmajorEditor*>(editor);
                cmajor::view::CmajorSourceCodeView* cmajorSourceCodeView = cmajorEditor->SourceCodeView();
                if (cmajorSourceCodeView)
                {
                    cmajorSourceCodeView->ToggleBreakpoint();
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::ProgramArgumentsClick()
{
    try
    {
        if (state != MainWindowState::idle)
        {
            throw std::runtime_error("wrong state");
        }
        if (!solutionData)
        {
            throw std::runtime_error("no solution open");
        }
        cmajor::ast::Project* activeProject = solutionData->GetSolution()->ActiveProject();
        if (!activeProject)
        {
            throw std::runtime_error("no active project set for the solution");
        }
        if (!IsProgramTarget(activeProject->GetTarget()))
        {
            throw std::runtime_error("active project is a library project");
        }
        ProjectData* projectData = solutionData->GetProjectDataByProject(activeProject);
        if (!projectData)
        {
            throw std::runtime_error("active project has no data");
        }
        ProgramArgumentsDialog dialog;
        dialog.SetProgramArguments(projectData->ProgramArguments());
        if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
        {
            projectData->SetProgramArguments(dialog.ProgramArguments());
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::CloseAllTabsClick()
{
    try
    {
        codeTabControl->CloseAllTabPages();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::CloseExternalTabsClick()
{
    try
    {
        std::vector<wing::TabPage*> tabgPagesToClose;
        Component* child = codeTabControl->TabPages().FirstChild();
        while (child)
        {
            if (child->IsTabPage())
            {
                wing::TabPage* tabPage = static_cast<wing::TabPage*>(child);
                cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
                if (editor)
                {
                    SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(editor->FilePath());
                    if (!data || !data->project)
                    {
                        tabgPagesToClose.push_back(tabPage);
                    }
                }
            }
            child = child->NextSibling();
        }
        for (wing::TabPage* tabPage : tabgPagesToClose)
        {
            tabPage->Hide();
            tabPage->Close();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::HomepageClick()
{
    try
    {
        std::string homePage = "http://slaakko.github.io/cmajor/";
        ShellExecuteA(Handle(), "open", homePage.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::LocalDocumentationClick()
{
    try
    {
        std::string indexFilePath = util::Path::Combine(cmajor::ast::CmajorRootDir(), "doc/index.html");
        ShellExecuteA(Handle(), "open", indexFilePath.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::AboutClick()
{
    try
    {
        AboutDialog dialog;
        dialog.ShowDialog(*this);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::GotoPreviousLocationClick()
{
    try
    {
        locations.GotoPreviousLocation(CurrentLocation());
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::GotoNextLocationClick()
{
    try
    {
        locations.GotoNextLocation(CurrentLocation());
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::CppButtonClick()
{
    backend = "cpp";
    llvmToolButton->SetState(wing::ToolButtonState::normal);
    masmToolButton->SetState(wing::ToolButtonState::normal);
    cmToolButton->SetState(wing::ToolButtonState::normal);
    sbinToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveBackEnd(cmajor::ast::BackEnd::cpp);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::LlvmButtonClick()
{
    backend = "llvm";
    cppToolButton->SetState(wing::ToolButtonState::normal);
    masmToolButton->SetState(wing::ToolButtonState::normal);
    cmToolButton->SetState(wing::ToolButtonState::normal);
    sbinToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveBackEnd(cmajor::ast::BackEnd::llvm);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::MasmButtonClick()
{
    backend = "masm";
    cppToolButton->SetState(wing::ToolButtonState::normal);
    llvmToolButton->SetState(wing::ToolButtonState::normal);
    cmToolButton->SetState(wing::ToolButtonState::normal);
    sbinToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveBackEnd(cmajor::ast::BackEnd::masm);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::CmButtonClick()
{
    backend = "cm";
    cppToolButton->SetState(wing::ToolButtonState::normal);
    llvmToolButton->SetState(wing::ToolButtonState::normal);
    masmToolButton->SetState(wing::ToolButtonState::normal);
    sbinToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveBackEnd(cmajor::ast::BackEnd::cm);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::SBinButtonClick()
{
    backend = "sbin";
    cppToolButton->SetState(wing::ToolButtonState::normal);
    llvmToolButton->SetState(wing::ToolButtonState::normal);
    masmToolButton->SetState(wing::ToolButtonState::normal);
    cmToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveBackEnd(cmajor::ast::BackEnd::sbin);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::DebugButtonClick()
{
    config = "debug";
    releaseToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveConfig("debug");
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::ReleaseButtonClick()
{
    config = "release";
    debugToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveConfig("release");
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::ZeroButtonClick()
{
    optLevel = 0;
    optLevelOneToolButton->SetState(wing::ToolButtonState::normal);
    optLevelTwoToolButton->SetState(wing::ToolButtonState::normal);
    optLevelThreeToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveOptLevel(0);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::OneButtonClick()
{
    optLevel = 1;
    optLevelZeroToolButton->SetState(wing::ToolButtonState::normal);
    optLevelTwoToolButton->SetState(wing::ToolButtonState::normal);
    optLevelThreeToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveOptLevel(1);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::TwoButtonClick()
{
    optLevel = 2;
    optLevelZeroToolButton->SetState(wing::ToolButtonState::normal);
    optLevelOneToolButton->SetState(wing::ToolButtonState::normal);
    optLevelThreeToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveOptLevel(2);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::ThreeButtonClick()
{
    optLevel = 3;
    optLevelZeroToolButton->SetState(wing::ToolButtonState::normal);
    optLevelOneToolButton->SetState(wing::ToolButtonState::normal);
    optLevelTwoToolButton->SetState(wing::ToolButtonState::normal);
    SetState(state);
    LoadEditModuleForCurrentFile();
    if (solutionData && solutionData->GetSolution())
    {
        solutionData->GetSolution()->SetActiveOptLevel(3);
        solutionData->GetSolution()->Save();
    }
}

void MainWindow::StopBuildClick()
{
    StopBuild();
}

void MainWindow::ToggleCodeCompletionClick()
{
/*
    const Options& oldOptions = GetOptions();
    Options newOptions(oldOptions);
    if (oldOptions.codeCompletion)
    {
        newOptions.codeCompletion = false;
        StopCodeCompletion(true);
        codeCompletionStatusBarItem->SetText("Off");
    }
    else
    {
        newOptions.codeCompletion = true;
        StartCodeCompletion();
        LoadEditModule();
        codeCompletionStatusBarItem->SetText("On");
    }
    SetOptions(newOptions);
    SaveConfiguration();
*/
}

void MainWindow::TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args)
{
    try
    {
        wing::TreeViewNode* node = args.node;
        if (node->Data())
        {
            SolutionTreeViewNodeData* data = static_cast<SolutionTreeViewNodeData*>(node->Data());
            if (data->kind == SolutionTreeViewNodeDataKind::solution || data->kind == SolutionTreeViewNodeDataKind::project)
            {
                switch (node->State())
                {
                case wing::TreeViewNodeState::collapsed:
                {
                    node->ExpandAll();
                    break;
                }
                case wing::TreeViewNodeState::expanded:
                {
                    node->CollapseAll();
                    break;
                }
                }
            }
            else if (data->kind == SolutionTreeViewNodeDataKind::file)
            {
                std::string ext = util::Path::GetExtension(data->fileName);
                if (ext == ".cm")
                {
                    wing::TabPage* prevTabPage = codeTabControl->GetTabPageByKey(data->key);
                    if (prevTabPage)
                    {
                        codeTabControl->SetSelectedTabPage(prevTabPage);
                    }
                    else
                    {
                        AddCmajorEditor(data->fileName, data->key, data->filePath, data->project);
                    }
                }
                else if (ext == ".xml")
                {
                    wing::TabPage* prevTabPage = codeTabControl->GetTabPageByKey(data->key);
                    if (prevTabPage)
                    {
                        codeTabControl->SetSelectedTabPage(prevTabPage);
                    }
                    else
                    {
                        AddResourceFileEditor(data->fileName, data->key, data->filePath, data->project);
                    }
                }
                else
                {
                    wing::TabPage* prevTabPage = codeTabControl->GetTabPageByKey(data->key);
                    if (prevTabPage)
                    {
                        codeTabControl->SetSelectedTabPage(prevTabPage);
                    }
                    else
                    {
                        AddTextFileEditor(data->fileName, data->key, data->filePath, data->project);
                    }
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::TreeViewNodeClick(wing::TreeViewNodeClickEventArgs& args)
{
    try
    {
        wing::TreeViewNode* node = args.node;
        if (args.buttons == wing::MouseButtons::lbutton)
        {
            node->Select();
        }
        else if (args.buttons == wing::MouseButtons::rbutton)
        {
            clickActions.clear();
            SolutionTreeViewNodeData* data = static_cast<SolutionTreeViewNodeData*>(node->Data());
            switch (data->kind)
            {
            case SolutionTreeViewNodeDataKind::solution:
            {
                std::unique_ptr<wing::ContextMenu> contextMenu(new wing::ContextMenu());
                std::unique_ptr<wing::MenuItem> buildMenuItem(new wing::MenuItem("Build"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new BuildSolutionAction(buildMenuItem.get(), this)));
                contextMenu->AddMenuItem(buildMenuItem.release());
                std::unique_ptr<wing::MenuItem> rebuildMenuItem(new wing::MenuItem("Rebuild"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new RebuildSolutionAction(rebuildMenuItem.get(), this)));
                contextMenu->AddMenuItem(rebuildMenuItem.release());
                std::unique_ptr<wing::MenuItem> cleanMenuItem(new wing::MenuItem("Clean"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new CleanSolutionAction(cleanMenuItem.get(), this)));
                contextMenu->AddMenuItem(cleanMenuItem.release());
                std::unique_ptr<wing::MenuItem> addNewProjectMenuItem(new wing::MenuItem("Add New Project..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddNewProjectAction(addNewProjectMenuItem.get(), this)));
                contextMenu->AddMenuItem(addNewProjectMenuItem.release());
                std::unique_ptr<wing::MenuItem> addExistingProjectMenuItem(new wing::MenuItem("Add Existing Project..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddExistingProjectAction(addExistingProjectMenuItem.get(), this)));
                contextMenu->AddMenuItem(addExistingProjectMenuItem.release());
                std::unique_ptr<wing::MenuItem> openFileLocationMenuItem(new wing::MenuItem("Open File Location"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new OpenFileLocationAction(openFileLocationMenuItem.get(), this, data->solution->FilePath())));
                contextMenu->AddMenuItem(openFileLocationMenuItem.release());
                if (contextMenu->HasMenuItems())
                {
                    wing::Point contentLoc = args.location;
                    wing::TreeView* treeView = node->GetTreeView();
                    if (treeView)
                    {
                        wing::Point treeViewContentLocation = treeView->ContentLocation();
                        wing::Point loc(contentLoc.X, contentLoc.Y - treeViewContentLocation.Y);
                        wing::Point screenLoc = solutionExplorer->GetTreeView()->ClientToScreen(loc);
                        ShowContextMenu(contextMenu.release(), screenLoc);
                    }
                }
                break;
            }
            case SolutionTreeViewNodeDataKind::project:
            {
                cmajor::ast::Project* project = data->project;
                std::unique_ptr<wing::ContextMenu> contextMenu(new wing::ContextMenu());
                std::unique_ptr<wing::MenuItem> buildMenuItem(new wing::MenuItem("Build"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new BuildProjectAction(buildMenuItem.get(), this, project)));
                contextMenu->AddMenuItem(buildMenuItem.release());
                std::unique_ptr<wing::MenuItem> rebuildMenuItem(new wing::MenuItem("Rebuild"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new RebuildProjectAction(rebuildMenuItem.get(), this, project)));
                contextMenu->AddMenuItem(rebuildMenuItem.release());
                std::unique_ptr<wing::MenuItem> cleanMenuItem(new wing::MenuItem("Clean"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new CleanProjectAction(cleanMenuItem.get(), this, project)));
                contextMenu->AddMenuItem(cleanMenuItem.release());
                std::unique_ptr<wing::MenuItem> addNewSourceFileMenuItem(new wing::MenuItem("Add New Source File..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddNewSourceFileAction(addNewSourceFileMenuItem.get(), this, project, node)));
                contextMenu->AddMenuItem(addNewSourceFileMenuItem.release());
                std::unique_ptr<wing::MenuItem> addExistingSourceFileMenuItem(new wing::MenuItem("Add Existing Source File..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddExistingSourceFileAction(addExistingSourceFileMenuItem.get(), this, project, node)));
                contextMenu->AddMenuItem(addExistingSourceFileMenuItem.release());
                std::unique_ptr<wing::MenuItem> addNewResourceFileMenuItem(new wing::MenuItem("Add New Resource File..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddNewResourceFileAction(addNewResourceFileMenuItem.get(), this, project, node)));
                contextMenu->AddMenuItem(addNewResourceFileMenuItem.release());
                std::unique_ptr<wing::MenuItem> addExistingResouceFileMenuItem(new wing::MenuItem("Add Existing Resource File..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddExistingResourceFileAction(addExistingResouceFileMenuItem.get(), this, project, node)));
                contextMenu->AddMenuItem(addExistingResouceFileMenuItem.release());
                std::unique_ptr<wing::MenuItem> addNewTextFileMenuItem(new wing::MenuItem("Add New Text File..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddNewTextFileAction(addNewTextFileMenuItem.get(), this, project, node)));
                contextMenu->AddMenuItem(addNewTextFileMenuItem.release());
                std::unique_ptr<wing::MenuItem> addExistingTextFileMenuItem(new wing::MenuItem("Add Existing Text File..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new AddExistingTextFileAction(addExistingTextFileMenuItem.get(), this, project, node)));
                contextMenu->AddMenuItem(addExistingTextFileMenuItem.release());
                std::unique_ptr<wing::MenuItem> referencesMenuItem(new wing::MenuItem("Rererences..."));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new ProjectReferencesAction(referencesMenuItem.get(), this, project)));
                contextMenu->AddMenuItem(referencesMenuItem.release());
                if (solutionData->GetSolution()->ActiveProject() != project)
                {
                    std::unique_ptr<wing::MenuItem> setActiveMenuItem(new wing::MenuItem("Set Active"));
                    clickActions.push_back(std::unique_ptr<wing::ClickAction>(new SetActiveProjectAction(setActiveMenuItem.get(), this, project, node)));
                    contextMenu->AddMenuItem(setActiveMenuItem.release());
                }
                std::unique_ptr<wing::MenuItem> openFileLocationMenuItem(new wing::MenuItem("Open File Location"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new OpenFileLocationAction(openFileLocationMenuItem.get(), this, project->FilePath())));
                contextMenu->AddMenuItem(openFileLocationMenuItem.release());
                std::unique_ptr<wing::MenuItem> removeMenuItem(new wing::MenuItem("Remove"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new RemoveProjectAction(removeMenuItem.get(), this, project)));
                contextMenu->AddMenuItem(removeMenuItem.release());
                if (contextMenu->HasMenuItems())
                {
                    wing::Point contentLoc = args.location;
                    wing::TreeView* treeView = node->GetTreeView();
                    if (treeView)
                    {
                        wing::Point treeViewContentLocation = treeView->ContentLocation();
                        wing::Point loc(contentLoc.X, contentLoc.Y - treeViewContentLocation.Y);
                        wing::Point screenLoc = solutionExplorer->GetTreeView()->ClientToScreen(loc);
                        ShowContextMenu(contextMenu.release(), screenLoc);
                    }
                }
                break;
            }
            case SolutionTreeViewNodeDataKind::file:
            {
                cmajor::ast::Project* project = data->project;
                std::unique_ptr<wing::ContextMenu> contextMenu(new wing::ContextMenu());
                std::unique_ptr<wing::MenuItem> openFileLocationMenuItem(new wing::MenuItem("Open File Location"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new OpenFileLocationAction(openFileLocationMenuItem.get(), this, data->filePath)));
                contextMenu->AddMenuItem(openFileLocationMenuItem.release());
                std::unique_ptr<wing::MenuItem> removeMenuItem(new wing::MenuItem("Remove"));
                clickActions.push_back(std::unique_ptr<wing::ClickAction>(new RemoveFileAction(removeMenuItem.get(), this, data->project, data->filePath, data->fileName, node)));
                contextMenu->AddMenuItem(removeMenuItem.release());
                if (contextMenu->HasMenuItems())
                {
                    wing::Point contentLoc = args.location;
                    wing::TreeView* treeView = node->GetTreeView();
                    if (treeView)
                    {
                        wing::Point treeViewContentLocation = treeView->ContentLocation();
                        wing::Point loc(contentLoc.X, contentLoc.Y - treeViewContentLocation.Y);
                        wing::Point screenLoc = solutionExplorer->GetTreeView()->ClientToScreen(loc);
                        ShowContextMenu(contextMenu.release(), screenLoc);
                    }
                }
                break;
            }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::TreeViewNodeExpanded(wing::TreeViewNodeEventArgs& args)
{
    try
    {
        wing::TreeViewNode* node = args.node;
        SolutionTreeViewNodeData* data = static_cast<SolutionTreeViewNodeData*>(node->Data());
        if (data->kind == SolutionTreeViewNodeDataKind::project)
        {
            solutionData->AddExpandedProject(util::ToUtf8(data->project->Name()));
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::TreeViewNodeCollapsed(wing::TreeViewNodeEventArgs& args)
{
    try
    {
        wing::TreeViewNode* node = args.node;
        SolutionTreeViewNodeData* data = static_cast<SolutionTreeViewNodeData*>(node->Data());
        if (data->kind == SolutionTreeViewNodeDataKind::project)
        {
            solutionData->RemoveExpandedProject(util::ToUtf8(data->project->Name()));
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::UpdateColors()
{
    wing::Window::UpdateColors();
    AddToolButtons();
    if (!solutionData) return;
    cmajor::ast::Solution* solution = solutionData->GetSolution();
    if (!solution) return;
    std::string solutionFilePath = solution->FilePath();
    if (CloseSolution())
    {
        OpenProject(solutionFilePath);
    }
}

cmajor::view::Editor* MainWindow::GetEditorByTabPage(wing::TabPage* tabPage) const
{
    auto it = tabPageEditorMap.find(tabPage);
    if (it != tabPageEditorMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

cmajor::view::CmajorEditor* MainWindow::AddCmajorEditor(const std::string& fileName, const std::string& key, const std::string& filePath, cmajor::ast::Project* project)
{
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage(fileName, key));
    cmajor::debugger::BreakpointList* breakpointList = nullptr;
    if (project)
    {
        ProjectData* projectData = solutionData->GetProjectDataByProject(project);
        if (projectData)
        {
            breakpointList = &projectData->GetBreakpointCollection().GetBreakpointList(filePath);
        }
    }
    if (!breakpointList)
    {
        breakpointList = &solutionData->GetSolutionBreakpointCollection().GetBreakpointList(filePath);
    }
    cmajor::view::CmajorEditorCreateParams cmajorEditorCreateParams(filePath, cmajor::view::CmajorSourceCodeViewCreateParams().BackgroundColor(wing::GetColor("cmajor.editor.background")),
        cmajor::view::DebugStripCreateParams(breakpointList).BackgroundColor(wing::GetColor("debug.strip.background")).BreakpointColor(wing::GetColor("debug.strip.break.point")));
    cmajorEditorCreateParams.editorCreateParams.BackgroundColor(wing::GetColor("cmajor.editor.background"));
    std::unique_ptr<cmajor::view::CmajorEditor> editorPtr(new cmajor::view::CmajorEditor(cmajorEditorCreateParams));
    cmajor::view::CmajorEditor* editor = editorPtr.get();
    cmajor::view::CmajorSourceCodeView* sourceCodeView = editor->SourceCodeView();
    if (sourceCodeView)
    {
        sourceCodeView->DirtyChanged().AddHandler(this, &MainWindow::EditorDirtyChanged);
        sourceCodeView->CCDirtyChanged().AddHandler(this, &MainWindow::EditorCCDirtyChanged);
        sourceCodeView->ReadOnlyChanged().AddHandler(this, &MainWindow::EditorReadOnlyChanged);
        sourceCodeView->CaretPosChanged().AddHandler(this, &MainWindow::EditorCaretPosChanged);
        sourceCodeView->SelectionChanged().AddHandler(this, &MainWindow::EditorSelectionChanged);
        sourceCodeView->RightClick().AddHandler(this, &MainWindow::EditorRightClick);
        sourceCodeView->Copy().AddHandler(this, &MainWindow::CopyClick);
        sourceCodeView->Cut().AddHandler(this, &MainWindow::CutClick);
        sourceCodeView->Paste().AddHandler(this, &MainWindow::PasteClick);
        sourceCodeView->ExpressionHover().AddHandler(this, &MainWindow::ExpressionHover);
        sourceCodeView->SetUndoRedoMenuItems(undoMenuItem, redoMenuItem);
        //sourceCodeView->CCTextChanged().AddHandler(this, &MainWindow::CCTextChanged);
        //sourceCodeView->CC().AddHandler(this, &MainWindow::CCShow);
        //sourceCodeView->EscapePressed().AddHandler(this, &MainWindow::CCEscape);
        //sourceCodeView->CCNext().AddHandler(this, &MainWindow::CCNext);
        //sourceCodeView->CCPrev().AddHandler(this, &MainWindow::CCPrev);
        //sourceCodeView->CCNextPage().AddHandler(this, &MainWindow::CCNextPage);
        //sourceCodeView->CCPrevPage().AddHandler(this, &MainWindow::CCPrevPage);
        //sourceCodeView->CCSelect().AddHandler(this, &MainWindow::CCSelect);
        //sourceCodeView->CCStart().AddHandler(this, &MainWindow::CCStart);
        //sourceCodeView->CCStop().AddHandler(this, &MainWindow::CCStop);
        //sourceCodeView->ParamHelpNext().AddHandler(this, &MainWindow::ParamHelpNext);
        //sourceCodeView->ParamHelpPrev().AddHandler(this, &MainWindow::ParamHelpPrev);
        //sourceCodeView->ParamHelpClose().AddHandler(this, &MainWindow::ParamHelpClose);
        //sourceCodeView->ContentLocationChanged().AddHandler(this, &MainWindow::CCViewLocationChanged);
        if (state != MainWindowState::idle)
        {
            sourceCodeView->SetReadOnly();
        }
        cmajor::view::DebugStrip* debugStrip = sourceCodeView->GetDebugStrip();
        if (debugStrip)
        {
            debugStrip->ChangeBreakpoints().AddHandler(this, &MainWindow::ChangeBreakpoints);
            debugStrip->BreakpointAdded().AddHandler(this, &MainWindow::BreakpointAdded);
            debugStrip->BreakpointRemoved().AddHandler(this, &MainWindow::BreakpointRemoved);
        }
    }
    tabPage->AddChild(editorPtr.release());
    tabPageEditorMap[tabPage.get()] = editor;
    codeTabControl->AddTabPage(tabPage.release());
    SetEditorState();
    SetFocusToEditor();
    solutionData->AddOpenFile(filePath);
    return editor;
}

cmajor::view::ResourceFileEditor* MainWindow::AddResourceFileEditor(const std::string& fileName, const std::string& key, const std::string& filePath, cmajor::ast::Project* project)
{
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage(fileName, key));
    cmajor::view::ResourceFileEditorCreateParams resourceFileEditorCreateParams(filePath);
    resourceFileEditorCreateParams.textViewCreateParams.TextColor(wing::GetColor("resource.editor.text")).BackgroundColor(wing::GetColor("resource.editor.background"));
    std::unique_ptr<cmajor::view::ResourceFileEditor> editorPtr(new cmajor::view::ResourceFileEditor(resourceFileEditorCreateParams));
    cmajor::view::ResourceFileEditor* editor = editorPtr.get();
    wing::TextView* textView = editor->GetTextView();
    if (textView)
    {
        textView->DirtyChanged().AddHandler(this, &MainWindow::EditorDirtyChanged);
        textView->ReadOnlyChanged().AddHandler(this, &MainWindow::EditorReadOnlyChanged);
        textView->CaretPosChanged().AddHandler(this, &MainWindow::EditorCaretPosChanged);
        textView->SelectionChanged().AddHandler(this, &MainWindow::EditorSelectionChanged);
        textView->Copy().AddHandler(this, &MainWindow::CopyClick);
        textView->Cut().AddHandler(this, &MainWindow::CutClick);
        textView->Paste().AddHandler(this, &MainWindow::PasteClick);
        textView->SetUndoRedoMenuItems(undoMenuItem, redoMenuItem);
        if (state != MainWindowState::idle)
        {
            textView->SetReadOnly();
        }
    }
    tabPage->AddChild(editorPtr.release());
    tabPageEditorMap[tabPage.get()] = editor;
    codeTabControl->AddTabPage(tabPage.release());
    SetEditorState();
    SetFocusToEditor();
    solutionData->AddOpenFile(filePath);
    return editor;
}

cmajor::view::TextFileEditor* MainWindow::AddTextFileEditor(const std::string& fileName, const std::string& key, const std::string& filePath, cmajor::ast::Project* project)
{
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage(fileName, key));
    cmajor::view::TextFileEditorCreateParams textFileEditorCreateParams(filePath);
    textFileEditorCreateParams.textViewCreateParams.TextColor(wing::GetColor("text.editor.text")).BackgroundColor(wing::GetColor("text.editor.background"));
    std::unique_ptr<cmajor::view::TextFileEditor> editorPtr(new cmajor::view::TextFileEditor(textFileEditorCreateParams));
    cmajor::view::TextFileEditor* editor = editorPtr.get();
    wing::TextView* textView = editor->GetTextView();
    if (textView)
    {
        textView->DirtyChanged().AddHandler(this, &MainWindow::EditorDirtyChanged);
        textView->ReadOnlyChanged().AddHandler(this, &MainWindow::EditorReadOnlyChanged);
        textView->CaretPosChanged().AddHandler(this, &MainWindow::EditorCaretPosChanged);
        textView->SelectionChanged().AddHandler(this, &MainWindow::EditorSelectionChanged);
        textView->Copy().AddHandler(this, &MainWindow::CopyClick);
        textView->Cut().AddHandler(this, &MainWindow::CutClick);
        textView->Paste().AddHandler(this, &MainWindow::PasteClick);
        textView->SetUndoRedoMenuItems(undoMenuItem, redoMenuItem);
        if (state != MainWindowState::idle)
        {
            textView->SetReadOnly();
        }
    }
    tabPage->AddChild(editorPtr.release());
    tabPageEditorMap[tabPage.get()] = editor;
    codeTabControl->AddTabPage(tabPage.release());
    SetEditorState();
    SetFocusToEditor();
    solutionData->AddOpenFile(filePath);
    return editor;
}

void MainWindow::AddFilePathsToProject(const std::string& newSourceFilePath, const std::string& newResourceFilePath, const std::string& newTextFilePath, cmajor::ast::Project* project, wing::TreeViewNode* projectNode)
{
    projectNode->RemoveChildren();
    cmajor::ast::Solution* solution = solutionData->GetSolution();
    for (const std::string& sourceFilePath : project->SourceFilePaths())
    {
        SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(sourceFilePath);
        if (data)
        {
            std::unique_ptr<wing::TreeViewNode> fileNode(new wing::TreeViewNode(data->fileName));
            fileNode->SetData(data);
            fileNode->SetImageIndex(imageList.GetImageIndex(wing::GetBitmapName("file")));
            projectNode->AddChild(fileNode.release());
        }
        else
        {
            std::unique_ptr<SolutionTreeViewNodeData> newData(new SolutionTreeViewNodeData(SolutionTreeViewNodeDataKind::file, solution, project, sourceFilePath, 
                util::Path::GetFileName(sourceFilePath)));
            data = newData.get();
            std::unique_ptr<wing::TreeViewNode> fileNode(new wing::TreeViewNode(newData->fileName));
            fileNode->SetData(newData.get());
            fileNode->SetImageIndex(imageList.GetImageIndex(wing::GetBitmapName("file")));
            projectNode->AddChild(fileNode.release());
            solutionData->AddTreeViewNodeData(newData.release());
        }
        if (sourceFilePath == newSourceFilePath)
        {
            {
                std::ofstream file(sourceFilePath);
            }
            AddCmajorEditor(data->fileName, data->key, data->filePath, project);
        }
    }
    for (const std::string& resourceFilePath : project->ResourceFilePaths())
    {
        SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(resourceFilePath);
        if (data)
        {
            std::unique_ptr<wing::TreeViewNode> fileNode(new wing::TreeViewNode(data->fileName));
            fileNode->SetData(data);
            fileNode->SetImageIndex(imageList.GetImageIndex(wing::GetBitmapName("xmlfile")));
            projectNode->AddChild(fileNode.release());
        }
        else
        {
            std::unique_ptr<SolutionTreeViewNodeData> newData(new SolutionTreeViewNodeData(SolutionTreeViewNodeDataKind::file, solution, project, resourceFilePath, 
                util::Path::GetFileName(resourceFilePath)));
            data = newData.get();
            std::unique_ptr<wing::TreeViewNode> fileNode(new wing::TreeViewNode(newData->fileName));
            fileNode->SetData(newData.get());
            fileNode->SetImageIndex(imageList.GetImageIndex(wing::GetBitmapName("xmlfile")));
            projectNode->AddChild(fileNode.release());
            solutionData->AddTreeViewNodeData(newData.release());
        }
        if (resourceFilePath == newResourceFilePath)
        {
            std::ofstream file(resourceFilePath);
            AddResourceFileEditor(data->fileName, data->key, data->filePath, project);
        }
    }
    for (const std::string& textFilePath : project->TextFilePaths())
    {
        SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(textFilePath);
        if (data)
        {
            std::unique_ptr<wing::TreeViewNode> fileNode(new wing::TreeViewNode(data->fileName));
            fileNode->SetData(data);
            fileNode->SetImageIndex(imageList.GetImageIndex(wing::GetBitmapName("file")));
            projectNode->AddChild(fileNode.release());
        }
        else
        {
            std::unique_ptr<SolutionTreeViewNodeData> newData(new SolutionTreeViewNodeData(SolutionTreeViewNodeDataKind::file, solution, project, textFilePath, 
                util::Path::GetFileName(textFilePath)));
            data = newData.get();
            std::unique_ptr<wing::TreeViewNode> fileNode(new wing::TreeViewNode(data->fileName));
            fileNode->SetData(data);
            fileNode->SetImageIndex(imageList.GetImageIndex(wing::GetBitmapName("file")));
            projectNode->AddChild(fileNode.release());
            solutionData->AddTreeViewNodeData(newData.release());
        }
        if (textFilePath == newTextFilePath)
        {
            std::ofstream textFile(textFilePath);
            AddTextFileEditor(data->fileName, data->key, data->filePath, project);
        }
    }
    projectNode->Expand();
    solutionExplorer->GetTreeView()->SetChanged();
    solutionExplorer->GetTreeView()->Invalidate();
}

void MainWindow::CodeTabPageSelected()
{
    wing::TabPage* selectedTabPage = codeTabControl->SelectedTabPage();
    cmajor::view::Editor* editor = GetEditorByTabPage(selectedTabPage);
    if (editor)
    {
        editor->Select();
        SetEditorState();
        LoadEditModuleForCurrentFile();
    }
}

void MainWindow::CodeTabPageRemoved(wing::ControlEventArgs& args)
{
    SetEditorState();
    //CCHide(); TODO
    //ParamHelpClose(); TODO
    Control* removedControl = args.control;
    if (removedControl->IsTabPage())
    {
        wing::TabPage* removedTabPage = static_cast<wing::TabPage*>(removedControl);
        cmajor::view::Editor* editor = GetEditorByTabPage(removedTabPage);
        if (editor)
        {
            if (editor->IsDirty())
            {
                wing::MessageBoxResult result = wing::MessageBox::Show(editor->FilePath() + " is modified. Save changes?", "Question", this, MB_YESNO);
                if (result == wing::MessageBoxResult::yes)
                {
                    editor->Save();
                }
            }
            solutionData->RemoveOpenFile(editor->FilePath());
            if (editor->FilePath() == solutionData->CurrentOpenFile())
            {
                solutionData->SetCurrentOpenFile(std::string());
            }
        }
        tabPageEditorMap.erase(removedTabPage);
    }
    if (codeTabControl->TabPages().IsEmpty())
    {
        sourceFilePathStatusBarItem->SetText("");
        ResetFocusedControl();
    }
}

void MainWindow::OutputTabControlTabPageRemoved(wing::ControlEventArgs& args)
{
    if (args.control == outputTabPage)
    {
        outputTabPage = nullptr;
        outputLogView = nullptr;
    }
    else if (args.control == errorTabPage)
    {
        errorTabPage = nullptr;
        errorView = nullptr;
    }
    else if (args.control == searchResultsTabPage)
    {
        searchResultsTabPage = nullptr;
        searchResultsView = nullptr;
    }
    else if (args.control == callStackTabPage)
    {
        callStackTabPage = nullptr;
        callStackView = nullptr;
    }
    else if (args.control == localsTabPage)
    {
        localsTabPage = nullptr;
        localsView = nullptr;
    }
    else if (args.control == logTabPage)
    {
        logTabPage = nullptr;
        log = nullptr;
    }
    else if (args.control == consoleTabPage)
    {
        consoleTabPage = nullptr;
        console = nullptr;
    }
    else if (args.control == debugTabPage)
    {
        debugTabPage = nullptr;
        debugLog = nullptr;
    }
    SaveSolutionData();
}

void MainWindow::OutputTabControlTabPageSelected()
{
    wing::TabPage* tabPage = outputTabControl->SelectedTabPage();
    if (tabPage == callStackTabPage)
    {
        if (state == MainWindowState::debugging)
        {
            UpdateCallStack(true);
        }
    }
    else if (tabPage == localsTabPage)
    {
        if (state == MainWindowState::debugging)
        {
            ClearLocals();
            UpdateLocals();
        }
    }
}

wing::LogView* MainWindow::GetOutputLogView()
{
    if (!outputLogView)
    {
        std::unique_ptr<wing::TabPage> outputTabPagePtr(new wing::TabPage("Output", "output"));
        outputTabPage = outputTabPagePtr.get();
        std::unique_ptr<wing::LogView> outputLogViewPtr(
            new wing::LogView(wing::TextViewCreateParams().SelectionBackgroundColor(wing::GetColor("selection.background")).BackgroundColor(wing::GetColor("output.log.background")).
                TextColor(wing::GetColor("output.log.text"))));
        outputLogView = outputLogViewPtr.get();
        outputLogView->SetFlag(wing::ControlFlags::scrollSubject);
        outputLogView->SetDoubleBuffered();
        outputLogView->SetBackgroundItemName("output.log.background");
        outputLogView->SetTextItemName("output.log.text");
        std::unique_ptr<wing::Control> scrollableOutputLogView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(outputLogViewPtr.release()).SetDock(wing::Dock::fill)));
        outputTabPage->AddChild(scrollableOutputLogView.release());
        outputTabControl->AddTabPage(outputTabPagePtr.release());
    }
    outputTabPage->Select();
    return outputLogView;
}

cmajor::view::ErrorView* MainWindow::GetErrorView()
{
    if (!errorView)
    {
        std::unique_ptr<wing::TabPage> errorTabPagePtr(new wing::TabPage("Errors", "errors"));
        errorTabPage = errorTabPagePtr.get();
        std::unique_ptr<cmajor::view::ErrorView> errorViewPtr(new cmajor::view::ErrorView(cmajor::view::ErrorViewCreateParams().Defaults()));
        errorView = errorViewPtr.get();
        errorView->ViewError().AddHandler(this, &MainWindow::ViewError);
        errorTabPage->AddChild(errorViewPtr.release());
        outputTabControl->AddTabPage(errorTabPagePtr.release());
    }
    errorTabPage->Select();
    return errorView;
}

cmajor::view::WarningsView* MainWindow::GetWarningsView()
{
    if (!warningsView)
    {
        std::unique_ptr<wing::TabPage> warningsTabPagePtr(new wing::TabPage("Warnings", "warnings"));
        warningsTabPage = warningsTabPagePtr.get();
        std::unique_ptr<cmajor::view::WarningsView> warningsViewPtr(new cmajor::view::WarningsView(cmajor::view::WarningsViewCreateParams().Defaults()));
        warningsView = warningsViewPtr.get();
        warningsView->ViewWarning().AddHandler(this, &MainWindow::ViewWarning);
        warningsTabPage->AddChild(warningsViewPtr.release());
        outputTabControl->AddTabPage(warningsTabPagePtr.release());
    }
    warningsTabPage->Select();
    return warningsView;
}

void MainWindow::ViewError(cmajor::view::ViewErrorArgs& args)
{
    try
    {
        cmajor::info::bs::CompileError* error = args.error;
        if (error)
        {
            wing::TabPage* tabPage = codeTabControl->GetTabPageByKey(error->file);
            cmajor::view::Editor* editor = nullptr;
            if (tabPage)
            {
                editor = GetEditorByTabPage(tabPage);
                tabPage->Select();
            }
            if (!editor)
            {
                SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(error->file);
                if (data)
                {
                    editor = AddCmajorEditor(data->fileName, data->key, data->filePath, data->project);
                }
                else
                {
                    editor = AddCmajorEditor(util::Path::GetFileName(error->file), error->file, error->file, nullptr);
                }
            }
            wing::TextView* textView = editor->GetTextView();
            if (textView)
            {
                int line = error->line;
                int scol = error->scol;
                int ecol = error->ecol;
                textView->EnsureLineVisible(line);
                textView->SetCaretLineCol(std::min(line, static_cast<int>(textView->Lines().size())), 1 + textView->LineNumberFieldLength());
                textView->ScrollToCaret();
                textView->SetFocus();
                textView->Invalidate();
                if (scol != 0 && ecol != 0 && scol != ecol)
                {
                    textView->ResetSelection();
                    soul::ast::SourcePos start(line, scol);
                    soul::ast::SourcePos end(line, ecol);
                    textView->ExtendSelection(start, end);
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::ViewWarning(cmajor::view::ViewWarningArgs& args)
{
    try
    {
        cmajor::info::bs::Warning* warning = args.warning;
        if (warning)
        {
            wing::TabPage* tabPage = codeTabControl->GetTabPageByKey(warning->file);
            cmajor::view::Editor* editor = nullptr;
            if (tabPage)
            {
                editor = GetEditorByTabPage(tabPage);
                tabPage->Select();
            }
            if (!editor)
            {
                SolutionTreeViewNodeData* data = solutionData->GetSolutionTreeViewNodeDataByKey(warning->file);
                if (data)
                {
                    editor = AddCmajorEditor(data->fileName, data->key, data->filePath, data->project);
                }
                else
                {
                    editor = AddCmajorEditor(util::Path::GetFileName(warning->file), warning->file, warning->file, nullptr);
                }
            }
            wing::TextView* textView = editor->GetTextView();
            if (textView)
            {
                int line = warning->line;
                int scol = warning->scol;
                int ecol = warning->ecol;
                textView->EnsureLineVisible(line);
                textView->SetCaretLineCol(std::min(line, static_cast<int>(textView->Lines().size())), 1 + textView->LineNumberFieldLength());
                textView->ScrollToCaret();
                textView->SetFocus();
                textView->Invalidate();
                if (scol != 0 && ecol != 0 && scol != ecol)
                {
                    textView->ResetSelection();
                    soul::ast::SourcePos start(line, scol);
                    soul::ast::SourcePos end(line, ecol);
                    textView->ExtendSelection(start, end);
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

cmajor::view::Editor* MainWindow::CurrentEditor() const
{
    if (codeTabControl->TabPages().IsEmpty()) return nullptr;
    wing::TabPage* selectedTabPage = codeTabControl->SelectedTabPage();
    if (selectedTabPage)
    {
        cmajor::view::Editor* editor = GetEditorByTabPage(selectedTabPage);
        if (editor)
        {
            return editor;
        }
    }
    return nullptr;
}

cmajor::view::SearchResultsView* MainWindow::GetSearchResultsView()
{
    if (!searchResultsView)
    {
        searchResultsView = new cmajor::view::SearchResultsView();
        searchResultsView->ViewSearchResult().AddHandler(this, &MainWindow::ViewSearchResult);
        searchResultsTabPage = new wing::TabPage("Search Results", "searchResults");
        searchResultsTabPage->AddChild(searchResultsView);
        outputTabControl->AddTabPage(searchResultsTabPage);
    }
    searchResultsTabPage->Select();
    return searchResultsView;
}

cmajor::view::CallStackView* MainWindow::GetCallStackView()
{
    if (!callStackView)
    {
        callStackView = new cmajor::view::CallStackView();
        callStackView->FrameSelected().AddHandler(this, &MainWindow::CallStackFrameSelected);
        if (state == MainWindowState::debugging)
        {
            UpdateCallStack(true);
        }
        callStackTabPage = new wing::TabPage("Call Stack", "callStack");
        callStackTabPage->AddChild(callStackView);
        outputTabControl->AddTabPage(callStackTabPage);
    }
    callStackTabPage->Select();
    callStackOpen = true;
    return callStackView;
}

void MainWindow::ClearCallStack()
{
    if (signalReceived) return;
    callStackDepth = -1;
    if (callStackView)
    {
        callStackView->Clear();
    }
}

void MainWindow::UpdateCallStack(bool fetchDepth)
{
    if (!callStackView) return;
    if (fetchDepth)
    {
        callStackDepth = -1;
    }
    if (callStackDepth == -1)
    {
        cmajor::service::PutRequest(new cmajor::service::DepthDebugServiceRequest()); 
    }
    else if (callStackDepth >= 0)
    {
        callStackView->SetDepth(callStackDepth);
        std::pair<int, int> frameRange = callStackView->GetFrameRange();
        if (frameRange.first != -1 && frameRange.second != -1)
        {
            cmajor::service::PutRequest(new cmajor::service::FramesDebugServiceRequest(frameRange.first, frameRange.second)); 
        }
    }
}

void MainWindow::CallStackFrameSelected(cmajor::view::FrameSelectedEventArgs& args)
{
    try
    {
        HandleLocation(*args.frame, false, true);  
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

cmajor::view::LocalsView* MainWindow::GetLocalsView()
{
    if (!localsView)
    {
        localsView = new cmajor::view::LocalsView();
        localsView->LocalNodeExpanded().AddHandler(this, &MainWindow::LocalsViewNodeExpanded);
        localsView->UpdateNeeded().AddHandler(this, &MainWindow::LocalsViewUpdateNeeded);
        localsTabPage = new wing::TabPage("Locals", "locals");
        localsTabPage->AddChild(localsView);
        outputTabControl->AddTabPage(localsTabPage);
    }
    localsTabPage->Select();
    localsViewOpen = true;
    return localsView;
}

void MainWindow::ClearLocals()
{
    if (localsView)
    {
        localsView->Clear();
    }
}

void MainWindow::UpdateLocals()
{
    if (!localsView) return;
    int localCount = localsView->LocalCount();
    if (localCount == -1)
    {
        if (!localsView->LocalCountRequested())
        {
            localsView->SetLocalCountRequested();
            cmajor::service::PutRequest(new cmajor::service::CountDebugServiceRequest("@locals")); 
        }
    }
    else
    {
        if (!localsView->ChildExtentRequested())
        {
            cmajor::view::ChildExtent childExtent = localsView->GetChildExtent();
            if (!childExtent.IsEmpty())
            {
                localsView->SetChildExtentRequested();
                cmajor::service::PutRequest(new cmajor::service::EvaluateChildDebugServiceRequest(localsView->FetchExpression(), childExtent.start, childExtent.count)); 
            }
        }
        else
        {
            localsView->ResetChildExtentRequested();
            if (!localsView->IsFetchSetEmpty())
            {
                localsView->Fetch();
                cmajor::view::ChildExtent childExtent = localsView->GetChildExtent();
                if (!childExtent.IsEmpty())
                {
                    localsView->SetChildExtentRequested();
                    cmajor::service::PutRequest(new cmajor::service::EvaluateChildDebugServiceRequest(localsView->FetchExpression(), childExtent.start, childExtent.count)); 
                }
            }
        }
        localsView->Update();
    }
}

void MainWindow::LocalsViewNodeExpanded(wing::TreeViewNodeEventArgs& args)
{
    try
    {
        if (!localsView) return;
        UpdateLocals();
        localsView->UpdateFetchSet();
        if (!localsView->IsFetchSetEmpty())
        {
            localsView->Fetch();
        }
        UpdateLocals();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::LocalsViewUpdateNeeded()
{
    try
    {
        if (!localsView) return;
        UpdateLocals();
        localsView->UpdateFetchSet();
        if (!localsView->IsFetchSetEmpty())
        {
            localsView->Fetch();
        }
        UpdateLocals();
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

wing::Console* MainWindow::GetConsole()
{
    if (!console)
    {
        console = new wing::Console(wing::ConsoleCreateParams().Defaults());
        console->SetFlag(wing::ControlFlags::scrollSubject);
        console->SetDoubleBuffered();
        console->ConsoleInputReady().AddHandler(this, &MainWindow::ConsoleInputReady); 
        wing::ScrollableControl* scrollableConsole = new wing::ScrollableControl(wing::ScrollableControlCreateParams(console).SetDock(wing::Dock::fill));
        consoleTabPage = new wing::TabPage("Console", "console");
        consoleTabPage->AddChild(scrollableConsole);
        outputTabControl->AddTabPage(consoleTabPage);
    }
    consoleTabPage->Select();
    return console;
}

wing::LogView* MainWindow::GetDebugLog()
{
    if (!debugLog)
    {
        debugLog = new wing::LogView(wing::TextViewCreateParams().BackgroundColor(wing::GetColor("log.view.background")).TextColor(wing::GetColor("log.view.text")));
        debugLog->SetFlag(wing::ControlFlags::scrollSubject);
        debugLog->SetDoubleBuffered();
        wing::ScrollableControl* scrollableLog = new wing::ScrollableControl(wing::ScrollableControlCreateParams(debugLog).SetDock(wing::Dock::fill));
        debugTabPage = new wing::TabPage("Debug", "debug");
        debugTabPage->AddChild(scrollableLog);
        outputTabControl->AddTabPage(debugTabPage);
    }
    return debugLog;
}

void MainWindow::UpdateCurrentDebugStrip()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        if (editor->IsCmajorEditor())
        {
            cmajor::view::CmajorEditor* cmajorEditor = static_cast<cmajor::view::CmajorEditor*>(editor);
            cmajor::view::DebugStrip* debugStrip = cmajorEditor->GetDebugStrip();
            if (debugStrip)
            {
                debugStrip->Update();
            }
        }
    }
}

void MainWindow::ResetSelections()
{
    Component* child = codeTabControl->TabPages().FirstChild();
    while (child)
    {
        if (child->IsTabPage())
        {
            wing::TabPage* tabPage = static_cast<wing::TabPage*>(child);
            cmajor::view::Editor* editor = GetEditorByTabPage(tabPage);
            if (editor)
            {
                wing::TextView* textView = editor->GetTextView();
                if (textView)
                {
                    textView->ResetSelection();
                }
            }
        }
        child = child->NextSibling();
    }
}

void MainWindow::ExpressionHover(cmajor::view::ExpressionHoverEventArgs& args)
{
    if (state != MainWindowState::debugging) return;
    int requestId = expressionEvaluateRequests.size();
    ExpressionEvaluateRequest request(args.expression, args.screenLoc);
    expressionEvaluateRequests.push_back(request);
    cmajor::service::PutRequest(new cmajor::service::EvaluateDebugServiceRequest(args.expression, requestId)); 
}

/*  TODO
void MainWindow::CCEscape()
{
    CCHide();
    ParamHelpClose();
}

void MainWindow::CCTextChanged()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        if (!editor->GetTextView()->IsCCActive()) return;
        if (!editModuleLoaded)
        {
            PutOutputServiceMessage("loading edit module...");
            LoadEditModule();
        }
        cmajor::ast::Project* project = CurrentProject();
        if (project)
        {
            ccCanSelect = false;
            GetCCList(project->FilePath(), backend, config, editor->FilePath(), editor->GetTextView()->GetCCText(), ruleContext, editor->GetTextView()->CursorLine());
        }
    }
}

void MainWindow::CCShow()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        editor->GetTextView()->SetCCActive();
        CCTextChanged();
    }
}

void MainWindow::CCHide()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        editor->GetTextView()->ResetCCActive();
        editor->GetTextView()->ResetCCOpen();
    }
    codeCompletionListView->Hide();
}

void MainWindow::CCNext()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    codeCompletionListView->Next();
}

void MainWindow::CCPrev()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    codeCompletionListView->Prev();
}

void MainWindow::CCNextPage()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    codeCompletionListView->NextPage();
}

void MainWindow::CCPrevPage()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    codeCompletionListView->PrevPage();
}

void MainWindow::CCSelect(SelectedEventArgs& args)
{
    try
    {
        const Options& options = GetOptions();
        if (!options.codeCompletion) return;
        if (ccCanSelect)
        {
            currentCCEntry = codeCompletionListView->GetSelectedEntry();
            cmajor::view::Editor* editor = CurrentEditor();
            if (editor)
            {
                editor->GetTextView()->ReplaceCCText(ToUtf32(currentCCEntry.replacement));
                editor->GetTextView()->ResetCCActive();
                editor->GetTextView()->ResetCCOpen();
                editor->GetTextView()->SetCCCat(currentCCEntry.category);
            }
            codeCompletionListView->Hide();
            if (currentCCEntry.category == "FN" || currentCCEntry.category == "CL" || currentCCEntry.category == "CO")
            {
                GetParamHelpList(currentCCEntry.symbolIndex);
            }
            args.selected = true;
        }
        else
        {
            cmajor::view::Editor* editor = CurrentEditor();
            if (editor)
            {
                editor->GetTextView()->ResetCCOpen();
                editor->GetTextView()->ResetCCActive();
            }
            codeCompletionListView->Hide();
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void MainWindow::CCStart()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        editor->GetTextView()->SetCCActive();
    }
}

void MainWindow::CCStop()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        editor->GetTextView()->ResetCCOpen();
        editor->GetTextView()->ResetCCActive();
    }
    codeCompletionListView->Hide();
}

void MainWindow::ParamHelpNext()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    paramHelpView->Next();
}

void MainWindow::ParamHelpPrev()
{
    const Options& options = GetOptions();
    if (!options.codeCompletion) return;
    paramHelpView->Prev();
}

void MainWindow::ParamHelpClose()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        editor->GetTextView()->ResetParamHelpOpen();
    }
    paramHelpView->Hide();
}

void MainWindow::CCViewLocationChanged()
{
    cmajor::view::Editor* editor = CurrentEditor();
    if (editor)
    {
        editor->GetTextView()->ResetCCOpen();
        editor->GetTextView()->ResetCCActive();
    }
    codeCompletionListView->Hide();
}
*/

} // namespace cmcode
