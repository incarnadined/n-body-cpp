#pragma once
#include "pch.h"

#include "Vec3.h"
#include "Camera.h"

struct Vertex 
{
	Vertex(float x, float y, float z, float r, float g, float b, float a) : x(x), y(y), z(z), r(r), g(g), b(b), a(a) {}
	Vertex(Vec3f pos, float r, float g, float b, float a) : x(pos.GetX()), y(pos.GetY()), z(pos.GetZ()), r(r), g(g), b(b), a(a) {}
	Vertex(float x, float y, float z, Colour colour) : x(x), y(y), z(z), r(colour.r), g(colour.g), b(colour.b), a(colour.a) {}
	Vertex(Vec3f pos, Colour colour) : x(pos.GetX()), y(pos.GetY()), z(pos.GetZ()), r(colour.r), g(colour.g), b(colour.b), a(colour.a) {}
	float x, y, z;
	float r, g, b, a;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX transform;
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND hWnd, float width, float height);
	void BindVertexShader(std::wstring filepath);
	void BindPixelShader(std::wstring filepath);
	void Clear(Colour colour); // clears the screen to the specified colour - resets rtv, dsv
	void AddSphere(std::function<std::tuple<float, Colour, Vec3f>()> dataFunction); // adds a sphere location callback to the active drawable objects - returns true on success
	void SubdivideIcosahedron(Vec3f v1, Vec3f v2, Vec3f v3, Colour colour, std::vector<Vertex>& verticies, std::vector<unsigned int>& indicies);
	std::pair<std::vector<Vertex>, std::vector<unsigned int>> GenerateSphere(float radius, Colour colour, Vec3f position, size_t offset); // returns vertex and index array for a sphere (starting with cubes for now)
	void Draw(); 
	void EndFrame();

	// camera
	void Translate(Vec3f vec);
	void SetCamera(Vec3f vec);
	Vec3f CameraPos;
	Vec3f CameraDir;

	// Getters/Setters
	float GetStarConc();
	void SetStarConc(float conc);
	int mDepth;


private:
	std::vector< // a vector containing pointers to the functions that can be called to get spherical data
		std::function<
			std::tuple<float, Colour, Vec3f>() // the function returns a tuple in the form <RADIUS, COLOUR, POSITION)
		>> pSpheres;

	float starConcentration; // star intensity (0-1)
	float mWidth;
	float mHeight;

	// com objects
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlobVS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPS;
};

