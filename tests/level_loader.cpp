// compilation command: g++ tests/level_loader.cpp main/graphics.cpp creator/level_editor.cpp main/object.cpp objects/invisible_boundry.cpp objects/spike.cpp objects/wooden_floor.cpp main/collidable.cpp main/vec2.cpp -ld2d1 -lWindowsCodecs -lole32 -o tests/level_loader.exe

#include <chrono>

#include <Windows.h>

#include "../main/graphics.h"
#include "../creator/level_editor.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Graphics graphics;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	constexpr LPCWSTR CLASS_NAME = L"mainwin";

	using namespace std::chrono_literals;
	constexpr int targetFPS = 60;
    constexpr float ONE_SIXTYITH = 1.0f / 60;
	constexpr std::chrono::nanoseconds timestep(1000ms / targetFPS);

	WNDCLASSEXW window{ 0 };
	window.hInstance = hInstance;
	window.lpszClassName = CLASS_NAME;
	window.lpfnWndProc = WndProc;
	window.cbSize = sizeof(window);
	window.style = CS_OWNDC;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hIcon = nullptr;
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.lpszMenuName = nullptr;
	window.hIconSm = nullptr;
	RegisterClassExW(&window);

	RECT client_region = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&client_region, WS_CAPTION | WS_SYSMENU | WS_BORDER, FALSE, 0);

	HWND hWnd = CreateWindowExW(
		0,
		CLASS_NAME,
		L"Platformer",
		WS_CAPTION | WS_SYSMENU | WS_BORDER,
		75, 25, client_region.right - client_region.left, client_region.bottom - client_region.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);
	

	if (FAILED(graphics.CreateGraphicsResources(hWnd)))
	{
		return -1;
	}

	if (FAILED(LevelEditor::init_resources(graphics)))
		return -1;

    LevelEditor level_editor;
	ShowWindow(hWnd, SW_SHOW);

	using clock = std::chrono::high_resolution_clock;
	std::chrono::nanoseconds lag(0ns);
	auto timeStart = clock::now();


	MSG msg{ 0 };
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT) {
		auto currentTime = clock::now();
		auto delta_time = currentTime - timeStart;
		timeStart = currentTime;
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			while (lag >= timestep) {
				lag -= timestep;
				level_editor.update();
			}

			graphics.BeginDraw();
			graphics.ClearScreen(0.5f, 0.0f, 0.0f);
			level_editor.draw(graphics);
			graphics.EndDraw();
		}
	}

	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}