#include "pch.h"

#include "Vec3.h"
#include "Graphics.h"
#include "Body.h"

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
			gfx.Translate({ 0.0f, 0.0f, 0.01f });
			break;
		case 'S':
			gfx.Translate({ 0.0f, 0.0f, -0.01f });
			break;
		case 'A':
			gfx.Translate({ -0.01f, 0.0f, 0.0f });
			break;
		case 'D':
			gfx.Translate({ 0.01f, 0.0f, 0.0f });
			break;
		case VK_SPACE:
			gfx.Translate({ 0.0f, 0.01f, 0.0f });
			break;
		case VK_SHIFT:
			gfx.Translate({ 0.0f, -0.01f, 0.0f });
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
	gfx.BindVertexShader(L"G:\\Coding Projects\\n-body-cpp\\src\\VertexShader.hlsl");
	gfx.BindPixelShader(L"G:\\Coding Projects\\n-body-cpp\\src\\PixelShader.hlsl");
	ImGui_ImplWin32_Init(hWnd);

	std::vector<Body> bodies;
	bodies.push_back(Body(10, 0.15, { 1.7f, 0.4, 0.2 }, { 0.0f, 1.0f, 0.0f }));
	bodies.push_back(Body(50, 0.35, { -1.8f, -0.4, 0.8 }, { 1.0f, 0.0f, 0.0f }));
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
				ImGui::Text("Body %i", i);
				ImGui::BulletText("Pos X: %f", bodies[i].GetPosition().GetX());
				ImGui::BulletText("Pos Y: %f", bodies[i].GetPosition().GetY());
				ImGui::BulletText("Pos Z: %f", bodies[i].GetPosition().GetZ());
				ImGui::Separator();
			}
		}
		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::BulletText("Pos X: %f", gfx.CameraPos.GetX());
			ImGui::BulletText("Pos Y: %f", gfx.CameraPos.GetY());
			ImGui::BulletText("Pos Z: %f", gfx.CameraPos.GetZ());
		}
		ImGui::SliderFloat("Timestep", &dt, 0.001, 1);
		if (ImGui::Button("Step"))
		{
			for (auto itr = combinations.begin(); itr != combinations.end(); itr++)
			{ // for each combination of bodies, apply the force	
				std::pair<Body&, Body&> pair = *itr;
				pair.first.ApplyForce(pair.second, dt);
			}
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		gfx.EndFrame();
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}