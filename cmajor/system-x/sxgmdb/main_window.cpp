// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef CreateProcess

module sxgmdb.main.window;

import cmajor.systemx.kernel;
import sxgmdb.about.dialog;
import util;

namespace sxgmdb {

const char* sxgmdbVersion = "5.0.0";

std::string CmajorSystemXProjectsDir()
{
    std::string systemXProjectsDir = util::Path::Combine(util::Path::Combine(util::CmajorRoot(), "system-x"), "projects");
    std::filesystem::create_directories(systemXProjectsDir);
    return systemXProjectsDir;
}

MainDebuggerObserver::MainDebuggerObserver(MainWindow* mainWindow_) : mainWindow(mainWindow_)
{
}

void MainDebuggerObserver::DebuggerReady()
{
    SendMessage(mainWindow->Handle(), DEBUGGER_READY, 0, 0);
}

void MainDebuggerObserver::DebuggerError(const std::string& message)
{
    errorMessage = message;
    if (mainWindow->WaitingDebugger())
    {
        mainWindow->NotifyDebuggerError();
    }
    else
    {
        SendMessage(mainWindow->Handle(), DEBUGGER_ERROR, 0, 0);
    }
}

void MainDebuggerObserver::DebuggerProcessExit()
{
    if (mainWindow->WaitingDebugger())
    {
        mainWindow->NotifyDebuggingStopped();
    }
    else
    {
        SendMessage(mainWindow->Handle(), DEBUGGER_PROCESS_EXIT, 0, 0);
    }
}

MainWindow::MainWindow(const std::string& filePath_) :
    Window(wing::WindowCreateParams().Text("System X Debugger").BackgroundColor(wing::GetColor("window.background")).WindowClassName("system.x.gm.db.MainWindow")),
    observer(this), filePath(filePath_), args(), env(), machine(nullptr), process(nullptr), openFileMenuItem(nullptr), closeFileMenuItem(nullptr), exitMenuItem(nullptr),
    codeView(nullptr), registerView(nullptr), dataView(nullptr), argsView(nullptr), envView(nullptr), heapView(nullptr), stackView(nullptr), logView(nullptr),
    currentTopView(nullptr), currentBottomView(nullptr), waitingDebugger(false), startContinueMenuItem(nullptr), stopMenuItem(nullptr), resetMenuItem(nullptr),
    singleStepMenuItem(nullptr), stepOverMenuItem(nullptr), toggleBreakpointMenuItem(nullptr), fileOpen(false), terminalControl(nullptr),
    codeMenuItem(nullptr), dataMenuItem(nullptr), argsMenuItem(nullptr), envMenuItem(nullptr), heapMenuItem(nullptr), stackMenuItem(nullptr), 
    regsMenuItem(nullptr), startMenuItem(nullptr), endMenuItem(nullptr), nextLineMenuItem(nullptr), prevLineMenuItem(nullptr), nextQuarterMenuItem(nullptr),
    prevQuarterMenuItem(nullptr), nextPageMenuItem(nullptr), prevPageMenuItem(nullptr)
{
    std::unique_ptr<wing::MenuBar> menuBar(new wing::MenuBar());
    std::unique_ptr<wing::MenuItem> fileMenuItem(new wing::MenuItem("&File"));
    std::unique_ptr<wing::MenuItem> openFileMenuItemPtr(new wing::MenuItem("&Open..."));
    openFileMenuItem = openFileMenuItemPtr.get();
    openFileMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::o);
    openFileMenuItem->Click().AddHandler(this, &MainWindow::OpenFileClick);
    fileMenuItem->AddMenuItem(openFileMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> closeFileMenuItemPtr(new wing::MenuItem("&Close"));
    closeFileMenuItem = closeFileMenuItemPtr.get();
    closeFileMenuItem->SetShortcut(wing::Keys::controlModifier | wing::Keys::f4);
    closeFileMenuItem->Click().AddHandler(this, &MainWindow::CloseFileClick);
    fileMenuItem->AddMenuItem(closeFileMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> exitMenuItemPtr(new wing::MenuItem("E&xit"));
    exitMenuItem = exitMenuItemPtr.get();
    exitMenuItem->SetShortcut(wing::Keys::altModifier | wing::Keys::f4);
    exitMenuItem->Click().AddHandler(this, &MainWindow::ExitClick);
    fileMenuItem->AddMenuItem(exitMenuItemPtr.release());
    menuBar->AddMenuItem(fileMenuItem.release());
    std::unique_ptr<wing::MenuItem> viewMenuItem(new wing::MenuItem("&View"));
    std::unique_ptr<wing::MenuItem> codeMenuItemPtr(new wing::MenuItem("&Code"));
    codeMenuItemPtr->Click().AddHandler(this, &MainWindow::ViewCodeClick);
    codeMenuItem = codeMenuItemPtr.get();
    viewMenuItem->AddMenuItem(codeMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> dataMenuItemPtr(new wing::MenuItem("&Data"));
    dataMenuItem = dataMenuItemPtr.get();
    dataMenuItemPtr->Click().AddHandler(this, &MainWindow::ViewDataClick);
    viewMenuItem->AddMenuItem(dataMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> argsMenuItemPtr(new wing::MenuItem("&Arguments"));
    argsMenuItem = argsMenuItemPtr.get();
    argsMenuItemPtr->Click().AddHandler(this, &MainWindow::ViewArgsClick);
    viewMenuItem->AddMenuItem(argsMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> envMenuItemPtr(new wing::MenuItem("&Environment"));
    envMenuItem = envMenuItemPtr.get();
    envMenuItemPtr->Click().AddHandler(this, &MainWindow::ViewEnvClick);
    viewMenuItem->AddMenuItem(envMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> heapMenuItemPtr(new wing::MenuItem("&Heap"));
    heapMenuItem = heapMenuItemPtr.get();
    heapMenuItemPtr->Click().AddHandler(this, &MainWindow::ViewHeapClick);
    viewMenuItem->AddMenuItem(heapMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> stackMenuItemPtr(new wing::MenuItem("&Stack"));
    stackMenuItem = stackMenuItemPtr.get();
    stackMenuItemPtr->Click().AddHandler(this, &MainWindow::ViewStackClick);
    viewMenuItem->AddMenuItem(stackMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> regsMenuItemPtr(new wing::MenuItem("&Registers"));
    regsMenuItem = regsMenuItemPtr.get();
    regsMenuItemPtr->Click().AddHandler(this, &MainWindow::ViewRegsClick);
    viewMenuItem->AddMenuItem(regsMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> startMenuItemPtr(new wing::MenuItem("&Start"));
    startMenuItem = startMenuItemPtr.get();
    startMenuItemPtr->SetShortcut(wing::Keys::home);
    startMenuItemPtr->Click().AddHandler(this, &MainWindow::HomeClick);
    viewMenuItem->AddMenuItem(startMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> endMenuItemPtr(new wing::MenuItem("&End"));
    endMenuItem = endMenuItemPtr.get();
    endMenuItemPtr->SetShortcut(wing::Keys::end);
    endMenuItemPtr->Click().AddHandler(this, &MainWindow::EndClick);
    viewMenuItem->AddMenuItem(endMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> nextLineMenuItemPtr(new wing::MenuItem("&Next Line"));
    nextLineMenuItem = nextLineMenuItemPtr.get();
    nextLineMenuItemPtr->SetShortcut(wing::Keys::down);
    nextLineMenuItemPtr->Click().AddHandler(this, &MainWindow::NextLineClick);
    viewMenuItem->AddMenuItem(nextLineMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> prevLineMenuItemPtr(new wing::MenuItem("&Previous Line"));
    prevLineMenuItem = prevLineMenuItemPtr.get();
    prevLineMenuItemPtr->SetShortcut(wing::Keys::up);
    prevLineMenuItemPtr->Click().AddHandler(this, &MainWindow::PrevLineClick);
    viewMenuItem->AddMenuItem(prevLineMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> nextQuarterMenuItemPtr(new wing::MenuItem("Next &Quarter"));
    nextQuarterMenuItem = nextQuarterMenuItemPtr.get();
    nextQuarterMenuItemPtr->SetShortcut(wing::Keys::f8);
    nextQuarterMenuItemPtr->Click().AddHandler(this, &MainWindow::NextQuarterClick);
    viewMenuItem->AddMenuItem(nextQuarterMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> prevQuarterMenuItemPtr(new wing::MenuItem("&Previous Quarter"));
    prevQuarterMenuItem = prevQuarterMenuItemPtr.get();
    prevQuarterMenuItemPtr->SetShortcut(wing::Keys::f7);
    prevQuarterMenuItemPtr->Click().AddHandler(this, &MainWindow::PrevQuarterClick);
    viewMenuItem->AddMenuItem(prevQuarterMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> nextPageMenuItemPtr(new wing::MenuItem("Ne&xt Page"));
    nextPageMenuItem = nextPageMenuItemPtr.get();
    nextPageMenuItemPtr->SetShortcut(wing::Keys::pageDown);
    nextPageMenuItemPtr->Click().AddHandler(this, &MainWindow::NextPageClick);
    viewMenuItem->AddMenuItem(nextPageMenuItemPtr.release());
    std::unique_ptr<wing::MenuItem> prevPageMenuItemPtr(new wing::MenuItem("P&revious Page"));
    prevPageMenuItem = prevPageMenuItemPtr.get();
    prevPageMenuItemPtr->SetShortcut(wing::Keys::pageUp);
    prevPageMenuItemPtr->Click().AddHandler(this, &MainWindow::PrevPageClick);
    viewMenuItem->AddMenuItem(prevPageMenuItemPtr.release());
    menuBar->AddMenuItem(viewMenuItem.release());

    std::unique_ptr<wing::MenuItem> debugMenuItem(new wing::MenuItem("&Debug"));

    std::unique_ptr<wing::MenuItem> continueMenuItemPtr(new wing::MenuItem("&Start/Continue Debugging"));
    startContinueMenuItem = continueMenuItemPtr.get();
    startContinueMenuItem->SetShortcut(wing::Keys::f5);
    startContinueMenuItem->Click().AddHandler(this, &MainWindow::ContinueClick);
    debugMenuItem->AddMenuItem(continueMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> stopMenuItemPtr(new wing::MenuItem("S&top Debugging"));
    stopMenuItem = stopMenuItemPtr.get();
    stopMenuItem->SetShortcut(wing::Keys::shiftModifier | wing::Keys::f5);
    stopMenuItem->Click().AddHandler(this, &MainWindow::StopClick);
    debugMenuItem->AddMenuItem(stopMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> resetMenuItemPtr(new wing::MenuItem("&Reset Program To Start"));
    resetMenuItem = resetMenuItemPtr.get();
    resetMenuItem->SetShortcut(wing::Keys::f4);
    resetMenuItem->Click().AddHandler(this, &MainWindow::ResetClick);
    debugMenuItem->AddMenuItem(resetMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> singleStepMenuItemPtr(new wing::MenuItem("S&ingle Step"));;
    singleStepMenuItem = singleStepMenuItemPtr.get();
    singleStepMenuItem->SetShortcut(wing::Keys::f11);
    singleStepMenuItem->Click().AddHandler(this, &MainWindow::SingleStepClick);
    debugMenuItem->AddMenuItem(singleStepMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> stepOverMenuItemPtr(new wing::MenuItem("Step &Over"));;
    stepOverMenuItem = stepOverMenuItemPtr.get();
    stepOverMenuItem->SetShortcut(wing::Keys::f12);
    stepOverMenuItem->Click().AddHandler(this, &MainWindow::StepOverClick);
    debugMenuItem->AddMenuItem(stepOverMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> toggleBreakpointMenuItemPtr(new wing::MenuItem("&Toggle Breakpoint"));;
    toggleBreakpointMenuItem = toggleBreakpointMenuItemPtr.get();
    toggleBreakpointMenuItem->SetShortcut(wing::Keys::f9);
    toggleBreakpointMenuItem->Click().AddHandler(this, &MainWindow::ToggleBreakpointClick);
    debugMenuItem->AddMenuItem(toggleBreakpointMenuItemPtr.release());

    std::unique_ptr<wing::MenuItem> gotoPrevAddressMenuItemPtr(new wing::MenuItem("&Goto Previous Address"));
    gotoPrevAddressMenuItem = gotoPrevAddressMenuItemPtr.get();
    gotoPrevAddressMenuItem->SetShortcut(wing::Keys::left);
    gotoPrevAddressMenuItem->Click().AddHandler(this, &MainWindow::PrevAddressClick);
    debugMenuItem->AddMenuItem(gotoPrevAddressMenuItemPtr.release());

    menuBar->AddMenuItem(debugMenuItem.release());

    std::unique_ptr<wing::MenuItem> helpMenuItem(new wing::MenuItem("&Help"));
    std::unique_ptr<wing::MenuItem> aboutMenuItemPtr(new wing::MenuItem("&About..."));
    aboutMenuItemPtr->Click().AddHandler(this, &MainWindow::AboutClick);
    helpMenuItem->AddMenuItem(aboutMenuItemPtr.release());
    menuBar->AddMenuItem(helpMenuItem.release());
    AddChild(menuBar.release());

    wing::SplitContainerCreateParams verticalSplitContainerCreateParams(wing::SplitterOrientation::vertical);
    verticalSplitContainerCreateParams.BackgroundColor(wing::GetColor("splitContainer.background"));
    verticalSplitContainerCreateParams.Pane1BackgroundColor(wing::GetColor("splitContainer.background")).Pane2BackgroundColor(wing::GetColor("splitContainer.background")).
        BackgroundColor(wing::GetColor("splitContainer.background")).SplitterBackgroundColor(wing::GetColor("splitter.background")).SplitterEdgeColor(wing::GetColor("splitter.edge")).SplitterDistance(0).SetDock(wing::Dock::fill);
    std::unique_ptr<wing::SplitContainer> verticalSplitContainerPtr(new wing::SplitContainer(verticalSplitContainerCreateParams));
    verticalSplitContainer = verticalSplitContainerPtr.get();

    std::unique_ptr<wing::TabControl> topTabControlPtr(
        new wing::TabControl(wing::TabControlCreateParams().
            BackgroundColor(wing::GetColor("code.tab.control.background")).
            FrameColor(wing::GetColor("code.tab.control.frame")).
            TextColor(wing::GetColor("code.tab.control.text")).
            NormalBackgroundColor(wing::GetColor("code.tab.control.tab.normal.background")).
            SelectedBackgroundColor(wing::GetColor("code.tab.control.tab.selected.background")).
            CloseBoxSelectedColor(wing::GetColor("code.tab.control.close.box.selected.background")).
            SetDock(wing::Dock::fill)));
    topTabControl = topTabControlPtr.get();
    topTabControl->SetBackgroundItemName("code.tab.control.background");
    topTabControl->SetFrameItemName("code.tab.control.frame");
    topTabControl->SetTextItemName("code.tab.control.text");
    topTabControl->SetTabNormalBackgroundItemName("code.tab.control.tab.normal.background");
    topTabControl->SetTabSelectedBackgroundItemName("code.tab.control.tab.selected.background");
    topTabControl->SetCloseBoxSelectedBackgroundItemName("code.tab.control.close.box.selected.background");
    topTabControl->TabPageSelected().AddHandler(this, &MainWindow::TopTabPageSelected);
    topTabControl->ControlRemoved().AddHandler(this, &MainWindow::TopTabPageRemoved);
    std::unique_ptr<Control> paddedTopTabControl(new wing::PaddedControl(wing::PaddedControlCreateParams(topTabControlPtr.release()).Defaults()));
    std::unique_ptr<Control> borderedTopTabControl(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTopTabControl.release()).SetBorderStyle(wing::BorderStyle::single).
        NormalSingleBorderColor(wing::GetColor("code.tab.control.frame")).FocusedSingleBorderColor(wing::GetColor("code.tab.control.frame")).SetDock(wing::Dock::fill)));
    borderedTopTabControl->SetFrameItemName("code.tab.control.frame");
    verticalSplitContainer->Pane1Container()->AddChild(borderedTopTabControl.release());

    std::unique_ptr<wing::TabControl> bottomTabControlPtr(
        new wing::TabControl(wing::TabControlCreateParams().
            BackgroundColor(wing::GetColor("code.tab.control.background")).
            FrameColor(wing::GetColor("code.tab.control.frame")).
            TextColor(wing::GetColor("code.tab.control.text")).
            NormalBackgroundColor(wing::GetColor("code.tab.control.tab.normal.background")).
            SelectedBackgroundColor(wing::GetColor("code.tab.control.tab.selected.background")).
            CloseBoxSelectedColor(wing::GetColor("code.tab.control.close.box.selected.background")).
            SetDock(wing::Dock::fill)));
    bottomTabControl = bottomTabControlPtr.get();
    bottomTabControl->SetBackgroundItemName("code.tab.control.background");
    bottomTabControl->SetFrameItemName("code.tab.control.frame");
    bottomTabControl->SetTextItemName("code.tab.control.text");
    bottomTabControl->SetTabNormalBackgroundItemName("code.tab.control.tab.normal.background");
    bottomTabControl->SetTabSelectedBackgroundItemName("code.tab.control.tab.selected.background");
    bottomTabControl->SetCloseBoxSelectedBackgroundItemName("code.tab.control.close.box.selected.background");
    bottomTabControl->TabPageSelected().AddHandler(this, &MainWindow::BottomTabPageSelected);
    bottomTabControl->ControlRemoved().AddHandler(this, &MainWindow::BottomTabPageRemoved);
    std::unique_ptr<Control> paddedBottomTabControl(new wing::PaddedControl(wing::PaddedControlCreateParams(bottomTabControlPtr.release()).Defaults()));
    std::unique_ptr<Control> borderedBottomTabControl(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedBottomTabControl.release()).SetBorderStyle(wing::BorderStyle::single).
        NormalSingleBorderColor(wing::GetColor("code.tab.control.frame")).FocusedSingleBorderColor(wing::GetColor("code.tab.control.frame")).SetDock(wing::Dock::fill)));
    borderedBottomTabControl->SetFrameItemName("code.tab.control.frame");
    verticalSplitContainer->Pane2Container()->AddChild(borderedBottomTabControl.release());

    AddChild(verticalSplitContainerPtr.release());

    SetState(DebuggingState::debuggerIdle);

    terminalFile.reset(new cmajor::systemx::guiterm::TerminalFile());
    cmajor::systemx::kernel::SetTerminalFile(terminalFile.get());
    DisableViewMenuItems();
    if (!filePath.empty())
    {
        LoadProcess();
    }
}

MainWindow::~MainWindow()
{
    StopDebugging(true);
}

void MainWindow::NotifyDebuggingStopped()
{
    std::unique_lock<std::mutex> lock(mtx);
    SetState(DebuggingState::debuggerExit);
    debuggingStoppedOrErrorVar.notify_one();
}

void MainWindow::NotifyDebuggerError()
{
    std::unique_lock<std::mutex> lock(mtx);
    SetState(DebuggingState::debuggerError);
    debuggingStoppedOrErrorVar.notify_one();
}

void MainWindow::OnKeyDown(wing::KeyEventArgs& args)
{
    Window::OnKeyDown(args);
    if (!args.handled)
    {
        if (terminalControl && terminalControl->IsActive())
        {
            terminalControl->HandleKeyDown(args);
        }
    }
}

void MainWindow::OnKeyPress(wing::KeyPressEventArgs& args)
{
    Window::OnKeyPress(args);
    if (!args.handled)
    {
        if (terminalControl && terminalControl->IsActive())
        {
            terminalControl->HandleKeyPress(args);
        }
    }
}

bool MainWindow::ProcessMessage(wing::Message& msg)
{
    switch (msg.message)
    {
    case DEBUGGER_READY:
    {
        SetState(DebuggingState::debuggerWaitingForCommand);
        UpdateViews();
        msg.result = 0;
        return true;
    }
    case DEBUGGER_ERROR:
    {
        SetState(DebuggingState::debuggerError);
        UpdateViews();
        PrintError(observer.ErrorMessage());
        msg.result = 0;
        return true;
    }
    case DEBUGGER_PROCESS_EXIT:
    {
        SetState(DebuggingState::debuggerExit);
        UpdateViews();
        PrintExit();
        msg.result = 0;
        return true;
    }
    default:
    {
        return Window::ProcessMessage(msg);
    }
    }
}

void MainWindow::OnMouseWheel(wing::MouseWheelEventArgs& args)
{
    if (args.value > 0)
    {
        PrevQuarterClick();
    }
    else
    {
        NextQuarterClick();
    }
    args.handled = true;
}

void MainWindow::ToggleBreakpointClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::f9);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        try
        {
            debugger->ToggleBreakpoint(codeView->CurrentAddress());
            codeView->Reset();
            codeView->UpdateView(false);
        }
        catch (const std::exception& ex)
        {
            PrintError(ex.what());
        }
    }
}

void MainWindow::SingleStepClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::f11);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        try
        {
            if (state == DebuggingState::debuggerWaitingForCommand)
            {
                SetState(DebuggingState::debuggerBusy);
                debugger->SingleStep();
            }
            else
            {
                throw std::runtime_error("debugger not waiting for command");
            }
        }
        catch (const std::exception& ex)
        {
            PrintError(ex.what());
        }
    }
}

void MainWindow::StepOverClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::f12);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        try
        {
            if (state == DebuggingState::debuggerWaitingForCommand)
            {
                SetState(DebuggingState::debuggerBusy);
                debugger->StepOver();
            }
            else
            {
                throw std::runtime_error("debugger not waiting for command");
            }
        }
        catch (const std::exception& ex)
        {
            PrintError(ex.what());
        }
    }
}

void MainWindow::ContinueClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::f5);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        try
        {
            if (state == DebuggingState::debuggerWaitingForCommand)
            {
                SetState(DebuggingState::debuggerBusy);
                debugger->Continue();
            }
            else
            {
                throw std::runtime_error("debugger not waiting for command");
            }
        }
        catch (const std::exception& ex)
        {
            PrintError(ex.what());
        }
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

void MainWindow::OpenFileClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::controlModifier | wing::Keys::o);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        try
        {
            std::vector<std::pair<std::string, std::string>> descriptionFilterPairs;
            descriptionFilterPairs.push_back(std::make_pair("System X executable files (*.x)", "*.x"));
            std::string initialDirectory = util::GetFullPath(CmajorSystemXProjectsDir());
            std::string filePath;
            std::string currentDirectory;
            std::vector<std::string> fileNames;
            bool selected = wing::OpenFileName(Handle(), descriptionFilterPairs, initialDirectory, std::string(), "x",
                OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filePath, currentDirectory, fileNames);
            if (selected)
            {
                waitingDebugger = true;
                if (state == DebuggingState::debuggerWaitingForCommand)
                {
                    StopDebugging(true);
                }
                this->filePath = util::GetFullPath(filePath);
                waitingDebugger = false;
                LoadProcess();
                fileOpen = true;
            }
        }
        catch (const std::exception& ex)
        {
            waitingDebugger = false;
            wing::ShowErrorMessageBox(Handle(), ex.what());
        }
    }
}

void MainWindow::CloseFileClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::controlModifier | wing::Keys::f4);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        try
        {
            waitingDebugger = true;
            StopDebugging(true);
            waitingDebugger = false;
            fileOpen = false;
            DisableViewMenuItems();
            SetState(DebuggingState::debuggerIdle);
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(Handle(), ex.what());
        }
    }
}

