#include "pch.h"
#include "Graphics.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define THROWHR(hr) if(FAILED(hr)) {MessageBox(nullptr, (LPCWSTR)__LINE__, L"HR Error", MB_OK);}

Graphics::Graphics()
	: cameraPos(0.0f, 0.0f, 0.0f), starConcentration(0.5)
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
		0,
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
	// compile shader
	Microsoft::WRL::ComPtr<ID3DBlob> errors;
	THROWHR(D3DCompileFromFile(
		filepath.c_str(),
		NULL,
		NULL,
		"main",
		"vs_5_0",
		0,
		0,
		&pBlobVS,
		&errors
	));

	// load shader
	THROWHR(pDevice->CreateVertexShader(
		pBlobVS.Get(),
		pBlobVS->GetBufferSize(),
		NULL,
		&pVS
	));

	// set shader
	pDeviceContext->VSSetShader(
		pVS.Get(),
		NULL,
		0
	);
}

void Graphics::BindPixelShader(std::wstring filepath)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlobPS;

	// compile shader
	Microsoft::WRL::ComPtr<ID3DBlob> errors;
	THROWHR(D3DCompileFromFile(
		filepath.c_str(),
		NULL,
		NULL,
		"main",
		"ps_5_0",
		0,
		0,
		&pBlobPS,
		&errors
	));

	// load shader
	THROWHR(pDevice->CreatePixelShader(
		pBlobPS.Get(),
		pBlobPS->GetBufferSize(),
		NULL,
		&pPS
	));

	// set shader
	pDeviceContext->PSSetShader(
		pPS.Get(),
		NULL,
		0
	);
}

void Graphics::Clear(Colour colour)
{
	float c[4] = { colour.r, colour.g, colour.b, colour.a };
	pDeviceContext->ClearRenderTargetView(pRTV.Get(), c);
	pDeviceContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

bool Graphics::AddSphere(std::function<std::tuple<float, Colour, Vec3f>()> dataFunction)
{
	return false;
}

void Graphics::Draw()
{
}

void Graphics::EndFrame()
{
	pSwapChain->Present(1u, 0u);
}

float Graphics::GetStarConc()
{
	return 0.0f;
}

void Graphics::SetStarConc(float conc)
{
	starConcentration = std::max(0.0f, std::min(conc, 1.0f));
}

Vec3f Graphics::GetCameraPos()
{
	return Vec3f();
}

void Graphics::MoveCamera(Vec3f translation)
{
}
