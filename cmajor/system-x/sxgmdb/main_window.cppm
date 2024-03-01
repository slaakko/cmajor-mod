// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module sxgmdb.main.window;

import cmajor.systemx.guiterm;
import cmajor.systemx.machine;
import cmajor.systemx.kernel;
import sxgmdb.code.view;
import sxgmdb.debug.view;
import sxgmdb.reg.view;
import sxgmdb.data.view;
import sxgmdb.debugger;
import wing;
import std.core;

export namespace sxgmdb {

extern const char* sxgmdbVersion;

const int DEBUGGER_READY = WM_USER + 1;
const int DEBUGGER_ERROR = WM_USER + 2;
const int DEBUGGER_PROCESS_EXIT = WM_USER + 3;

class MainWindow;

class MainDebuggerObserver : public DebuggerObserver
{
public:
    MainDebuggerObserver(MainWindow* mainWindow_);
    void DebuggerReady() override;
    void DebuggerError(const std::string& message) override;
    void DebuggerProcessExit() override;
    const std::string& ErrorMessage() const { return errorMessage; }
private:
    MainWindow* mainWindow;
    std::string errorMessage;
};

enum class DebuggingState
{
    debuggerIdle, debuggerWaitingForCommand, debuggerBusy, debuggerExit, debuggerError
};

class MainWindow : public wing::Window
{
public:
    MainWindow(const std::string& filePath_);
    ~MainWindow();
    bool WaitingDebugger() const { return waitingDebugger; }
    void NotifyDebuggingStopped();
    void NotifyDebuggerError();
protected:
    void OnKeyDown(wing::KeyEventArgs& args);
    void OnKeyPress(wing::KeyPressEventArgs& args);
    bool ProcessMessage(wing::Message& msg) override;
    void OnMouseWheel(wing::MouseWheelEventArgs& args) override;
private:
    void ToggleBreakpointClick();
    void SingleStepClick();
    void StepOverClick();
    void ContinueClick();
    void AboutClick();
    void OpenFileClick();
    void CloseFileClick();
    void ResetClick();
    void StopClick();
    void ExitClick();
    void ViewCodeClick();
    void ViewRegsClick();
    void ViewDataClick();
    void ViewArgsClick();
    void ViewEnvClick();
    void ViewHeapClick();
    void ViewStackClick();
    void ViewLogClick();
    void NextLineClick();
    void PrevLineClick();
    void NextPageClick();
    void PrevPageClick();
    void NextQuarterClick();
    void PrevQuarterClick();
    void HomeClick();
    void EndClick();
    void PrevAddressClick();
    void TopTabPageSelected();
    void TopTabPageRemoved(wing::ControlEventArgs& controlEventArgs);
    void BottomTabPageSelected();
    void BottomTabPageRemoved(wing::ControlEventArgs& controlEventArgs);
    void EnableViewMenuItems();
    void DisableViewMenuItems();
    void LoadProcess();
    void StartDebugging();
    void StopDebugging(bool unloadProcess);
    void UpdateViews();
    void RemoveView(DebugView* debugView);
    void CreateCodeView();
    void CreateRegisterView();
    void CreateDataView();
    void CreateArgsView();
    void CreateEnvView();
    void CreateHeapView();
    void CreateStackView();
    void CreateLogView();
    wing::TabPage* GetTabPageByNameOrFirstTabPage(wing::TabControl* tabControl, const std::string& tabName) const;
    void WaitUntilDebuggingStoppedOrError();
    void PrintError(const std::string& errorMessage);
    void PrintExit();
    void SetState(DebuggingState state_);
    MainDebuggerObserver observer;
    std::unique_ptr<cmajor::systemx::machine::Machine> machine;
    std::unique_ptr<Debugger> debugger;
    CodeView* codeView;
    RegisterView* registerView;
    DataView* dataView;
    DataView* argsView;
    DataView* envView;
    DataView* heapView;
    DataView* stackView;
    wing::LogView* logView;
    std::string filePath;
    std::vector<std::string> args;
    std::vector<std::string> env;
    std::thread debuggerThread;
    cmajor::systemx::kernel::Process* process;
    wing::MenuItem* openFileMenuItem;
    wing::MenuItem* closeFileMenuItem;
    wing::MenuItem* exitMenuItem;
    wing::SplitContainer* verticalSplitContainer;
    wing::TabControl* topTabControl;
    wing::TabControl* bottomTabControl;
    std::unique_ptr<cmajor::systemx::guiterm::TerminalFile> terminalFile;
    cmajor::systemx::guiterm::TerminalControl* terminalControl;
    DebugView* currentTopView;
    DebugView* currentBottomView;
    std::vector<DebugView*> views;
    DataRanges dataRanges;
    bool waitingDebugger;
    std::mutex mtx;
    std::condition_variable debuggingStoppedOrErrorVar;
    wing::MenuItem* startContinueMenuItem;
    wing::MenuItem* stopMenuItem;
    wing::MenuItem* resetMenuItem;
    wing::MenuItem* singleStepMenuItem;
    wing::MenuItem* stepOverMenuItem;
    wing::MenuItem* toggleBreakpointMenuItem;
    wing::MenuItem* gotoPrevAddressMenuItem;

    wing::MenuItem* codeMenuItem;
    wing::MenuItem* dataMenuItem;
    wing::MenuItem* argsMenuItem;
    wing::MenuItem* envMenuItem;
    wing::MenuItem* heapMenuItem;
    wing::MenuItem* stackMenuItem;
    wing::MenuItem* regsMenuItem;
    wing::MenuItem* startMenuItem;
    wing::MenuItem* endMenuItem;
    wing::MenuItem* nextLineMenuItem;
    wing::MenuItem* prevLineMenuItem;
    wing::MenuItem* nextQuarterMenuItem;
    wing::MenuItem* prevQuarterMenuItem;
    wing::MenuItem* nextPageMenuItem;
    wing::MenuItem* prevPageMenuItem;
    DebuggingState state;
    bool fileOpen;
};

} // namespace sxgmdb