void MainWindow::ResetClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::f4);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        try
        {
            if (filePath.empty())
            {
                throw std::runtime_error("no executable file open");
            }
            waitingDebugger = true;
            if (state == DebuggingState::debuggerWaitingForCommand || state == DebuggingState::debuggerExit)
            {
                StopDebugging(true);
            }
            waitingDebugger = false;
            LoadProcess();
        }
        catch (const std::exception& ex)
        {
            waitingDebugger = false;
            wing::ShowErrorMessageBox(Handle(), ex.what());
        }
    }
}

void MainWindow::StopClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::shiftModifier | wing::Keys::f5);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        try
        {
            waitingDebugger = true;
            if (state == DebuggingState::debuggerWaitingForCommand)
            {
                StopDebugging(true);
            }
            waitingDebugger = false;
        }
        catch (const std::exception& ex)
        {
            waitingDebugger = false;
            wing::ShowErrorMessageBox(Handle(), ex.what());
        }
    }
}

void MainWindow::ExitClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::altModifier | wing::Keys::f4);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        Close();
    }
}

void MainWindow::ViewCodeClick()
{
    if (codeView)
    {
        codeView->SetVisible(true);
        wing::TabPage* tabPage = topTabControl->GetTabPageByKey("code");
        if (tabPage)
        {
            tabPage->Select();
        }
        codeView->UpdateView(true);
    }
    else
    {
        CreateCodeView();
    }
}

