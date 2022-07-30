#pragma once

#include <Windows.h>
#include <string>
#include "RenderManager.h"
#include "InputManager.h"
#define global_variable static

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

static HINSTANCE hInstance;
static std::wstring windowClassName;
global_variable RenderManager gRenderManager;
global_variable InputManager gInputManager;
global_variable bool Running;