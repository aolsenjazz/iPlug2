#pragma once

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

//TODO: would be nice not to put this here
#ifndef NO_IGRAPHICS
  #ifdef IGRAPHICS_AGG
    #define IGRAPHICS_DRAW_CLASS IGraphicsAGG
    #include "IGraphicsAGG.h"
  #elif defined IGRAPHICS_CAIRO
    #define IGRAPHICS_DRAW_CLASS IGraphicsCairo
    #include "IGraphicsCairo.h"
  #elif defined IGRAPHICS_NANOVG
    #define IGRAPHICS_DRAW_CLASS IGraphicsNanoVG
    #include "IGraphicsNanoVG.h"
  #else
    #define IGRAPHICS_DRAW_CLASS IGraphicsLice
    #include "IGraphicsLice.h"
  #endif
#endif

class IGraphicsWin : public IGRAPHICS_DRAW_CLASS
{
public:
  IGraphicsWin(IPlugBaseGraphics& plug, int w, int h, int fps);
  virtual ~IGraphicsWin();

  void SetHInstance(HINSTANCE hInstance) { mHInstance = hInstance; }

  void ForceEndUserEdit();

  void Resize(int w, int h);

  void HideMouseCursor();
  void ShowMouseCursor();
  int ShowMessageBox(const char* str, const char* caption, int type);

  void DrawScreen(const IRECT& rect) override;

  void* OpenWindow(void* pParentWnd);
  void CloseWindow();
  bool WindowIsOpen() { return (mPlugWnd); }
  
  void UpdateTooltips() {}

  void HostPath(WDL_String& path);
  void PluginPath(WDL_String& path);
  void DesktopPath(WDL_String& path);
  void VST3PresetsPath(WDL_String& path, bool isSystem = true);
  void AppSupportPath(WDL_String& path, bool isSystem = false);
  void SandboxSafeAppSupportPath(WDL_String& path) { AppSupportPath(path, false); }
  bool RevealPathInExplorerOrFinder(WDL_String& path, bool select = false);

  void PromptForFile(WDL_String& filename, EFileAction action = kFileOpen, WDL_String* pDir = 0, const char* ext = "");
  bool PromptForColor(IColor& colour, const char* str);

  IPopupMenu* GetItemMenu(long idx, long &idxInMenu, long &offsetIdx, IPopupMenu& baseMenu);
  HMENU CreateMenu(IPopupMenu& menu, long* offsetIdx);
  IPopupMenu* CreateIPopupMenu(IPopupMenu& menu, IRECT& areaRect);
  void CreateTextEntry(IControl* pControl, const IText& text, const IRECT& textRect, const char* str, IParam* pParam);

  bool OpenURL(const char* url, const char* msgWindowTitle = 0, const char* confirmMsg = 0, const char* errMsgOnFailure = 0);

  // Specialty use!
  void* GetWindow() { return mPlugWnd; }
  HWND GetParentWindow() { return mParentWnd; }
  HWND GetMainWnd();
  void SetMainWndClassName(const char* name) { mMainWndClassName.Set(name); }
//  void GetMainWndClassName(char* name) { strcpy(name, mMainWndClassName.Get()); }
  IRECT GetWindowRECT();
  void SetWindowTitle(const char* str);

  const char* GetGUIAPI() override { return "win32"; };
  
  bool GetTextFromClipboard(WDL_String& str);
protected:
  void SetTooltip(const char* tooltip);
  void ShowTooltip();
  void HideTooltip();

private:
  HINSTANCE mHInstance;
  HWND mPlugWnd, mParamEditWnd, mTooltipWnd;
  IControl* mEdControl;
  IParam* mEdParam;
  WNDPROC mDefEditProc;
  int mParamEditMsg;
  bool mShowingTooltip;
  int mTooltipIdx;
  COLORREF* mCustomColorStorage;

  DWORD mPID;
  HWND mParentWnd, mMainWnd;
  WDL_String mMainWndClassName;

public:
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK ParamEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  static BOOL CALLBACK FindMainWindow(HWND hWnd, LPARAM lParam);
};
