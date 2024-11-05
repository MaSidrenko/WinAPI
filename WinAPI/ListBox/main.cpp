#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include"Resource.h"

CONST CHAR* g_VALUES[] = {"This", "is", "a", "List", "Box"};

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstacne, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstacne, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case DOUBLE_CLICK:
	{
		/*HWND hList = GetDlgItem(hwnd, IDC_LIST);
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE]{};
		SendMessage(hList, , 0, (LPARAM)sz_buffer);*/
	}
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		HWND hList = GetDlgItem(hwnd, IDC_LIST);
		for (int i = 0; i < sizeof(g_VALUES) / sizeof(g_VALUES[0]); i++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_VALUES[i]);
		}
		SendMessage(hList, LB_SETCURSEL, 0, 0);
	}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);

			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
			CHAR sz_message[SIZE]{};
			sprintf(sz_message, "�� ������� ����� �%i �� ��������� \"%s\".", i, sz_buffer);

			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCADD:
		{
			HWND hEditBox = GetDlgItem(hwnd, IDC_ADD_ELM);
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage(hEditBox, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDCDELETE:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_DELETESTRING, i, 0);
		}
		break;
		case IDCANCEL: EndDialog(hwnd, 0); break;

		}
	}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}