void MainWindow::ViewRegsClick()
{
    if (registerView)
    {
        registerView->SetVisible(true);
        wing::TabPage* tabPage = bottomTabControl->GetTabPageByKey("regs");
        if (tabPage)
        {
            tabPage->Select();
        }
        registerView->UpdateView(true);
    }
    else
    {
        CreateRegisterView();
    }
}

void MainWindow::ViewDataClick()
{
    if (dataView)
    {
        dataView->SetVisible(true);
        wing::TabPage* tabPage = topTabControl->GetTabPageByKey("data");
        if (tabPage)
        {
            tabPage->Select();
        }
        dataView->UpdateView(true);
    }
    else
    {
        CreateDataView();
    }
}

void MainWindow::ViewArgsClick()
{
    if (argsView)
    {
        argsView->SetVisible(true);
        wing::TabPage* tabPage = topTabControl->GetTabPageByKey("args");
        if (tabPage)
        {
            tabPage->Select();
        }
        argsView->UpdateView(true);
    }
    else
    {
        CreateArgsView();
    }
}

void MainWindow::ViewEnvClick()
{
    if (envView)
    {
        envView->SetVisible(true);
        wing::TabPage* tabPage = topTabControl->GetTabPageByKey("env");
        if (tabPage)
        {
            tabPage->Select();
        }
        envView->UpdateView(true);
    }
    else
    {
        CreateEnvView();
    }
}

