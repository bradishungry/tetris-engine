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
#include "Core/NVCRoot.h"

static NVCRoot gNVCRoot;

/*
#pragma comment(lib, "d3d11.lib")
#define global_variable static
global_variable bool Running;
global_variable int Framerate = 60;

typedef struct _vertexPositionColor {
  DirectX::XMFLOAT3 pos;
  DirectX::XMFLOAT3 color;
} VertexPositionColor;

typedef struct _constantBufferStruct {
  DirectX::XMFLOAT4X4 world;
  DirectX::XMFLOAT4X4 view;
  DirectX::XMFLOAT4X4 projection;
} ConstantBufferStruct;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
  case WM_CLOSE: {
    Running = false;
  }

  case WM_DESTROY:
    Running = false;
    break;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
*/
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
	gNVCRoot.startUp();
	gNVCRoot.runGame();
	gNVCRoot.shutDown();
	
/*  // Window resources are dealt with here.

  auto m_windowClassName = L"TetrisWindow";

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
  wndClass.lpszClassName = m_windowClassName;

  if (!RegisterClass(&wndClass)) {
    DWORD dwError = GetLastError();
    if (dwError != ERROR_CLASS_ALREADY_EXISTS)
      return HRESULT_FROM_WIN32(dwError);
  }

  int x = CW_USEDEFAULT;
  int y = CW_USEDEFAULT;
  RECT m_rc;
  // This example uses a non-resizable 640 by 480 viewport for simplicity.
  int nDefaultWidth = 640;
  int nDefaultHeight = 480;
  SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
  AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, false);

  // Create the window for our viewport.
  auto m_hWnd = CreateWindow(m_windowClassName, L"Cube11", WS_OVERLAPPEDWINDOW,
                             x, y, (m_rc.right - m_rc.left),
                             (m_rc.bottom - m_rc.top), 0, NULL, hInstance, 0);

  if (m_hWnd == NULL) {
    DWORD dwError = GetLastError();
  }

  if (!IsWindowVisible(m_hWnd))
    ShowWindow(m_hWnd, SW_SHOW);

    D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_9_1,  D3D_FEATURE_LEVEL_9_2,
                                D3D_FEATURE_LEVEL_9_3,  D3D_FEATURE_LEVEL_10_0,
                                D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0,
                                D3D_FEATURE_LEVEL_11_1};

  UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  D3D_FEATURE_LEVEL m_featureLevel;
  Microsoft::WRL::ComPtr<ID3D11Device> m_pd3dDevice;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pd3dDeviceContext;
  Microsoft::WRL::ComPtr<ID3D11Device> device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
  HRESULT hr;
  hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags,
                         levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &device,
                         &m_featureLevel, &context);

  if (FAILED(hr)) {
  }

  device.As(&m_pd3dDevice);
  context.As(&m_pd3dDeviceContext);

  DXGI_SWAP_CHAIN_DESC desc;
  ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
  desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
  desc.BufferCount = 2;
  desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.SampleDesc.Count = 1; // multisampling setting
  desc.SampleDesc.Quality = 0; // vendor-specific flag
  desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  desc.OutputWindow = m_hWnd;

  // Create the DXGI device object to use in other factories, such as Direct2D.
  Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
  m_pd3dDevice.As(&dxgiDevice);

  // Create swap chain.
  Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
  Microsoft::WRL::ComPtr<IDXGIFactory> factory;
  Microsoft::WRL::ComPtr<IDXGISwapChain> m_pDXGISwapChain;

  hr = dxgiDevice->GetAdapter(&adapter);

  if (SUCCEEDED(hr)) {

    adapter->GetParent(IID_PPV_ARGS(&factory));

    hr = factory->CreateSwapChain(m_pd3dDevice.Get(), &desc, &m_pDXGISwapChain);
  }

  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBackBuffer;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;
  D3D11_TEXTURE2D_DESC m_bbDesc;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencil;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
  hr = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                   (void **)&m_pBackBuffer);

  hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr,
                                            m_pRenderTarget.GetAddressOf());

  m_pBackBuffer->GetDesc(&m_bbDesc);

  CD3D11_TEXTURE2D_DESC depthStencilDesc(
      DXGI_FORMAT_D24_UNORM_S8_UINT, static_cast<UINT>(m_bbDesc.Width),
      static_cast<UINT>(m_bbDesc.Height),
      1, // This depth stencil view has only one texture.
      1, // Use a single mipmap level.
      D3D11_BIND_DEPTH_STENCIL);

  m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencil);

  CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(
      D3D11_DSV_DIMENSION_TEXTURE2D);

  m_pd3dDevice->CreateDepthStencilView(
      m_pDepthStencil.Get(), &depthStencilViewDesc, &m_pDepthStencilView);

  D3D11_VIEWPORT m_viewport;

  ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
  m_viewport.Height = (float)m_bbDesc.Height;
  m_viewport.Width = (float)m_bbDesc.Width;
  m_viewport.MinDepth = 0;
  m_viewport.MaxDepth = 1;

  m_pd3dDeviceContext->RSSetViewports(1, &m_viewport);

  Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayoutExtended;

  FILE *vShader, *pShader;
  BYTE *bytes;

  size_t destSize = 32767;
  size_t bytesRead = 0;
  bytes = new BYTE[destSize];
  int s = fopen_s(&vShader, "CubeVertexShader.cso", "rb");
  bytesRead = fread_s(bytes, destSize, 1, 32767, vShader);
  hr = device->CreateVertexShader(bytes, bytesRead, nullptr, &m_pVertexShader);

  D3D11_INPUT_ELEMENT_DESC iaDesc[] = {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
       D3D11_INPUT_PER_VERTEX_DATA, 0},

      {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
       D3D11_INPUT_PER_VERTEX_DATA, 0},
  };

  hr = device->CreateInputLayout(iaDesc, ARRAYSIZE(iaDesc), bytes, bytesRead,
                                 &m_pInputLayout);

  delete bytes;

  bytes = new BYTE[destSize];
  bytesRead = 0;
  fopen_s(&pShader, "CubePixelShader.cso", "rb");
  bytesRead = fread_s(bytes, destSize, 1, 32767, pShader);
  hr = device->CreatePixelShader(bytes, bytesRead, nullptr,
                                 m_pPixelShader.GetAddressOf());

  delete bytes;

  CD3D11_BUFFER_DESC cbDesc(sizeof(ConstantBufferStruct),
                            D3D11_BIND_CONSTANT_BUFFER);

  hr = device->CreateBuffer(&cbDesc, nullptr, m_pConstantBuffer.GetAddressOf());

  fclose(vShader);
  fclose(pShader);

  VertexPositionColor CubeVertices[] = {
      {
          DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(0, 0, 0),
      },
      {
          DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(0, 0, 1),
      },
      {
          DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT3(0, 1, 0),
      },
      {
          DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0, 1, 1),
      },

      {
          DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(1, 0, 0),
      },
      {
          DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(1, 0, 1),
      },
      {
          DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT3(1, 1, 0),
      },
      {
          DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1, 1, 1),
      },
  };

  CD3D11_BUFFER_DESC vDesc(sizeof(CubeVertices), D3D11_BIND_VERTEX_BUFFER);

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;

  D3D11_SUBRESOURCE_DATA vData;
  ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
  vData.pSysMem = CubeVertices;
  vData.SysMemPitch = 0;
  vData.SysMemSlicePitch = 0;

  hr = device->CreateBuffer(&vDesc, &vData, &m_pVertexBuffer);

  // Create index buffer:
  unsigned short CubeIndices[] = {
      0, 2, 1, // -x
      1, 2, 3,

      4, 5, 6, // +x
      5, 7, 6,

      0, 1, 5, // -y
      0, 5, 4,

      2, 6, 7, // +y
      2, 7, 3,

      0, 4, 6, // -z
      0, 6, 2,

      1, 3, 7, // +z
      1, 7, 5,
  };

  UINT m_indexCount = ARRAYSIZE(CubeIndices);

  CD3D11_BUFFER_DESC iDesc(sizeof(CubeIndices), D3D11_BIND_INDEX_BUFFER);

  D3D11_SUBRESOURCE_DATA iData;
  ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
  iData.pSysMem = CubeIndices;
  iData.SysMemPitch = 0;
  iData.SysMemSlicePitch = 0;

  hr = device->CreateBuffer(&iDesc, &iData, &m_pIndexBuffer);

  ConstantBufferStruct m_constantBufferData;

  DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
  DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
  DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

  DirectX::XMStoreFloat4x4(
      &m_constantBufferData.view,
      DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, at, up)));

  float aspectRatioX =
      static_cast<float>(m_bbDesc.Width) / static_cast<float>(m_bbDesc.Height);
  ;
  float aspectRatioY =
      aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

  DirectX::XMStoreFloat4x4(
      &m_constantBufferData.projection,
      DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovRH(
          2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) /
                           aspectRatioY),
          aspectRatioX, 0.01f, 100.0f)));
  // The render loop is controlled here.
  bool bGotMsg;
  MSG msg;
  msg.message = WM_NULL;
  PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
  unsigned int m_frameCount = 0;
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

      // Rotate the cube 1 degree per frame.
      DirectX::XMStoreFloat4x4(
          &m_constantBufferData.world,
          DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(
              DirectX::XMConvertToRadians((float)m_frameCount++))));

      if (m_frameCount == MAXUINT)
        m_frameCount = 0;

      context->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr,
                                 &m_constantBufferData, 0, 0);
      auto renderTarget = m_pRenderTarget.Get();
      auto depthStencilView = m_pDepthStencilView.Get();
      // Clear the render target and the z-buffer.
      const float teal[] = {0.098f, 0.439f, 0.439f, 1.000f};
      context->ClearRenderTargetView(renderTarget, teal);
      context->ClearDepthStencilView(
          depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

      // Set the render target.
      context->OMSetRenderTargets(1, &renderTarget, depthStencilView);

      // Set up the IA stage by setting the input topology and layout.
      UINT stride = sizeof(VertexPositionColor);
      UINT offset = 0;

      context->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride,
                                  &offset);

      context->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

      context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      context->IASetInputLayout(m_pInputLayout.Get());

      // Set up the vertex shader stage.
      context->VSSetShader(m_pVertexShader.Get(), nullptr, 0);

      context->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

      // Set up the pixel shader stage.
      context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

      // Calling Draw tells Direct3D to start sending commands to the graphics
      // device.
      context->DrawIndexed(m_indexCount, 0, 0);
      m_pDXGISwapChain->Present(1, 0);
    }

    LARGE_INTEGER endCtr;
    QueryPerformanceCounter(&endCtr);

    LONGLONG timeElapsed = endCtr.QuadPart - lastCtr.QuadPart;

    auto secondsElapsed = ((timeElapsed * 1000) / perfCountFrequency) * 60;

    wchar_t buffer[256];
    wsprintf(buffer, L"Millisecond: %d\n", secondsElapsed);
    OutputDebugString(buffer);
    lastCtr = endCtr;
  }
  */
  return 0;
}
