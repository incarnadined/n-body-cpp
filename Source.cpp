#include "pch.h"

#include "Vec3.h"
#include "Graphics.h"
#include "Body.h"

#define WIDTH 800
#define HEIGHT 600

Graphics gfx;
bool paused = true;

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
	switch (uMsg)
	{
	case WM_SETCURSOR:
		SetCursor(NULL);
		return true;
	case WM_KEYDOWN:
		switch (wp)
		{
		case 'W':
			gfx.Translate(DirectX::XMVectorSet(0.0f, 0.0f, 0.01f, 0.0f));
			break;
		case 'S':
			gfx.Translate(DirectX::XMVectorSet(0.0f, 0.0f, -0.01f, 0.0f));
			break;
		case 'A':
			gfx.Translate(DirectX::XMVectorSet(-0.01f, 0.0f, 0.0f, 0.0f));
			break;
		case 'D':
			gfx.Translate(DirectX::XMVectorSet(0.01f, 0.0f, 0.0f, 0.0f));
			break;
		case 'P':
			paused = paused == true ? false : true;
			break;
		case VK_SPACE:
			gfx.Translate(DirectX::XMVectorSet(0.0f, 0.01f, 0.0f, 0.0f));
			break;
		case VK_SHIFT:
			gfx.Translate(DirectX::XMVectorSet(0.0f, -0.01f, 0.0f, 0.0f));
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"N-Body-Class";

	RegisterClassEx(&wc);

	// calculate the window dimensions to ensure the client area is WIDTH*HEIGHT
	RECT clientArea = {};
	clientArea.right = WIDTH;
	clientArea.bottom = HEIGHT;
	AdjustWindowRect(&clientArea, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE, false);

	HWND hWnd = CreateWindowEx(NULL, L"N-Body-Class", L"N-Body Simulation", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		200, 200, clientArea.right-clientArea.left, clientArea.bottom-clientArea.top, NULL, NULL, hInstance, NULL);

	gfx.Init(hWnd, WIDTH, HEIGHT);
	gfx.BindVertexShader(L"G:\\Coding Projects\\n-body-cpp\\VertexShader.hlsl");
	gfx.BindPixelShader(L"G:\\Coding Projects\\n-body-cpp\\PixelShader.hlsl");

	std::vector<Body> bodies;
	bodies.push_back(Body(1000, 0.15, { 0.5f, 0.4, 0.2 }, { 0.0f, 1.0f, 0.0f }));
	bodies.push_back(Body(5000, 0.35, { -0.2f, -0.4, 0.8 }, { 1.0f, 0.0f, 0.0f }));
	for (size_t i = 0; i < bodies.size(); i++)
	{
		gfx.AddSphere(std::bind(&Body::GetData, &bodies[i]));
	}

	std::set<std::pair<Body&, Body&>> combinations;
	for (size_t i = 0; i < bodies.size(); i++)
	{
		for (size_t j = 0; j < bodies.size(); j++)
		{
			combinations.insert(std::pair<Body&, Body&>(bodies[i], bodies[j]));
		}
	}

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, true))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Render
		gfx.Clear(Colour(0x08_uc, 0x1e_uc, 0x39_uc));
		if (!paused)
		{
			for (auto itr = combinations.begin(); itr != combinations.end(); itr++)
			{
				std::pair<Body&, Body&> pair = *itr;
				pair.first.CalculateForce(pair.second, 0.0001f);
			}
		}
		gfx.Draw();
		gfx.EndFrame();
	}
}