void MainWindow::ViewHeapClick()
{
    if (heapView)
    {
        heapView->SetVisible(true);
        wing::TabPage* tabPage = topTabControl->GetTabPageByKey("heap");
        if (tabPage)
        {
            tabPage->Select();
        }
        heapView->UpdateView(true);
    }
    else
    {
        CreateHeapView();
    }
}

void MainWindow::ViewStackClick()
{
    if (stackView)
    {
        stackView->SetVisible(true);
        wing::TabPage* tabPage = topTabControl->GetTabPageByKey("stack");
        if (tabPage)
        {
            tabPage->Select();
        }
        stackView->UpdateView(true);
    }
    else
    {
        CreateStackView();
    }
}

void MainWindow::ViewLogClick()
{
    if (logView)
    {
        wing::TabPage* tabPage = bottomTabControl->GetTabPageByKey("log");
        if (tabPage)
        {
            tabPage->Select();
        }
    }
    else
    {
        CreateLogView();
    }
}

void MainWindow::NextLineClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::down);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView)
        {
            currentTopView->NextLine();
        }
    }
}

void MainWindow::PrevLineClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::up);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView)
        {
            currentTopView->PrevLine();
        }
    }
}

void MainWindow::NextQuarterClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::f8);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView)
        {
            currentTopView->NextQuarter();
        }
    }
}

