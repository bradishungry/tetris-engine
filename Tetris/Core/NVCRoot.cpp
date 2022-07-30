#include "NVCRoot.h"

NVCRoot::NVCRoot() {}

NVCRoot::~NVCRoot() {}

void NVCRoot::startUp() {
  createWindow();
  gInputManager.startUp();
  gSaveManager.startUp();
  gRenderManager.startUp(m_hWnd);
}

void NVCRoot::shutDown() {
  gRenderManager.shutDown();
  gSaveManager.shutDown();
  gInputManager.shutDown();
}

void NVCRoot::runGame() {
    auto list = gSaveManager.scoreList;
    for (int i = 0; i < gSaveManager.scoreCount; i += 1) {
        char buffer[256];
        sprintf_s(buffer, "Player: %s, Score: %d, Date: %s\n",
            list->playerName, list->score, list->date);
        OutputDebugStringA(buffer);
        list += 1;
    }

  LARGE_INTEGER perfCountFrequencyResult;
  QueryPerformanceFrequency(&perfCountFrequencyResult);
  LONGLONG perfCountFrequency = perfCountFrequencyResult.QuadPart;
  
  // The render lpop is controlled here.
  bool bGotMsg;
  MSG msg;
  msg.message = WM_NULL;
  PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
  Running = true;
  LARGE_INTEGER lastCtr;
  QueryPerformanceCounter(&lastCtr);
  while (Running) {
    // Process window events.
    // Use PeekMessage() so we can use idle time to render the scene.
    bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

    if (bGotMsg) {
      // Translate and dispatch the message
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
        gInputManager.pollGamepadInputs();
      gRenderManager.setDegreeAngle(&gInputManager.controller);
      gRenderManager.update();
      gRenderManager.draw();
    }

    LARGE_INTEGER endCtr;
    QueryPerformanceCounter(&endCtr);

    LONGLONG timeElapsed = endCtr.QuadPart - lastCtr.QuadPart;

    auto secondsElapsed = ((timeElapsed * 999) / perfCountFrequency) * 60;

    //wchar_t buffer[255];
    //wsprintf(buffer, L"Millisecond: %d\n", secondsElapsed);
    //OutputDebugString(buffer);
    lastCtr = endCtr;
  }
}

LRESULT CALLBACK NVCRoot::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                     LPARAM lParam) {
  switch (uMsg) {
	case WM_KEYDOWN:
	case WM_KEYUP:
    {
        bool wasDown = ((lParam & (1 << 30)) != 0);
        bool isDown = ((lParam & (1 << 31)) == 0);
        gInputManager.pollKeyboardInputs(wParam, wasDown, isDown);
    }
	break;
  case WM_CLOSE: {
    Running = false;
	break;
  }

  case WM_DESTROY:
    Running = false;
    break;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HRESULT NVCRoot::createWindow() {
  HRESULT hr = S_OK;
  windowClassName = L"TetrisWindow";

  if (hInstance == NULL)
    hInstance = (HINSTANCE)GetModuleHandle(NULL);

  HICON hIcon = NULL;
  WCHAR szExePath[MAX_PATH];
  GetModuleFileName(NULL, szExePath, MAX_PATH);

  // If the icon is NULL, then use the first one found in the exe
  if (hIcon == NULL)
    hIcon = ExtractIcon(hInstance, szExePath, 0);

  // Register the windows class
  WNDCLASS wndClass;
  wndClass.style = CS_DBLCLKS;
  wndClass.lpfnWndProc = WindowProc;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = hInstance;
  wndClass.hIcon = hIcon;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wndClass.lpszMenuName = NULL;
  wndClass.lpszClassName = windowClassName.c_str();

  if (!RegisterClass(&wndClass)) {
    DWORD dwError = GetLastError();
    if (dwError != ERROR_CLASS_ALREADY_EXISTS)
      return HRESULT_FROM_WIN32(dwError);
  }

  int x = CW_USEDEFAULT;
  int y = CW_USEDEFAULT;
  m_rc;
  // This example uses a non-resizable 640 by 480 viewport for simplicity.
  int nDefaultWidth = 640;
  int nDefaultHeight = 480;
  SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
  AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, false);

  // Create the window for our viewport.
  m_hWnd = CreateWindow(windowClassName.c_str(), L"Cube11", WS_OVERLAPPEDWINDOW,
                        x, y, (m_rc.right - m_rc.left),
                        (m_rc.bottom - m_rc.top), 0, NULL, hInstance, 0);

  if (m_hWnd == NULL) {
    DWORD dwError = GetLastError();
  }

  if (!IsWindowVisible(m_hWnd))
    ShowWindow(m_hWnd, SW_SHOW);

  return hr;
}