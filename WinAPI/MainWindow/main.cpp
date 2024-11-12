#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include"resource.h"

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "My Window";	//Имя класса окна

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hIntance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//Регистрация окна:
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	//Размер?:
	wc.style = 0;
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	//Внешиний вид:
	//wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	wc.hIcon = (HICON)LoadImage(hIntance, "Peace_ico.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hIntance, "weather_ico.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	

	wc.hCursor = (HCURSOR)LoadImage(hIntance, 
		"Cursors\\starcraft-original\\Working In Background.ani",
		IMAGE_CURSOR, LR_DEFAULTSIZE,LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hCursor = LoadCursor(hIntance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	//Тех. Составляющия?:
	wc.hInstance = hIntance;
	wc.lpszClassName = NULL;
	wc.lpszClassName = g_sz_MY_WINDOW_CLASS;

	wc.lpfnWndProc = (WNDPROC)WndProc;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	//Создание окна:

	INT screen_Width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_Height = GetSystemMetrics(SM_CYSCREEN);

	INT Window_Width = screen_Width * 0.75;
	INT Window_Height = screen_Height * 0.75;

	INT Window_Start_X = screen_Width / 8;
	INT Window_Start_Y = screen_Height / 8;

	HWND hwnd = CreateWindowEx
	(
		NULL,								  //ExStyles
		g_sz_MY_WINDOW_CLASS,				  //Class name
		g_sz_MY_WINDOW_CLASS,				  //Window title
		WS_OVERLAPPEDWINDOW,				  //Window stule
		Window_Start_X,Window_Start_Y,		  //Window position
		Window_Width,Window_Height,			  //Window size
		NULL,								  //Paretn Window
		NULL,								  //Main menu resourceID for MainWindow or ResourceID for ChileWindow
		hIntance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow); // Задает режим отображения окна (Развернуто на весь экран, 
																 //свернуто в окно, свернуто в панель задач)
	UpdateWindow(hwnd);			//Прорисовывает окно

	//Запуск цикла сообщений:
	MSG msg; // Создаем сообщение
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.message;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_MOVE:
	case WM_SIZE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		INT window_width = rect.right - rect.left;
		INT window_height = rect.bottom - rect.top;

		CONST INT SIZE = 256;
		CHAR sz_title[SIZE]{};
		sprintf(sz_title, 
			"%s - Pos: %ix%i;Size: %ix%i",
			g_sz_MY_WINDOW_CLASS ,
			rect.left, rect.top, 
			window_width, window_height);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	}
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}