void MainWindow::PrevQuarterClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::f7);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView)
        {
            currentTopView->PrevQuarter();
        }
    }
}

void MainWindow::NextPageClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::pageDown);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView)
        {
            currentTopView->NextPage();
        }
    }
}

void MainWindow::PrevPageClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::pageUp);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView)
        {
            currentTopView->PrevPage();
        }
    }
}

void MainWindow::HomeClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::home);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView)
        {
            currentTopView->ToStart();
        }
    }
}

void MainWindow::EndClick()
{
    if (terminalControl && terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::end);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView)
        {
            currentTopView->ToEnd();
        }
    }
}

void MainWindow::PrevAddressClick()
{
    if (terminalControl&& terminalControl->IsActive())
    {
        wing::KeyEventArgs args(wing::Keys::left);
        terminalControl->HandleKeyDown(args);
    }
    else
    {
        if (currentTopView == codeView)
        {
            codeView->GotoPrevAddress();
        }
    }
}

void MainWindow::TopTabPageSelected()
{
    currentTopView->SetVisible(false);
    wing::TabPage* selectedTabPage = topTabControl->SelectedTabPage();
    if (selectedTabPage->Key() == "code")
    {
        currentTopView = codeView;
    }
    else if (selectedTabPage->Key() == "data")
    {
        currentTopView = dataView;
    }
    else if (selectedTabPage->Key() == "args")
    {
        currentTopView = argsView;
    }
    else if (selectedTabPage->Key() == "env")
    {
        currentTopView = envView;
    }
    else if (selectedTabPage->Key() == "heap")
    {
        currentTopView = heapView;
    }
    else if (selectedTabPage->Key() == "stack")
    {
        currentTopView = stackView;
    }
    currentTopView->SetVisible(true);
    currentTopView->UpdateView(true);
}

