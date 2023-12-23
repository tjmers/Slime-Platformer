// compilation command: g++ tests/keyboard_vk_codes.cpp -o tests/keyboard_input_tests.exe

#include <iostream>

#include <Windows.h>


LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    constexpr wchar_t CLASS_NAME[] = L"mainwin";
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

    constexpr int SCREEN_WIDTH = 600, SCREEN_HEIGHT = 400;
    RECT client_region = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&client_region, WS_CAPTION | WS_SYSMENU | WS_BORDER, FALSE, 0);

	HWND hWnd = CreateWindowExW(
		0,
		CLASS_NAME,
		L"Keyboard input Virtual Key Codes shower",
		WS_CAPTION | WS_SYSMENU | WS_BORDER,
		75, 25, client_region.right - client_region.left, client_region.bottom - client_region.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);


    ShowWindow(hWnd, SW_SHOW);
    MSG msg{ 0 };
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT)
    {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    }

    return static_cast<int>(msg.wParam);
}



LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case WM_KEYDOWN:
        std::cout << wParam << '\n';
        break;
	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

    return 0;
}