#include "pch.h"

#include "Vec3.h"

#define WIDTH 600
#define HEIGHT 600

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
	switch (uMsg)
	{
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

	HWND hWnd = CreateWindowEx(NULL, L"N-Body-Class", L"N-Body Simulation", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		200, 200, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

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
	}
}