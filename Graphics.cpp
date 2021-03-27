#include "Graphics.h"

#pragma comment (lib, "d3d11.lib")

#define THROWHR(hr) if(FAILED(hr)) {MessageBox(nullptr, (LPCWSTR)__LINE__, L"HR Error", MB_OK);}

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::Init(HWND hWnd, int width, int height)
{
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	THROWHR(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		6u,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		NULL,
		&pDeviceContext));

	// Generate the rtv
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	THROWHR(pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRTV));

	// Generate the depth state
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthState;
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = true;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	THROWHR(pDevice->CreateDepthStencilState(&dsd, &pDepthState));

	// Bind the depth state
	pDeviceContext->OMSetDepthStencilState(pDepthState.Get(), 1u);

	// Generate depth texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthTexture;
	D3D11_TEXTURE2D_DESC dt = {};
	dt.Width = width;
	dt.Height = height;
	dt.MipLevels = 1u;
	dt.ArraySize = 1u;
	dt.Format = DXGI_FORMAT_D32_FLOAT;
	dt.SampleDesc.Count = 1u;
	dt.SampleDesc.Quality = 0u;
	dt.Usage = D3D11_USAGE_DEFAULT;
	dt.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	THROWHR(pDevice->CreateTexture2D(&dt, nullptr, &pDepthTexture));

	// Generate depth view
	D3D11_DEPTH_STENCIL_VIEW_DESC dvd = {};
	dvd.Format = DXGI_FORMAT_D32_FLOAT;
	dvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dvd.Texture2D.MipSlice = 0u;
	THROWHR(pDevice->CreateDepthStencilView(pDepthTexture.Get(), &dvd, &pDSV));

	// Bind depth view
	pDeviceContext->OMSetRenderTargets(1u, pRTV.GetAddressOf(), pDSV.Get());
}

void Graphics::BindVertexShader(std::wstring filepath)
{
}

void Graphics::BindPixelShader(std::wstring filepath)
{
}

void Graphics::Clear(colour colour)
{
}

bool Graphics::AddSphere(std::function<std::tuple<float, colour, Vec3f>()> dataFunction)
{
	return false;
}

void Graphics::Draw()
{
}

void Graphics::EndFrame()
{
}

float Graphics::GetStarConc()
{
	return 0.0f;
}

void Graphics::SetStarConc()
{
}

Vec3f Graphics::GetCameraPos()
{
	return Vec3f();
}

void Graphics::MoveCamera(Vec3f translation)
{
}
