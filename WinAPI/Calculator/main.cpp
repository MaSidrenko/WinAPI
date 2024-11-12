#include<Windows.h>

#define IDC_BUTTON_1 1

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "Calculator";

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.style = 0;
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wc.hInstance = hInstance;
	wc.lpszClassName = NULL;
	wc.lpszClassName = g_sz_MY_WINDOW_CLASS;

	wc.lpfnWndProc = (WNDPROC)WndProc;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	INT screen_Width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_Height = GetSystemMetrics(SM_CYSCREEN);

	INT Window_Width = screen_Width * 0.15;
	INT Window_Height = screen_Height * 0.35;

	INT Window_Start_X = screen_Width / 8;
	INT Window_Start_Y = screen_Height / 8;

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_MY_WINDOW_CLASS,
		g_sz_MY_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		Window_Start_X, Window_Start_Y,
		Window_Width, Window_Height,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	

	ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.message;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndButton_1;
	static HWND TextBox;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		TextBox = CreateWindow
		(
			"EDIT",
			"",
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_RIGHT,
			10, 10,
			250, 20,
			hwnd,
			NULL,
			NULL,
			NULL
		);
		hwndButton_1 = CreateWindow
		(
			"BUTTON",
			"1",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			10, 280,
			50, 50,
			hwnd,
			(HMENU)IDC_BUTTON_1,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL
			);
	}break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case 1:
			{
				CONST INT SIZE = 256;
				CHAR sz_buffer[SIZE]{};
				GetWindowText(hwndButton_1, sz_buffer, SIZE);
				SetWindowText(TextBox, sz_buffer);
			}
			break;
		}
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}