#include "pch.h"

#include "Vec3.h"
#include "Graphics.h"
#include "Clock.h"
#include "Body.h"
#include "resource.h"

#define WIDTH 1920
#define HEIGHT 1080

Graphics gfx;
bool paused = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
	// give the msg to imgui first
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wp, lp))
	{
		return true;
	}

	switch (uMsg)
	{
	case WM_SETCURSOR:
		SetCursor(NULL);
		return true;
	case WM_KEYDOWN:
		switch (wp)
		{
		case 'W':
			gfx.GetCamera().Translate({ 0.0f, 0.0f, 0.01f });
			break;
		case 'S':
			gfx.GetCamera().Translate({ 0.0f, 0.0f, -0.01f });
			break;
		case 'A':
			gfx.GetCamera().Translate({ -0.01f, 0.0f, 0.0f });
			break;
		case 'D':
			gfx.GetCamera().Translate({ 0.01f, 0.0f, 0.0f });
			break;
		case VK_SPACE:
			gfx.GetCamera().Translate({ 0.0f, 0.01f, 0.0f });
			break;
		case VK_SHIFT:
			gfx.GetCamera().Translate({ 0.0f, -0.01f, 0.0f });
			break;
		case 'P':
			paused = paused == true ? false : true;
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
	wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"N-Body-Class";
	wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, NULL);

	RegisterClassEx(&wc);

	// calculate the window dimensions to ensure the client area is WIDTH*HEIGHT
	RECT clientArea = {};
	clientArea.right = WIDTH;
	clientArea.bottom = HEIGHT;
	AdjustWindowRect(&clientArea, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE, false);

	HWND hWnd = CreateWindowEx(NULL, L"N-Body-Class", L"N-Body Simulation", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		0, 0, clientArea.right-clientArea.left, clientArea.bottom-clientArea.top, NULL, NULL, hInstance, NULL);

	gfx.Init(hWnd, WIDTH, HEIGHT);
	gfx.BindVertexShader(L"G:\\Coding Projects\\n-body-cpp\\src\\VertexShader.hlsl");
	gfx.BindPixelShader(L"G:\\Coding Projects\\n-body-cpp\\src\\PixelShader.hlsl");
	ImGui_ImplWin32_Init(hWnd);

	std::vector<Body> bodies;
	bodies.push_back(Body(1, 0.15f, { 1.7f, 0.4f, 0.2f }, { 0x50_uc, 0x40, 0x51 }));
	bodies.push_back(Body(4, 0.35f, { -0.2f, -0.3f, 1.2f }, { 0xDC_uc, 0x76, 0x4D }));
	bodies.push_back(Body(1.5, 0.19f, { -1.8f, -0.4f, 0.8f }, { 0x27_uc, 0x30, 0x57 }));
	bodies.push_back(Body(2.7, 0.24f, { -1.4f, 0.6f, 3.0f }, { 0x60_uc, 0xA8, 0x9B }));
	for (size_t i = 0; i < bodies.size(); i++)
	{
		gfx.AddSphere(std::bind(&Body::GetData, &bodies[i]));
	}

	std::set<std::pair<Body&, Body&>> combinations;
	for (size_t i = 0; i < bodies.size(); i++)
	{
		for (size_t j = 0; j < bodies.size(); j++)
		{
			if (i < j) // avoid generating permutations
				combinations.insert(std::pair<Body&, Body&>(bodies[i], bodies[j]));
		}
	}

	float dt = 0.001f;
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
			{ // for each combination of bodies, apply the force	
				std::pair<Body&, Body&> pair = *itr;
				pair.first.ApplyForce(pair.second, dt);
			}
		}
		gfx.Draw();

		// render imgui
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Checkbox("Paused", &paused);
		if (ImGui::CollapsingHeader("Bodies"))
		{
			for (size_t i = 0; i < bodies.size(); i++)
			{
				bodies[i].DrawImGui(i);
				if (ImGui::Button("Snap"))
				{
					gfx.GetCamera().SetCamera(bodies[i].GetPosition());
				}
				ImGui::Separator();
			}
		}
		if (ImGui::CollapsingHeader("Camera"))
		{
			gfx.GetCamera().DrawImGui();
		}
		ImGui::SliderFloat("Timestep", &dt, 0.001f, 1);
		gfx.DrawImGui();
		if (ImGui::Button("Step"))
		{
			for (auto itr = combinations.begin(); itr != combinations.end(); itr++)
			{ // for each combination of bodies, apply the force	
				std::pair<Body&, Body&> pair = *itr;
				pair.first.ApplyForce(pair.second, dt);
			}
		}
		if (ImGui::Button("Reset"))
		{
			for (size_t i = 0; i < bodies.size(); i++)
			{
				bodies[i].Reset();
			}
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		gfx.EndFrame();
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	UnregisterClassW(L"N-Body-Class", hInstance);
}