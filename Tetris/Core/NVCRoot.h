#pragma once

#include <Windows.h>
#include <string>
#include "definitions.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "SaveManager.h"

class NVCRoot {
public:
  NVCRoot();
  ~NVCRoot();
  void startUp();
  void shutDown();
  void runGame();
  HWND GetWindowHandle() { return m_hWnd; };
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                     LPARAM lParam);

private:
  HRESULT createWindow();
  HMENU m_hMenu;
  RECT m_rc;
  HWND m_hWnd;
};

global_variable HINSTANCE hInstance;
global_variable std::wstring windowClassName;
global_variable RenderManager gRenderManager;
global_variable InputManager gInputManager;
global_variable SaveManager gSaveManager;
global_variable bool Running;