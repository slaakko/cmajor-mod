// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#include <gdiplus.h>

export module wing.window;

import wing.application;
import wing.base;
import wing.container.control;
import wing.control;
import wing.event;
import wing.graphics;
import wing.keys;
import wing.window.frame;
import std.core;

export namespace wing {

struct NCPaintEventArgs
{
    NCPaintEventArgs(HWND hwnd_, HRGN hrgn_) : hwnd(hwnd_), hrgn(hrgn_), handled(false) {}
    HWND hwnd;
    HRGN hrgn;
    bool handled;
};

using NCPaintEvent = EventWithArgs<NCPaintEventArgs>;

enum class WindowState : int
{
    normal, minimized, maximized
};

int GetDialogResult(void* dialogWindowPtr);
void DialogWindowKeyPreview(void* dialogWindowPtr, Keys key, bool& handled);

enum class DialogResult : int
{
    none = 0, ok = 1, cancel = 2, abort = 3, retry = 4, ignore = 5, yes = 6, no = 7
};

class Button;
class MenuBar;
class ContextMenu;

using WindowStateChangedEvent = Event;
using WindowClosingEvent = EventWithArgs<wing::CancelArgs>;
using WindowClosedEvent = Event;

inline int64_t DefaultWindowClassWindowBackgroundColor()
{
    return COLOR_WINDOW;
}

inline Color DefaultWindowBackgroundColor()
{
    return GetSystemColor(DefaultWindowClassWindowBackgroundColor());
}

std::string DefaultWindowFontFamilyName();
float DefaultWindowFontSize();
FontStyle DefaultWindowFontStyle();
std::string DefaultWindowCaptionFontFamilyName();
float DefaultWindowCaptionFontPercent();

struct WindowCreateParams
{
    WindowCreateParams();
    WindowCreateParams& Defaults();
    WindowCreateParams& WindowClassName(const std::string& windowClassName_);
    WindowCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    WindowCreateParams& WindowStyle(int windowStyle_);
    WindowCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    WindowCreateParams& BackgroundColor(const Color& backgroundColor_);
    WindowCreateParams& Text(const std::string& text_);
    WindowCreateParams& Location(Point location_);
    WindowCreateParams& SetSize(Size size_);
    WindowCreateParams& SetAnchors(Anchors anchors_);
    WindowCreateParams& SetDock(Dock dock_);
    WindowCreateParams& FontFamilyName(const std::string& fontFamilyName_);
    WindowCreateParams& FontSize(float fontSize_);
    WindowCreateParams& SetFontStyle(FontStyle fontStyle_);
    WindowCreateParams& CaptionFontFamilyName(const std::string& captionFontFamilyName_);
    WindowCreateParams& CaptionFontPercent(float captionFontPercent_);
    ControlCreateParams controlCreateParams;
    std::string fontFamilyName;
    float fontSize;
    FontStyle fontStyle;
    std::string captionFontFamilyName;
    float captionFontPercent;
};

class Window : public ContainerControl
{
public:
    Window(WindowCreateParams& createParams);
    bool IsWindow() const override { return true; }
    void SetAsMainWindow() { mainWindow = true; }
    bool IsMainWindow() const { return mainWindow; }
    bool ShowingDialog() const { return showingDialog; }
    void SetShowingDialog() { showingDialog = true; }
    void ResetShowingDialog() { showingDialog = false; }
    void Close();
    Button* DefaultButton() const { return defaultButton; }
    void SetDefaultButton(Button* defaultButton_);
    Button* CancelButton() const { return cancelButton; }
    void SetCancelButton(Button* cancelButton_) { cancelButton = cancelButton_; }
    Control* GetFocusedControl() const { return focusedControl; }
    void SetFocusedControl(Control* focusedControl_) { focusedControl = focusedControl_; }
    void ResetFocusedControl() { focusedControl = nullptr; }
    void FocusNext();
    void FocusPrev();
    DialogResult ShowDialog(Window& parentWindow);
    DialogResult GetDialogResult() const { return dialogResult; }
    void SetDialogResult(DialogResult dialogResult_) { dialogResult = dialogResult_; }
    void MouseUpNotificationInternal(MouseEventArgs& args) { MouseUpNotification(args); }
    MenuBar* GetMenuBar() const { return menuBar; }
    ContextMenu* GetContextMenu() const { return contextMenu; }
    void SetContextMenu(ContextMenu* contextMenu_);
    void ShowContextMenu(ContextMenu* contextMenu_, Point& ptInScreenCoords);
    void RemoveContextMenu();
    void HideContextMenu();
    void SetIcon(const Icon& icon);
    void SetSmallIcon(const Icon& icon);
    KeyPreviewMethod GetKeyPreviewMethod() const;
    void SetKeyPreviewMethod(KeyPreviewMethod& keyPreviewMethod_);
    KeyPreviewMethod GetDialogKeyPreviewMethod() const;
    void SetDialogKeyPreviewMethod(KeyPreviewMethod& dialogKeyPreviewMethod_);
    void DefaultKeyPreview(Keys key, bool& handled);
    WindowState GetWindowState() const { return windowState; }
    void SetWindowState(WindowState newWindowState);
    WindowStateChangedEvent& WindowStateChanged() { return windowStateChanged; }
    void ShowWindow(int showCommand);
    void HandleNCPaint(NCPaintEventArgs& args);
    void UpdateColors() override;
    bool Active() const { return windowActive; }
protected:
    virtual void MouseUpNotification(MouseEventArgs& args);
    bool ProcessMessage(Message& msg) override;
    void OnKeyDown(KeyEventArgs& args) override;
    void OnPaint(PaintEventArgs& args) override;
    void OnControlAdded(ControlEventArgs& args) override;
    void OnControlRemoved(ControlEventArgs& args) override;
    void OnMouseDown(MouseEventArgs& args) override;
    void OnMouseUp(MouseEventArgs& args) override;
    void OnMouseMove(MouseEventArgs& args) override;
    void OnGotFocus() override;
    void OnLostFocus() override;
    void OnLocationChanged() override;
    virtual void OnWindowStateChanged();
    virtual void OnWindowClosing(wing::CancelArgs& args);
    virtual void OnWindowClosed(bool& processed);
    virtual void OnNCPaint(NCPaintEventArgs& args);
    virtual void PaintWindowRect(Graphics& graphics);
    virtual bool OnNCLButtonDown(int hitTestValue);
    virtual bool OnNCLButtonUp(int hitTestValue);
private:
    void DoWindowStateChanged(int sizeType);
    void DrawWindowCaption(Graphics& graphics);
    void PaintFrame(Graphics& graphics);
    void MakeCaptionResources();
    void MakeWindowFrame();
    std::string fontFamilyName;
    float fontSize;
    FontStyle fontStyle;
    std::string captionFontFamilyName;
    float captionFontPercent;
    int captionHeight;
    int borderWidth;
    int buttonWidth;
    int buttonHeight;
    bool windowActive;
    std::unique_ptr<Font> captionFont;
    std::unique_ptr<SolidBrush> captionBrush;
    std::unique_ptr<SolidBrush> disabledCaptionBrush;
    std::unique_ptr<Pen> sysButtonPen;
    std::unique_ptr<Pen> sysButtonSelectedPen;
    std::unique_ptr<Pen> sysButtonDisabledPen;
    std::unique_ptr<SolidBrush> titleBarBackgroundBrush;
    std::unique_ptr<SolidBrush> sysButtonHoverBackgroundBrush;
    std::unique_ptr<WindowFrame> frame;
    int ncLButtonDownHitTest;
    bool mainWindow;
    bool showingDialog;
    bool flatSBInitialized;
    WindowState windowState;
    Button* defaultButton;
    Button* cancelButton;
    Control* focusedControl;
    MenuBar* menuBar;
    ContextMenu* contextMenu;
    DialogResult dialogResult;
    WindowClosingEvent windowClosing;
    WindowClosedEvent windowClosed;
    WindowStateChangedEvent windowStateChanged;
    NCPaintEvent ncPaint;
    KeyPreviewMethod keyPreviewMethod;
    KeyPreviewMethod dialogKeyPreviewMethod;
};

} // wing