void MainWindow::TopTabPageRemoved(wing::ControlEventArgs& controlEventArgs)
{
    wing::TabPage* tabPage = static_cast<wing::TabPage*>(controlEventArgs.control);
    if (tabPage->Key() == "code")
    {
        RemoveView(codeView);
        codeView = nullptr;
    }
    else if (tabPage->Key() == "data")
    {
        RemoveView(dataView);
        dataView = nullptr;
    }
    else if (tabPage->Key() == "args")
    {
        RemoveView(argsView);
        argsView = nullptr;
    }
    else if (tabPage->Key() == "env")
    {
        RemoveView(envView);
        envView = nullptr;
    }
    else if (tabPage->Key() == "heap")
    {
        RemoveView(heapView);
        heapView = nullptr;
    }
    else if (tabPage->Key() == "stack")
    {
        RemoveView(stackView);
        stackView = nullptr;
    }
}

void MainWindow::BottomTabPageSelected()
{
    wing::TabPage* selectedTabPage = bottomTabControl->SelectedTabPage();
    if (selectedTabPage->Key() == "regs")
    {
        currentBottomView->SetVisible(false);
        currentBottomView = registerView;
        currentBottomView->SetVisible(true);
        currentBottomView->UpdateView(true);
    }
}

void MainWindow::BottomTabPageRemoved(wing::ControlEventArgs& controlEventArgs)
{
    wing::TabPage* tabPage = static_cast<wing::TabPage*>(controlEventArgs.control);
    if (tabPage->Key() == "regs")
    {
        RemoveView(registerView);
        registerView = nullptr;
    }
    else if (tabPage->Key() == "log")
    {
        logView = nullptr;
    }
}

void MainWindow::EnableViewMenuItems()
{
    codeMenuItem->Enable();
    dataMenuItem->Enable();
    argsMenuItem->Enable();
    envMenuItem->Enable();
    heapMenuItem->Enable();
    stackMenuItem->Enable();
    regsMenuItem->Enable();
    startMenuItem->Enable();
    endMenuItem->Enable();
    nextLineMenuItem->Enable();
    prevLineMenuItem->Enable();
    nextQuarterMenuItem->Enable();
    prevQuarterMenuItem->Enable();
    nextPageMenuItem->Enable();
    prevPageMenuItem->Enable();
}

void MainWindow::DisableViewMenuItems()
{
    codeMenuItem->Disable();
    dataMenuItem->Disable();
    argsMenuItem->Disable();
    envMenuItem->Disable();
    heapMenuItem->Disable();
    stackMenuItem->Disable();
    regsMenuItem->Disable();
    startMenuItem->Disable();
    endMenuItem->Disable();
    nextLineMenuItem->Disable();
    prevLineMenuItem->Disable();
    nextQuarterMenuItem->Disable();
    prevQuarterMenuItem->Disable();
    nextPageMenuItem->Disable();
    prevPageMenuItem->Disable();
}

void MainWindow::LoadProcess()
{
    SetState(DebuggingState::debuggerBusy);
    machine.reset(new cmajor::systemx::machine::Machine());
    cmajor::systemx::kernel::Kernel::Instance().SetMachine(machine.get());
    cmajor::systemx::kernel::Kernel::Instance().Start();
    EnableViewMenuItems();
    process = cmajor::systemx::kernel::ProcessManager::Instance().CreateProcess();
    process->SetFilePath(filePath);
    args.clear();
    args.push_back(filePath);
    cmajor::systemx::kernel::Load(process, args, env, *machine);
    dataRanges.SetMachine(machine.get());
    dataRanges.SetProcess(process);
    if (codeView)
    {
        codeView->Reset();
    }
    else
    {
        CreateCodeView();
    }
    if (!registerView)
    {
        CreateRegisterView();
    }
    if (!logView)
    {
        CreateLogView();
    }
    logView->Clear();
    if (!dataView)
    {
        CreateDataView();
    }
    if (!argsView)
    {
        CreateArgsView();
    }
    if (!envView)
    {
        CreateEnvView();
    }
    if (!heapView)
    {
        CreateHeapView();
    }
    if (!stackView)
    {
        CreateStackView();
    }
    for (DebugView* view : views)
    {
        view->SetMachine(machine.get());
        view->SetProcess(process);
        view->SetVisible(false);
    }
    ViewCodeClick();
    ViewRegsClick();
    if (!terminalControl)
    {
        cmajor::systemx::guiterm::TerminalControlCreateParams createParams;
        terminalControl = new cmajor::systemx::guiterm::TerminalControl(createParams);
        terminalControl->SetFlag(wing::ControlFlags::scrollSubject);
        terminalControl->SetDoubleBuffered();
        terminalFile->SetTerminalControl(terminalControl);
        wing::ScrollableControl* scrollableTerminal = new wing::ScrollableControl(wing::ScrollableControlCreateParams(terminalControl).SetDock(wing::Dock::fill));
        wing::TabPage* terminalTabPage = new wing::TabPage("Terminal", "terminal");
        terminalTabPage->AddChild(scrollableTerminal);
        bottomTabControl->AddTabPage(terminalTabPage);
    }
    ViewRegsClick();
    StartDebugging();
}

