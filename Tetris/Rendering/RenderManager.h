#pragma once
#include <Windows.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <d3d.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <InputManager.h>
#include <cmath>

#pragma comment(lib, "d3d11.lib")

typedef struct _vertexPositionColor {
  DirectX::XMFLOAT3 pos;
  DirectX::XMFLOAT3 color;
} VertexPositionColor;

typedef struct _constantBufferStruct {
  DirectX::XMFLOAT4X4 world;
  DirectX::XMFLOAT4X4 view;
  DirectX::XMFLOAT4X4 projection;
} ConstantBufferStruct;

class RenderManager {
public:
  RenderManager();
  ~RenderManager();
  void startUp(HWND hWnd);
  void shutDown();
  void setDegreeAngle(Controller *input);
  void update();
  void draw();

private:
  Microsoft::WRL::ComPtr<ID3D11Device> m_pd3dDevice;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pd3dDeviceContext;
  Microsoft::WRL::ComPtr<IDXGISwapChain> m_pDXGISwapChain;
  D3D11_TEXTURE2D_DESC m_bbDesc;
  Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;
  ConstantBufferStruct m_constantBufferData;

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;

  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencil;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

  unsigned int  m_indexCount;
  unsigned int  m_frameCount;
  int yAngle;
  int xAngle;

  void createDXResources();
  void createSwapChain(HWND hWnd);
  HRESULT createBuffer();
  HRESULT setBuffers();
};