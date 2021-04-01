#pragma once
#include "pch.h"

#include "Vec3.h"
#include "Clock.h"
#include "Camera.h"

struct Vertex 
{
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
	Vertex(Vec3f pos, float colourID) : x(pos.GetX()), y(pos.GetY()), z(pos.GetZ()) {}
	Vertex operator+(Vertex other) { return Vertex(x + other.x, y + other.y, z + other.z); }
	Vertex operator+(Vec3f other) { return Vertex(x + other.GetX(), y + other.GetY(), z + other.GetZ()); }
	Vertex operator*(float other) { return Vertex(x * other, y * other, z * other); }
	Vertex operator/(int other) { return Vertex(x / other, y / other, z / other); }
	float magnitude() { return (float)std::pow(x * x + y * y + z * z, 0.5); }
	Vertex normalise() { return Vertex(x /= magnitude(), y /= magnitude(), z /= magnitude()); }
	float x, y, z; 
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
	float trianglesPerSphere[4];
	Colour colours[8];
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
	void AddSphere(std::function<std::tuple<float, Vec3f, Colour>()> dataFunction); // adds a sphere location callback to the active drawable objects
	static void SubdivideIcosahedron(int depth);
	static void GenerateSphereMesh(int depth); // populates mesh with vertex and index array for a sphere
	std::pair<std::vector<Vertex>, std::vector<unsigned int>> GenerateSphere(float radius, Vec3f position, size_t offset); // returns vertex and index array for a sphere
	void Draw(); 
	void DrawImGui();
	void EndFrame();

	// Getters/Setters
	float GetStarConc();
	void SetStarConc(float conc);
	void SetSubdivisions(int depth);

	Camera& GetCamera();

private:
	std::vector< // a vector containing pointers to the functions that can be called to get spherical data
		std::function<
			std::tuple<float, Vec3f, Colour>() // the function returns a tuple in the form <RADIUS, POSITION, POSITION>
		>> pSpheres;
	static std::pair<std::vector<Vertex>, std::vector<Triangle>> mesh;

	float starConcentration; // star intensity (0-1)
	float mWidth;
	float mHeight;
	int currentDepth = 3;
	Camera camera;
	Clock clock;

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

