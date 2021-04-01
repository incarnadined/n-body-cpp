#include "pch.h"
#include "Graphics.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define THROWHR(hr) if(FAILED(hr)) {MessageBox(nullptr, (LPCWSTR)__LINE__, L"HR Error", MB_OK);}

std::pair<std::vector<Vertex>, std::vector<Triangle>> Graphics::mesh;

Graphics::Graphics()
	: starConcentration(0.5), mWidth(800), mHeight(600), currentDepth(3)
{
	SetSubdivisions(3);
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	pSwapChain->SetFullscreenState(false, nullptr);
}

void Graphics::Init(HWND hWnd, float width, float height)
{
	HRESULT hr = 0;
	mWidth = width;
	mHeight = height;

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
	dt.Width = (UINT)width;
	dt.Height = (UINT)height;
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

	// set default fullscreen true
	//pSwapChain->SetFullscreenState(true, nullptr);

	// init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplDX11_Init(pDevice.Get(), pDeviceContext.Get());
}

void Graphics::BindVertexShader(std::wstring filepath)
{
	HRESULT hr = 0;
	// compile shader
	Microsoft::WRL::ComPtr<ID3DBlob> errors;
	hr = D3DCompileFromFile(
		filepath.c_str(),
		NULL,
		NULL,
		"main",
		"vs_5_0",
		0,
		0,
		&pBlobVS,
		&errors
	);

	// load shader
	hr = pDevice->CreateVertexShader(
		pBlobVS->GetBufferPointer(),
		pBlobVS->GetBufferSize(),
		NULL,
		&pVS
	);

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
	HRESULT hr;

	// compile shader
	Microsoft::WRL::ComPtr<ID3DBlob> errors;
	hr = D3DCompileFromFile(
		filepath.c_str(),
		NULL,
		NULL,
		"main",
		"ps_5_0",
		0,
		0,
		&pBlobPS,
		&errors
	);

	// load shader
	hr = pDevice->CreatePixelShader(
		pBlobPS->GetBufferPointer(),
		pBlobPS->GetBufferSize(),
		NULL,
		&pPS
	);

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

void Graphics::AddSphere(std::function<std::tuple<float, Vec3f, Colour>()> dataFunction)
{
	pSpheres.push_back(dataFunction);
}

void Graphics::SetSubdivisions(int depth)
{
	currentDepth = depth;
	GenerateSphereMesh(depth);
}

Camera& Graphics::GetCamera()
{
	return camera;
}

void Graphics::SubdivideIcosahedron(int depth)
{
	if (depth == 0)
	{
		return;
	}

	std::vector<Vertex>& verticies = mesh.first;
	std::vector<Triangle>& indicies = mesh.second;
	std::vector<Triangle> newIndicies;
	for (size_t i = 0; i < indicies.size(); i += 1)
	{
		// push back the first three original verticies
		Vertex v12 = ((verticies[indicies[i].a] + verticies[indicies[i].b]) / 2).normalise();
		Vertex v13 = ((verticies[indicies[i].a] + verticies[indicies[i].c]) / 2).normalise();
		Vertex v23 = ((verticies[indicies[i].b] + verticies[indicies[i].c]) / 2).normalise();
		verticies.push_back(v12);
		verticies.push_back(v13);
		verticies.push_back(v23);
		
		// create a triangle with verticies v1, v12, v13
		newIndicies.push_back({ indicies[i].a, verticies.size() - 3, verticies.size() - 2 });
		// create a triangle with verticies v2, v23, v12
		newIndicies.push_back({ indicies[i].b, verticies.size() - 1, verticies.size() - 3 });
		// create a triangle with verticies v3, v13, v23
		newIndicies.push_back({ indicies[i].c, verticies.size() - 2, verticies.size() - 1 });
		// create a triangle with verticies v12, v23, v13
		newIndicies.push_back({ verticies.size() - 3, verticies.size() - 1, verticies.size() - 2 });
	}
	mesh.second.clear();
	mesh.second = newIndicies;

	return SubdivideIcosahedron(depth - 1);
}

void Graphics::GenerateSphereMesh(int depth)
{
	// generate icosahedron
	float X = 0.525731112119133606f;
	float Z = 0.850650808352039932f;
	float N = 0.0f;

	mesh.first =
	{
		{ -X, N, Z }, {  X, N, Z }, { -X, N,-Z, }, {  X, N,-Z, },
		{  N, Z, X }, {  N, Z,-X }, {  N,-Z, X, }, {  N,-Z,-X, },
		{  Z, X, N }, { -Z, X, N }, {  Z,-X, N, }, { -Z,-X, N, }
	};

	mesh.second = 
	{
		{ 0, 4,  1 }, { 0, 9, 4 },  { 9, 5,  4 }, { 4,  5, 8 }, { 4, 8, 1 },
		{ 8, 10, 1 }, { 8, 3, 10 }, { 5, 3,  8 }, { 5,  2, 3 }, { 2, 7, 3 },
		{ 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },
		{ 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9,  2, 5 }, { 7, 2, 11 }
	};

	SubdivideIcosahedron(depth);
}

void Graphics::DrawImGui()
{
	// add slider to change detail of spheres
	if (ImGui::SliderInt("Subdivisions", &currentDepth, 0, 5))
	{
		SetSubdivisions(currentDepth);
	}

	// draw framerate/fps
	double diff = clock.Diff();
	ImGui::Text("Framerate: %ims", diff);
	ImGui::Text("FPS: %f", 1000.0f / diff);
}

std::pair<std::vector<Vertex>, std::vector<unsigned int>> Graphics::GenerateSphere(float radius, Vec3f position, size_t offset)
{
	std::vector<Vertex> verticies;
	// apply translation and scale to verticies
	for (size_t i = 0; i < mesh.first.size(); i++)
	{
		verticies.push_back(mesh.first[i] * radius + position);
	}
	// add current offset in data to all indicies and convert to pure uint list
	std::vector<unsigned int> indicies;
	for (size_t i = 0; i < mesh.second.size(); i++)
	{
		indicies.push_back(mesh.second[i].a + offset);
		indicies.push_back(mesh.second[i].b + offset);
		indicies.push_back(mesh.second[i].c + offset);
	};

	return std::make_pair(verticies, indicies);
}

void Graphics::Draw()
{
	// retrieve vertex and index buffer data for the active spheres
	std::vector<Vertex> VData;
	std::vector<int> IData;
	for (std::size_t i = 0; i < pSpheres.size(); i++)
	{
		auto data = pSpheres[i]();
		if (std::get<0>(data) == 0.0f)
			continue; // if the radius is 0, the sphere is void and should not be rendered
		auto points = GenerateSphere(std::get<0>(data), std::get<1>(data), VData.size());
		VData.insert(VData.end(), points.first.begin(), points.first.end());
		IData.insert(IData.end(), points.second.begin(), points.second.end());
	}

	// create the vertex buffer
	D3D11_BUFFER_DESC VBufferDesc = {};
	VBufferDesc.ByteWidth = VData.size() * sizeof(Vertex);
	VBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBufferDesc.CPUAccessFlags = 0u;
	VBufferDesc.MiscFlags = 0u;
	VBufferDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA VBufferSR = {};
	VBufferSR.pSysMem = VData.data();
	VBufferSR.SysMemPitch = NULL;
	VBufferSR.SysMemSlicePitch = NULL;
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	THROWHR(pDevice->CreateBuffer(
		&VBufferDesc,
		&VBufferSR,
		&VertexBuffer
	));
	const UINT strides = sizeof(Vertex);
	const UINT offsets = 0u;
	pDeviceContext->IASetVertexBuffers(
		0u,
		1u,
		VertexBuffer.GetAddressOf(),
		&strides,
		&offsets
	);

	// create the index buffer
	D3D11_BUFFER_DESC IBufferDesc = {};
	IBufferDesc.ByteWidth = IData.size() * sizeof(int);
	IBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBufferDesc.CPUAccessFlags = 0u;
	IBufferDesc.MiscFlags = 0u;
	IBufferDesc.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA IBufferSR = {};
	IBufferSR.pSysMem = IData.data();
	IBufferSR.SysMemPitch = NULL;
	IBufferSR.SysMemSlicePitch = NULL;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
	THROWHR(pDevice->CreateBuffer(
		&IBufferDesc,
		&IBufferSR,
		&IndexBuffer
	));
	pDeviceContext->IASetIndexBuffer(
		IndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		0u
	);

	// create the matrix constant buffer
	ConstantBuffer CData =
	{
		{
			camera.GetCamera()
		}
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC CBufferDesc = {};
	CBufferDesc.ByteWidth = sizeof(CData);
	CBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	CBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CBufferDesc.MiscFlags = 0u;
	CBufferDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA CBufferSR = {};
	CBufferSR.pSysMem = &CData;
	THROWHR(pDevice->CreateBuffer(
		&CBufferDesc, 
		&CBufferSR, 
		&pConstantBuffer
	));
	// bind constant buffer
	pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// create the colour constant buffer
	ColourBuffer ColourData =
	{
		{
			(float)(20 * std::pow(4, currentDepth)),
			(float)(20 * std::pow(4, currentDepth)),
			(float)(20 * std::pow(4, currentDepth)),
			(float)(20 * std::pow(4, currentDepth))
		},
		{
			{ 0.0f, 0.0f, 0.0f, 0.0f }, // default all colour slots to black
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f }
		}
	};
	for (size_t i = 0; i < pSpheres.size(); i++)
	{
		ColourData.colours[i] = std::get<2>(pSpheres[i]());
	}
	Microsoft::WRL::ComPtr<ID3D11Buffer> pCConstantBuffer;
	D3D11_BUFFER_DESC CCBufferDesc = {};
	CCBufferDesc.ByteWidth = sizeof(ColourData);
	CCBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	CCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CCBufferDesc.CPUAccessFlags = 0u;
	CCBufferDesc.MiscFlags = 0u;
	CCBufferDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA CCBufferSR = {};
	CCBufferSR.pSysMem = &ColourData;
	THROWHR(pDevice->CreateBuffer(
		&CCBufferDesc,
		&CCBufferSR,
		&pCConstantBuffer
	));
	// bind constant buffer
	pDeviceContext->PSSetConstantBuffers(0u, 1u, pCConstantBuffer.GetAddressOf());
		
	// define the input layout
	const D3D11_INPUT_ELEMENT_DESC iLayout[] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;
	THROWHR(pDevice->CreateInputLayout(
		iLayout,
		std::size(iLayout),
		pBlobVS->GetBufferPointer(),
		pBlobVS->GetBufferSize(),
		pLayout.GetAddressOf()
	));
	pDeviceContext->IASetInputLayout(pLayout.Get());

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// define the viewport
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (float)mWidth;
	vp.Height = (float)mHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	pDeviceContext->RSSetViewports(1u, &vp);

	pDeviceContext->DrawIndexed(IData.size(), 0u, 0);
}

void Graphics::EndFrame()
{
	pSwapChain->Present(1u, 0u);
}

float Graphics::GetStarConc()
{
	return starConcentration;
}

void Graphics::SetStarConc(float conc)
{
	starConcentration = std::max(0.0f, std::min(conc, 1.0f));
}