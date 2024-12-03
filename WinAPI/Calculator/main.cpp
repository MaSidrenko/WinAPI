#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include"iostream"
//#include"map"

CONST CHAR g_sz_CLASS_NAME[] = "Calculator PV_319";

CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_INTERVAL = 5;
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_FONT_HEIGHT = 32;
CONST INT g_i_FONT_WIDTH = (g_i_FONT_HEIGHT * 2) / 5;
CONST INT g_i_DISPLAY_WIDTH = g_i_BUTTON_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_DISPLAY_HEIGHT = g_i_FONT_HEIGHT + 4;

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL;
CONST INT g_i_OPERATION_BUTTON_START_X = g_i_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3;
CONST INT g_i_OPERATION_BUTTON_START_Y = g_i_BUTTON_START_Y;
CONST INT g_i_CONTROL_BUTTON_START_X = g_i_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4;
CONST INT g_i_CONTROL_BUTTON_START_Y = g_i_BUTTON_START_Y;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_START_X * 2 + 16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y * 2 + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + 38;

CONST CHAR* g_OPERATIONS[] = { "+", "-", "*", "/" };

CONST COLORREF g_DISPLAY_BACKGROUND[] = { RGB(0, 0, 0), RGB(114, 114, 114) };
CONST COLORREF g_DISPLAY_FOREGROUND[] = { RGB(0, 255, 0), RGB(255, 255, 255) };

CONST COLORREF g_WINDOW_BACKGROUND[] = { RGB(0, 30, 150), RGB(50, 50, 50) };

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SetSkin(HWND hwnd, CONST CHAR* skin);
VOID SetSkinFromDLL(HWND hwnd, CONST CHAR* skin);
INT GetKey(HWND hwnd, WPARAM wParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	// Регистрация класса окна:
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	//wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	HBITMAP hBackGround = (HBITMAP)LoadImage(hInstance, "Picture\\CalcBMP.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	wClass.hbrBackground = CreatePatternBrush(hBackGround);

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_CLASS_NAME;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszMenuName = NULL;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed!", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	//Создание окна:

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);
	// Запуск Цикла Сообщений:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


