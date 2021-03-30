#pragma once
#include "pch.h"

#include "Vec3.h"
#include "Camera.h"

struct Vertex 
{
	Vertex(float x, float y, float z, float colourID) : x(x), y(y), z(z), colourID(colourID) {}
	Vertex(Vec3f pos, float colourID) : x(pos.GetX()), y(pos.GetY()), z(pos.GetZ()), colourID(colourID) {}
	Vertex operator+(Vertex other) { return Vertex(x + other.x, y + other.y, z + other.z, colourID); }
	Vertex& operator+=(Vec3f other) { x + other.GetX(); y + other.GetY(); z + other.GetZ(); return *this;  }
	Vertex& operator*=(float other) { x * other; y * other; z * other; return *this; }
	Vertex operator/(int other) { return Vertex(x / other, y / other, z / other, colourID); }
	float magnitude() { return (float)std::pow(x * x + y * y + z * z, 0.5); }
	Vertex normalise() { return Vertex(x /= magnitude(), y /= magnitude(), z /= magnitude(), colourID); }
	float x, y, z; 
	float colourID;
};

struct Triangle
{
	unsigned int a, b, c;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX transform;
};
struct ColourBuffer
{
	struct
	{
		float r;
		float g;
		float b;
		float a;
	} colours[4];
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
	void AddSphere(std::function<std::tuple<float, Vec3f>()> dataFunction); // adds a sphere location callback to the active drawable objects
	std::pair<std::vector<Vertex>, std::vector<Triangle>> SubdivideIcosahedron(std::vector<Vertex> verticies, std::vector<Triangle> indicies, int depth);
	std::pair<std::vector<Vertex>, std::vector<unsigned int>> GenerateSphere(float radius, Vec3f position, size_t offset); // returns vertex and index array for a sphere (starting with cubes for now)
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
			std::tuple<float, Vec3f>() // the function returns a tuple in the form <RADIUS, COLOUR, POSITION)
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