void RunDebugger(Debugger* debugger, cmajor::systemx::machine::Machine* machine)
{
    debugger->Run();
}

void MainWindow::StartDebugging()
{
    if (!machine)
    {
        return;
    }
    if (debuggerThread.joinable())
    {
        debuggerThread.join();
    }
    debugger.reset(new Debugger(machine.get(), process));
    debugger->SetObserver(&observer);
    for (auto view : views)
    {
        view->SetDebugger(debugger.get());
    }
    debuggerThread = std::thread(RunDebugger, debugger.get(), machine.get());
}

void MainWindow::StopDebugging(bool unloadProcess)
{
    if (debugger)
    {
        debugger->Stop();
    }
    if (debuggerThread.joinable())
    {
        debuggerThread.join();
    }
    debugger.reset();
    if (unloadProcess)
    {
        if (process)
        {
            cmajor::systemx::kernel::ProcessManager::Instance().DeleteProcess(process->Id());
            process = nullptr;
        }
        if (machine)
        {
            machine->Exit();
        }
        cmajor::systemx::kernel::Kernel::Instance().Stop();
        if (machine)
        {
            machine.reset();
        }
        cmajor::systemx::kernel::Kernel::Instance().SetMachine(nullptr);
        dataRanges.SetMachine(nullptr);
        dataRanges.SetProcess(nullptr);
        for (auto& view : views)
        {
            view->SetMachine(nullptr);
            view->SetProcess(nullptr);
        }
        UpdateViews();
    }
    SetState(DebuggingState::debuggerExit);
}

void MainWindow::UpdateViews()
{
    for (DebugView* view : views)
    {
        if (view->IsVisible())
        {
            view->UpdateView(true);
        }
    }
}

void MainWindow::RemoveView(DebugView* debugView)
{
    auto it = std::find(views.begin(), views.end(), debugView);
    if (it != views.end())
    {
        views.erase(it);
    }
}

void MainWindow::CreateCodeView()
{
    if (!machine) return;
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage("Code", "code"));
    codeView = new CodeView(CodeViewCreateParams().Defaults());
    codeView->SetMachine(machine.get());
    codeView->SetProcess(process);
    codeView->SetVisible(true);
    views.push_back(codeView);
    currentTopView = codeView;
    tabPage->AddChild(codeView);
    if (topTabControl->TabPages().IsEmpty())
    {
        topTabControl->AddTabPage(tabPage.release());
    }
    else
    {
        topTabControl->InsertTabPageBefore(tabPage.release(), static_cast<wing::TabPage*>(topTabControl->TabPages().FirstChild()));
    }
    codeView->UpdateView(true);
}

void MainWindow::CreateRegisterView()
{
    if (!machine) return;
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage("Registers", "regs"));
    registerView = new RegisterView(RegisterViewCreateParams().Defaults());
    registerView->SetMachine(machine.get());
    registerView->SetProcess(process);
    registerView->SetVisible(true);
    views.push_back(registerView);
    currentBottomView = registerView;
    tabPage->AddChild(registerView);
    if (bottomTabControl->TabPages().IsEmpty())
    {
        bottomTabControl->AddTabPage(tabPage.release());
    }
    else
    {
        bottomTabControl->InsertTabPageBefore(tabPage.release(), static_cast<wing::TabPage*>(bottomTabControl->TabPages().FirstChild()));
    }
    registerView->UpdateView(true);
}

void MainWindow::CreateDataView()
{
    if (!machine) return;
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage("Data", "data"));
    dataView = new DataView(DataViewCreateParams().Defaults(), dataRanges.GetDataRange("data"));
    dataView->SetMachine(machine.get());
    dataView->SetProcess(process);
    dataView->SetVisible(true);
    views.push_back(dataView);
    currentTopView = dataView;
    tabPage->AddChild(dataView);
    if (topTabControl->TabPages().IsEmpty())
    {
        topTabControl->AddTabPage(tabPage.release());
    }
    else
    {
        topTabControl->InsertTabPageAfter(tabPage.release(), GetTabPageByNameOrFirstTabPage(topTabControl, "code"));
    }
    dataView->UpdateView(true);
}

void MainWindow::CreateArgsView()
{
    if (!machine) return;
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage("Arguments", "args"));
    argsView = new DataView(DataViewCreateParams().Defaults(), dataRanges.GetDataRange("args"));
    argsView->SetMachine(machine.get());
    argsView->SetProcess(process);
    argsView->SetVisible(true);
    views.push_back(argsView);
    currentTopView = argsView;
    tabPage->AddChild(argsView);
    if (topTabControl->TabPages().IsEmpty())
    {
        topTabControl->AddTabPage(tabPage.release());
    }
    else
    {
        topTabControl->InsertTabPageAfter(tabPage.release(), GetTabPageByNameOrFirstTabPage(topTabControl, "data"));
    }
    argsView->UpdateView(true);
}

