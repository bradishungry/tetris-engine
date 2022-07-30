#include <dxgi.h>
#include <dxgi1_3.h>
#include <d3d.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>
#include <memory>
#include <ppltasks.h>
#include <filesystem>
#include "RenderManager.h"
#include "InputManager.h"
#pragma comment(lib, "d3d11.lib")

RenderManager::RenderManager() {}

RenderManager::~RenderManager() {}

void RenderManager::startUp(HWND hWnd) {
  m_frameCount = 0;
  yAngle = 0;
  xAngle = 0;
  createDXResources();
  createSwapChain(hWnd);
  createBuffer();
  setBuffers();
}

void RenderManager::shutDown() {}

void RenderManager::update() {
    /*if (input.left) {
        m_frameCount++;
    }
    if (input.right) {
        m_frameCount--;
    }*/
  // Rotate the cube 1 degree per frame.
  DirectX::XMStoreFloat4x4(
      &m_constantBufferData.world,
      DirectX::XMMatrixMultiplyTranspose(DirectX::XMMatrixRotationY(
          DirectX::XMConvertToRadians((float)yAngle)),
          DirectX::XMMatrixRotationX(
              DirectX::XMConvertToRadians((float)xAngle))));

  if (yAngle == MAXINT)
      yAngle = 0;
}

void RenderManager::setDegreeAngle(Controller *input) {
    if (input->left) {
        yAngle++;
    }
    if (input->right) {
        yAngle--;
    }
    if (input->forward) {
        xAngle++;
    }
    if (input->backward) {
        xAngle--;
    }
}

void RenderManager::draw() {
  m_pd3dDeviceContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr,
                                         &m_constantBufferData, 0, 0);
  auto renderTarget = m_pRenderTarget.Get();
  auto depthStencilView = m_pDepthStencilView.Get();
  // Clear the render target and the z-buffer.
  const float teal[] = {0.098f, 0.439f, 0.439f, 1.000f};
  m_pd3dDeviceContext->ClearRenderTargetView(renderTarget, teal);
  m_pd3dDeviceContext->ClearDepthStencilView(
      depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

  // Set the render target.
  m_pd3dDeviceContext->OMSetRenderTargets(1, &renderTarget, depthStencilView);

  // Set up the IA stage by setting the input topology and layout.
  UINT stride = sizeof(VertexPositionColor);
  UINT offset = 0;

  m_pd3dDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(),
                                          &stride, &offset);

  m_pd3dDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(),
                                        DXGI_FORMAT_R16_UINT, 0);

  m_pd3dDeviceContext->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  m_pd3dDeviceContext->IASetInputLayout(m_pInputLayout.Get());

  // Set up the vertex shader stage.
  m_pd3dDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);

  m_pd3dDeviceContext->VSSetConstantBuffers(0, 1,
                                            m_pConstantBuffer.GetAddressOf());

  // Set up the pixel shader stage.
  m_pd3dDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

  // Calling Draw tells Direct3D to start sending commands to the graphics
  // device.
  m_pd3dDeviceContext->DrawIndexed(m_indexCount, 0, 0);
  m_pDXGISwapChain->Present(1, 0);
}
void RenderManager::createDXResources() {

  D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_9_1,  D3D_FEATURE_LEVEL_9_2,
                                D3D_FEATURE_LEVEL_9_3,  D3D_FEATURE_LEVEL_10_0,
                                D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0,
                                D3D_FEATURE_LEVEL_11_1};

  UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  D3D_FEATURE_LEVEL m_featureLevel;
  Microsoft::WRL::ComPtr<ID3D11Device> device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
  HRESULT hr;
  hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags,
                         levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &device,
                         &m_featureLevel, &context);

  device.As(&m_pd3dDevice);
  context.As(&m_pd3dDeviceContext);
}

void RenderManager::createSwapChain(HWND hWnd) {
  DXGI_SWAP_CHAIN_DESC desc;
  ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
  desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
  desc.BufferCount = 2;
  desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.SampleDesc.Count = 1;   // multisampling setting
  desc.SampleDesc.Quality = 0; // vendor-specific flag
  desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  desc.OutputWindow = hWnd;

  // Create the DXGI device object to use in other factories, such as Direct2D.
  Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
  m_pd3dDevice.As(&dxgiDevice);

  // Create swap chain.
  Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
  Microsoft::WRL::ComPtr<IDXGIFactory> factory;

  HRESULT hr = dxgiDevice->GetAdapter(&adapter);

  if (SUCCEEDED(hr)) {

    adapter->GetParent(IID_PPV_ARGS(&factory));

    hr = factory->CreateSwapChain(m_pd3dDevice.Get(), &desc, &m_pDXGISwapChain);
  }
}

HRESULT RenderManager::createBuffer() {
  HRESULT hr = S_OK;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBackBuffer;

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

  return hr;
}

HRESULT RenderManager::setBuffers() {
  HRESULT hr = S_OK;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayoutExtended;

  FILE *vShader, *pShader;
  BYTE *bytes;

  size_t destSize = 32767;
  size_t bytesRead = 0;
  bytes = new BYTE[destSize];
  int s = fopen_s(&vShader, "CubeVertexShader.cso", "rb");
  bytesRead = fread_s(bytes, destSize, 1, 32767, vShader);
  hr = m_pd3dDevice->CreateVertexShader(bytes, bytesRead, nullptr,
                                        &m_pVertexShader);

  D3D11_INPUT_ELEMENT_DESC iaDesc[] = {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
       D3D11_INPUT_PER_VERTEX_DATA, 0},

      {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
       D3D11_INPUT_PER_VERTEX_DATA, 0},
  };

  hr = m_pd3dDevice->CreateInputLayout(iaDesc, ARRAYSIZE(iaDesc), bytes,
                                       bytesRead, &m_pInputLayout);

  delete bytes;

  bytes = new BYTE[destSize];
  bytesRead = 0;
  fopen_s(&pShader, "CubePixelShader.cso", "rb");
  bytesRead = fread_s(bytes, destSize, 1, 32767, pShader);
  hr = m_pd3dDevice->CreatePixelShader(bytes, bytesRead, nullptr,
                                       m_pPixelShader.GetAddressOf());

  delete bytes;

  CD3D11_BUFFER_DESC cbDesc(sizeof(ConstantBufferStruct),
                            D3D11_BIND_CONSTANT_BUFFER);

  hr = m_pd3dDevice->CreateBuffer(&cbDesc, nullptr,
                                  m_pConstantBuffer.GetAddressOf());

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
  D3D11_SUBRESOURCE_DATA vData;
  ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
  vData.pSysMem = CubeVertices;
  vData.SysMemPitch = 0;
  vData.SysMemSlicePitch = 0;

  hr = m_pd3dDevice->CreateBuffer(&vDesc, &vData, &m_pVertexBuffer);

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

  m_indexCount = ARRAYSIZE(CubeIndices);

  CD3D11_BUFFER_DESC iDesc(sizeof(CubeIndices), D3D11_BIND_INDEX_BUFFER);

  D3D11_SUBRESOURCE_DATA iData;
  ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
  iData.pSysMem = CubeIndices;
  iData.SysMemPitch = 0;
  iData.SysMemSlicePitch = 0;

  hr = m_pd3dDevice->CreateBuffer(&iDesc, &iData, &m_pIndexBuffer);

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

  return hr;
}