//TODO: Кастомный шрифт
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DOUBLE a = DBL_MIN;
	static DOUBLE b = DBL_MIN;
	static INT operation = 0;
	static BOOL input = FALSE;
	static BOOL input_operation = FALSE;
	//--------------------------------//

	static INT color_index = 0;

	//--------------------------------//
	switch (uMsg)
	{
	case WM_CREATE:
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_RIGHT,
			10, 10,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT_DISPLAY,
			GetModuleHandle(NULL),
			NULL
		);

		AddFontResourceEx("Fonts\\digital-7.ttf", FR_PRIVATE, 0);
		HFONT hFont = CreateFont
		(
			g_i_FONT_HEIGHT, g_i_FONT_WIDTH,
			0,				//Escapement - Наклон шрифта в дестаках градусов
			0,				//Orientation - ???
			FW_BOLD,		//Weight - толщина
			FALSE,			//Italic - курсив
			FALSE,			//Underline - Подчеркнутый
			FALSE,			//Strikeout - Перечеркнутый
			ANSI_CHARSET,
			OUT_TT_PRECIS,
			CLIP_TT_ALWAYS,
			ANTIALIASED_QUALITY,
			FF_DONTCARE,
			"digital-7"
		);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

		//DONE:	Button Icons.
		CHAR sz_digit[2] = "0";
		CONST INT SIZE = 256;
		CHAR str_bmp[SIZE] = ".bmp";
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = 49 + i + j; //49 - ASCII - код единицы
				HWND hButton = CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
					g_i_BUTTON_START_X + j * (g_i_BUTTON_SIZE + g_i_INTERVAL),
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (2 - i / 3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1 + i + j),
					GetModuleHandle(NULL),
					NULL
				);
				/*CHAR str_path[SIZE] = "ButtonsBMP\\square_blue\\button_";
				HBITMAP hBMP_Num = (HBITMAP)LoadImage
				(
					GetModuleHandle(NULL),
					strcat(strcat(str_path, sz_digit), str_bmp),
					IMAGE_BITMAP,
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					LR_LOADFROMFILE
				);
				SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMP_Num);*/
			}
		}
		HWND hButton0 = CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_BUTTON_START_X,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		/*HBITMAP hBMP_0 = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			"ButtonsBMP\\square_blue\\button_0.bmp",
			IMAGE_BITMAP,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMP_0);*/
		HWND hButton_Dot = CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_BUTTON_START_X + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		/*HBITMAP hBMP_Dot = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			"ButtonsBMP\\square_blue\\button_dot.bmp",
			IMAGE_BITMAP,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton_Dot, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMP_Dot);*/
		for (int i = 0; i < 4; i++)
		{
			HWND hOperations = CreateWindowEx
			(
				NULL, "Button", g_OPERATIONS[i],
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
				g_i_OPERATION_BUTTON_START_X,
				g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
			/*		const std::map<int, CONST CHAR*> Operations =
					{
						{0, "Plus" },
						{1, "Minus" },
						{2, "Aster" },
						{3, "Slash" }
					};
					CHAR str_path[SIZE] = "ButtonsBMP\\square_blue\\button_";

					HBITMAP hBMP_Operations = (HBITMAP)LoadImage
					(
						GetModuleHandle(NULL),
						strcat(strcat(str_path, Operations.at(i)), str_bmp),
						IMAGE_BITMAP,
						g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
						LR_LOADFROMFILE
					);
					SendMessage(hOperations, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMP_Operations);*/
		}
		HWND hButton_BSP = CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_CONTROL_BUTTON_START_X,
			g_i_CONTROL_BUTTON_START_Y,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		/*HBITMAP hBMP_BSP = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			"ButtonsBMP\\square_blue\\button_bsp.bmp",
			IMAGE_BITMAP,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton_BSP, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMP_BSP);*/
		HWND hButton_CLR = CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_CONTROL_BUTTON_START_X,
			g_i_CONTROL_BUTTON_START_Y + g_i_BUTTON_SIZE + g_i_INTERVAL,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLR,
			GetModuleHandle(NULL),
			NULL
		);
		/*	HBITMAP hBMP_CLR = (HBITMAP)LoadImage
			(
				GetModuleHandle(NULL),
				"ButtonsBMP\\square_blue\\button_clr.bmp",
				IMAGE_BITMAP,
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				LR_LOADFROMFILE
			);
			SendMessage(hButton_CLR, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMP_CLR);*/
		HWND hButtons_Equal = CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_CONTROL_BUTTON_START_X,
			g_i_CONTROL_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 2,
			g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
		/*HBITMAP hBPM_Equal = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			"ButtonsBMP\\square_blue\\button_equal.bmp",
			IMAGE_BITMAP,
			g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButtons_Equal, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBPM_Equal);*/
		SetSkin(hwnd, "square_blue");
	}break;
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		HWND hEdit = (HWND)lParam;
		if (GetDlgCtrlID(hEdit) == IDC_EDIT_DISPLAY)
		{
			SetTextColor(hdc, g_DISPLAY_FOREGROUND[color_index]);
			SetBkColor(hdc, g_DISPLAY_BACKGROUND[color_index]);
			HBRUSH hbrBackGround = CreateSolidBrush(g_WINDOW_BACKGROUND[color_index]);
			SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)hbrBackGround);
			SendMessage(hwnd, WM_ERASEBKGND, wParam, 0);
			//ReleaseDC(hEdit, hdc);
			return (INT_PTR)hbrBackGround;
		}
	}break;
	case WM_COMMAND:
	{
		CONST INT SIZE = 256;
		CHAR sz_digit[2] = {};
		CHAR sz_display[SIZE]{};
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
		if (
			LOWORD(wParam) >= IDC_BUTTON_0 &&
			LOWORD(wParam) <= IDC_BUTTON_9
			)
		{
			//DONE: Если первый символ '.', то последующий ввод не правильный.
			if (!input && !input_operation)
			{
				SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
				ZeroMemory(sz_display, SIZE);
			}
			if (!input && input_operation /*&& !strchr(sz_display, '.')*/)
				sz_display[0] = 0;

			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			if (strlen(sz_display) == 1 && sz_display[0] == '0')
				sz_display[0] = sz_digit[0];
			else
				strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			//input_operation = FALSE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			if (strchr(sz_display, '.') && input)break;
			if (input_operation && a == atof(sz_display))strcpy(sz_display, "0.");
			else strcat(sz_display, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			strcat(sz_display, ".");
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			if (strlen(sz_display) == 1)
				sz_display[0] = '0';
			else
				sz_display[strlen(sz_display) - 1] = 0;
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = b = DBL_MIN;
			operation = 0;
			input = input_operation = FALSE;
			ZeroMemory(sz_display, SIZE);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS &&
			LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			//SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (a == DBL_MIN) a = atof(sz_display);
			//else b = atof(sz_display);
			input = FALSE;
			if (input_operation)SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			operation = LOWORD(wParam);
			input_operation = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			//if(b == DBL_MIN && !input) 
			if (input || b == DBL_MIN && !input)b = atof(sz_display);
			input = FALSE;
			switch (operation)
			{
			case IDC_BUTTON_PLUS:  a += b; break;
			case IDC_BUTTON_MINUS: a -= b; break;
			case IDC_BUTTON_ASTER: a *= b; break;
			case IDC_BUTTON_SLASH: a /= b; break;
			}
			input_operation = FALSE;
			//input = FALSE;
			if (a == DBL_MIN)strcpy(sz_display, "0");
			else sprintf(sz_display, "%g", a);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		SetFocus(hwnd);
	}break;
	case WM_KEYDOWN:
	{
		SendMessage(GetDlgItem(hwnd, GetKey(hwnd, wParam)), BM_SETSTATE, TRUE, 0);
	}break;
	case WM_KEYUP:
	{
		SendMessage(GetDlgItem(hwnd, GetKey(hwnd, wParam)), BM_SETSTATE, FALSE, 0);
		SendMessage(hwnd, WM_COMMAND, GetKey(hwnd, wParam), 0);
	}break;
	case WM_CONTEXTMENU:
	{
		HMENU hSubMenuSkins = CreatePopupMenu();
		InsertMenu(hSubMenuSkins, 0, MF_BYPOSITION | MF_STRING, IDR_METAL_MISTRAL, "Metal Mistral");
		InsertMenu(hSubMenuSkins, 0, MF_BYPOSITION | MF_STRING, IDR_SQUARE_BLUE, "Square Blue");

		HMENU hMenu = CreatePopupMenu();
		InsertMenu(hMenu, 0, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hSubMenuSkins, "Skins");
		InsertMenu(hMenu, 1, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
		InsertMenu(hMenu, 2, MF_BYPOSITION | MF_STRING, IDR_EXIT, "Exit");
		BOOL skin_index = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD, LOWORD(lParam), HIWORD(lParam), 0, hwnd, 0);
		switch (skin_index)
		{
		case IDR_SQUARE_BLUE:	SetSkinFromDLL(hwnd, "square_blue"); break;
		case IDR_METAL_MISTRAL:	SetSkinFromDLL(hwnd, "metal_mistral"); break;
		case IDR_EXIT:			DestroyWindow(hwnd);
		}

		DestroyMenu(hSubMenuSkins);
		DestroyMenu(hMenu);

		if (skin_index >= IDR_SQUARE_BLUE && skin_index <= IDR_METAL_MISTRAL)
		{
			color_index = skin_index - IDR_CONTEX_MENU - 1;
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			HDC hdcDisplay = GetDC(hEditDisplay);
			SendMessage(hwnd, WM_CTLCOLOREDIT, WPARAM(hdcDisplay), LPARAM(hEditDisplay));

			ReleaseDC(hEditDisplay, hdcDisplay);
			CHAR sz_buffer[MAX_PATH]{};
			SendMessage(hEditDisplay, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
	}break;
	case WM_DESTROY:
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		HDC hdc = GetDC(hEdit);
		ReleaseDC(hEdit, hdc);
		PostQuitMessage(0);
	}break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

CONST CHAR* g_BUTTON_FILENAME[] =
{
	"button_0",
	"button_1",
	"button_2",
	"button_3",
	"button_4",
	"button_5",
	"button_6",
	"button_7",
	"button_8",
	"button_9",
	"button_point",
	"button_plus",
	"button_minus",
	"button_aster",
	"button_slash",
	"button_bsp",
	"button_clr",
	"button_equal"
};

VOID SetSkin(HWND hwnd, CONST CHAR* skin)
{
	//CHAR sz_path[MAX_PATH]{};
	//CHAR sz_file[FILENAME_MAX]{};
	CHAR sz_file_name[MAX_PATH]{};
	for (int i = 0; i < 18; i++)
	{
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_0 + i);
		sprintf(sz_file_name, "ButtonsBMP\\%s\\%s.bmp", skin, g_BUTTON_FILENAME[i]);
		HBITMAP ButtonBMP = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			sz_file_name,
			IMAGE_BITMAP,
			i + IDC_BUTTON_0 == IDC_BUTTON_0 ? g_i_BUTTON_DOUBLE_SIZE : g_i_BUTTON_SIZE,
			i + IDC_BUTTON_0 == IDC_BUTTON_EQUAL ? g_i_BUTTON_DOUBLE_SIZE : g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		std::cout << sz_file_name << std::endl;
		SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)ButtonBMP);
	}
}
INT GetKey(HWND hwnd, WPARAM wParam)
{
	if (GetKeyState(VK_SHIFT) < 0)
	{
		if (wParam == 0x38)
		{
			return IDC_BUTTON_ASTER;
		}
	}
	else if (wParam >= '0' && wParam <= '9')
		return wParam - 0x30 + IDC_BUTTON_0;
	else if (wParam >= 0x60 && wParam <= 0x69)

		return wParam - 0x60 + IDC_BUTTON_0;

	switch (wParam)
	{
	case VK_DECIMAL:
	case VK_OEM_PERIOD:
		return LOWORD(IDC_BUTTON_POINT);
		break;
	case VK_ESCAPE:
		return LOWORD(IDC_BUTTON_CLR);
		break;
	case VK_BACK:
		return LOWORD(IDC_BUTTON_BSP);
		break;
	case VK_OEM_PLUS:
	case VK_ADD:
		return LOWORD(IDC_BUTTON_PLUS);
		break;
	case VK_OEM_MINUS:
	case VK_SUBTRACT:
		return LOWORD(IDC_BUTTON_MINUS);
		break;
	case VK_OEM_2:
	case VK_DIVIDE:
		return LOWORD(IDC_BUTTON_SLASH);
		break;
	case VK_RETURN:
		return LOWORD(IDC_BUTTON_EQUAL);
		break;
	case VK_MULTIPLY:
		return LOWORD(IDC_BUTTON_ASTER);
		break;
	}
}

VOID SetSkinFromDLL(HWND hwnd, CONST CHAR* skin)
{
	CHAR file_name[MAX_PATH]{};
	sprintf(file_name, "ButtonsBMP\\%s", skin);
	HMODULE hInst = LoadLibrary(file_name);
	for (int i = IDC_BUTTON_0; i <= IDC_BUTTON_EQUAL; i++)
	{
		HBITMAP buttonBMP = (HBITMAP)LoadImage
		(
			hInst,
			MAKEINTRESOURCE(i),
			IMAGE_BITMAP,
			i > IDC_BUTTON_0		? g_i_BUTTON_SIZE : g_i_BUTTON_DOUBLE_SIZE,
			i < IDC_BUTTON_EQUAL	? g_i_BUTTON_SIZE : g_i_BUTTON_DOUBLE_SIZE,
			NULL
		);
		SendMessage(GetDlgItem(hwnd, i), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)buttonBMP);
	}
	FreeLibrary(hInst);
}