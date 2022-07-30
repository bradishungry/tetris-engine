#include <Windows.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <d3d.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <cmath>
#include <string>
#include <memory>
#include <ppltasks.h>
#include <filesystem>
#include "NVCRoot.h"
#include "definitions.h"


internal_static NVCRoot gNVCRoot;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
	gNVCRoot.startUp();
	gNVCRoot.runGame();
	gNVCRoot.shutDown();
  return 0;
}