void MainWindow::CreateEnvView()
{
    if (!machine) return;
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage("Environment", "env"));
    envView = new DataView(DataViewCreateParams().Defaults(), dataRanges.GetDataRange("env"));
    envView->SetMachine(machine.get());
    envView->SetProcess(process);
    envView->SetVisible(true);
    views.push_back(envView);
    currentTopView = envView;
    tabPage->AddChild(envView);
    if (topTabControl->TabPages().IsEmpty())
    {
        topTabControl->AddTabPage(tabPage.release());
    }
    else
    {
        topTabControl->InsertTabPageAfter(tabPage.release(), GetTabPageByNameOrFirstTabPage(topTabControl, "args"));
    }
    envView->UpdateView(true);
}

void MainWindow::CreateHeapView()
{
    if (!machine) return;
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage("Heap", "heap"));
    heapView = new DataView(DataViewCreateParams().Defaults(), dataRanges.GetDataRange("heap"));
    heapView->SetMachine(machine.get());
    heapView->SetProcess(process);
    heapView->SetVisible(true);
    views.push_back(heapView);
    currentTopView = heapView;
    tabPage->AddChild(heapView);
    if (topTabControl->TabPages().IsEmpty())
    {
        topTabControl->AddTabPage(tabPage.release());
    }
    else
    {
        topTabControl->InsertTabPageAfter(tabPage.release(), GetTabPageByNameOrFirstTabPage(topTabControl, "env"));
    }
    heapView->UpdateView(true);
}

void MainWindow::CreateStackView()
{
    if (!machine) return;
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage("Stack", "stack"));
    stackView = new DataView(DataViewCreateParams().Defaults(), dataRanges.GetDataRange("stack"));
    stackView->SetMachine(machine.get());
    stackView->SetProcess(process);
    stackView->SetVisible(true);
    views.push_back(stackView);
    currentTopView = stackView;
    tabPage->AddChild(stackView);
    if (topTabControl->TabPages().IsEmpty())
    {
        topTabControl->AddTabPage(tabPage.release());
    }
    else
    {
        topTabControl->InsertTabPageAfter(tabPage.release(), GetTabPageByNameOrFirstTabPage(topTabControl, "heap"));
    }
    stackView->UpdateView(true);
}

void MainWindow::CreateLogView()
{
    if (!machine) return;
    std::unique_ptr<wing::TabPage> tabPage(new wing::TabPage("Log", "log"));
    logView = new wing::LogView(wing::TextViewCreateParams().SetDock(wing::Dock::fill));
    tabPage->AddChild(logView);
    if (bottomTabControl->TabPages().IsEmpty())
    {
        bottomTabControl->AddTabPage(tabPage.release());
    }
    else
    {
        bottomTabControl->InsertTabPageAfter(tabPage.release(), GetTabPageByNameOrFirstTabPage(bottomTabControl, "regs"));
    }
}

wing::TabPage* MainWindow::GetTabPageByNameOrFirstTabPage(wing::TabControl* tabControl, const std::string& tabName) const
{
    wing::TabPage* tabPage = tabControl->GetTabPageByKey(tabName);
    if (tabPage)
    {
        return tabPage;
    }
    else if (!tabControl->TabPages().IsEmpty())
    {
        return static_cast<wing::TabPage*>(tabControl->TabPages().FirstChild());
    }
    else
    {
        return nullptr;
    }
}

void MainWindow::WaitUntilDebuggingStoppedOrError()
{
    std::unique_lock<std::mutex> lock(mtx);
    debuggingStoppedOrErrorVar.wait(lock, [this] { return state == DebuggingState::debuggerExit || state == DebuggingState::debuggerError; });
    waitingDebugger = false;
    switch (state)
    {
    case DebuggingState::debuggerError:
    {
        std::string errorMessage = observer.ErrorMessage();
        PrintError(errorMessage);
        break;
    }
    case DebuggingState::debuggerExit:
    {
        PrintExit();
        break;
    }
    }
}

void MainWindow::PrintError(const std::string& errorMessage)
{
    ViewLogClick();
    logView->WriteLine(errorMessage);
}

void MainWindow::PrintExit()
{
    uint8_t exitCode = 255;
    if (process)
    {
        exitCode = process->ExitCode();
    }
    ViewLogClick();
    logView->WriteLine("process exited with code " + std::to_string(static_cast<int>(exitCode)));
}

void MainWindow::SetState(DebuggingState state_)
{
    state = state_;
    startContinueMenuItem->Disable();
    stopMenuItem->Disable();
    resetMenuItem->Disable();
    singleStepMenuItem->Disable();
    stepOverMenuItem->Disable();
    toggleBreakpointMenuItem->Disable();
    gotoPrevAddressMenuItem->Disable();
    closeFileMenuItem->Disable();
    switch (state)
    {
    case DebuggingState::debuggerBusy:
    {
        stopMenuItem->Enable();
        break;
    }
    case DebuggingState::debuggerWaitingForCommand:
    {
        startContinueMenuItem->Enable();
        stopMenuItem->Enable();
        resetMenuItem->Enable();
        singleStepMenuItem->Enable();
        stepOverMenuItem->Enable();
        toggleBreakpointMenuItem->Enable();
        gotoPrevAddressMenuItem->Enable();
        if (fileOpen)
        {
            closeFileMenuItem->Enable();
        }
        break;
    }
    case DebuggingState::debuggerError:
    {
        resetMenuItem->Enable();
        if (fileOpen)
        {
            closeFileMenuItem->Enable();
        }
        break;
    }
    case DebuggingState::debuggerExit:
    {
        resetMenuItem->Enable();
        if (fileOpen)
        {
            closeFileMenuItem->Enable();
        }
        break;
    }
    }
}

} // namespace cmajor::systemx::db
