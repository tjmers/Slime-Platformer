// compilation command: g++ creator/platformer_creator.cpp creator/level_editor.cpp main/input.cpp main/vec2.cpp objects/wooden_floor.cpp main/object.cpp objects/invisible_boundry.cpp .\objects\spike.cpp main/graphics.cpp main/collidable.cpp  -ld2d1 -lWindowsCodecs -lole32 -o level_creator.exe

#include <chrono>
#include <stdexcept>
#include <iostream>

#include <Windows.h>

#include "../main/graphics.h"
#include "../main/input.h"
#include "level_editor.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Graphics graphics;

LevelEditor make_level_editor(int new_level);

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
		std::cerr << "Graphics initialization failed\n";
		return -1;
	}
	if (FAILED(LevelEditor::init_resources(graphics)))
	{
		std::cerr << "Level Editor resource initialization failed\n";
		return -1;
	}

	
	Input::Reset();

	int should_make_new_level;

	do {
		std::cout << "Make a new level? [y/n]: ";
		should_make_new_level = getchar();
		if (should_make_new_level >= static_cast<int>('A') && should_make_new_level <= static_cast<int>('Z'))
			should_make_new_level += 32;
	} while (should_make_new_level != static_cast<int>('y') && should_make_new_level != static_cast<int>('n'));

	LevelEditor level_editor = make_level_editor(should_make_new_level);



	ShowWindow(hWnd, SW_SHOW);

	using clock = std::chrono::high_resolution_clock;
	std::chrono::nanoseconds lag(0ns);
	auto timeStart = clock::now();

    float frame_delta_time = 0.0f;


	MSG msg{ 0 };
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT) {
		auto currentTime = clock::now();
		auto delta_time = currentTime - timeStart;
		timeStart = currentTime;
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);
        frame_delta_time += delta_time.count();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			while (lag >= timestep) {
				lag -= timestep;
                constexpr static int ONE_BILLION = 1e9;
				level_editor.update();
                frame_delta_time = 0;
				Input::Update();
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

	case WM_KILLFOCUS:
		Input::Reset();
		break;
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
		Input::HandleKeyboardInput(wParam, (lParam & 0x80000000) == 0);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		Input::HandleMouseDown({*reinterpret_cast<short*>(&lParam), *(reinterpret_cast<short*>(&lParam) + 1)}, wParam & MK_LBUTTON);
		break;

	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}


LevelEditor make_level_editor(int new_level)
{
	if (new_level == static_cast<int>('y'))
		return LevelEditor();

	else if (new_level == static_cast<int>('n'))
	{
		std::string file_path;
		std::cout << "Enter file path: ";
		std::cin >> file_path;
		return LevelEditor(file_path);
	}
	throw std::invalid_argument("new_level shuld be y or n");
}