#include<Windows.h>

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
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
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
	HWND hwnd = CreateWindowEx
	(
		NULL,				  //ExStyles
		g_sz_MY_WINDOW_CLASS, //Class name
		g_sz_MY_WINDOW_CLASS, //Window title
		WS_OVERLAPPEDWINDOW,  //Window stule
		CW_USEDEFAULT,		  //Window position
		CW_USEDEFAULT,		  //Window position
		CW_USEDEFAULT,		  //Window size
		CW_USEDEFAULT,		  //Window size
		NULL,				  //Paretn Window
		NULL,				  //Main menu resourceID for MainWindow or ResourceID for ChileWindow
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