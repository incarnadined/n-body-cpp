#pragma once
#include "pch.h"

#include "Vec3.h"

struct Vertex 
{
	float x, y, z;
	float r, g, b, a;
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND hWnd, int width, int height);
	void BindVertexShader(std::wstring filepath);
	void BindPixelShader(std::wstring filepath);
	void Clear(Colour colour); // clears the screen to the specified colour - resets rtv, dsv
	void AddSphere(std::function<std::tuple<float, Colour, Vec3f>()> dataFunction); // adds a sphere location callback to the active drawable objects - returns true on success
	std::pair<std::vector<Vertex>, std::vector<unsigned int>> GenerateSphere(float radius, Colour colour, Vec3f position, size_t offset); // returns vertex and index array for a sphere (starting with cubes for now)
	void Draw(); 
	void EndFrame();

	// Getters/Setters
	float GetStarConc();
	void SetStarConc(float conc);
	Vec3f GetCameraPos();
	void MoveCamera(Vec3f translation);

private:
	std::vector< // a vector containing pointers to the functions that can be called to get spherical data
		std::function<
			std::tuple<float, Colour, Vec3f>() // the function returns a tuple in the form <RADIUS, COLOUR, POSITION)
		>> pSpheres;

	float starConcentration; // star intensity (0-1)
	Vec3f cameraPos;
	int mWidth;
	int mHeight;

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

