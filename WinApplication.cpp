#ifndef UNICODE
#define UNICODE
#endif

// fstream -> width, heiht, grid_color(first, second, third)...

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <vector>

struct grid_color {
	INT first = 255;
	INT second = 0;
	INT third = 0;
};

struct app {
public:
	LONG height = 320;
	LONG width = 240;
};

// number == 0 - nothing.
// number == 1 - circle.
// number == 2 - cross.
struct Matrix {
	INT x;
	INT x1;
	INT y;
	INT y1;
	INT number;
};

struct Click {
	INT X = 0;
	INT Y = 0;
};

const INT N = 2;
WNDCLASS wc = { };
HBRUSH hBrush;
HPEN hPen;
HWND hwnd;
app app_info;
Click click;
grid_color grid;
BOOL Flag = TRUE;

std::vector<std::vector<Matrix>> matrix(N + 1, std::vector<Matrix>(N + 1));

VOID update_app_info(LONG height, LONG width) {
	app_info.height = height;
	app_info.width = width;
}

VOID update_matrix_info() {
	for (INT i = 0; i < N + 1; ++i) {
		for (INT j = 0; j < N + 1; ++j) {
			matrix[i][j].x = j * app_info.width / (N + 1);
			matrix[i][j].y = i * app_info.height / (N + 1);
			matrix[i][j].x1 = (j + 1) * app_info.width / (N + 1);
			matrix[i][j].y1 = (i + 1) * app_info.height / (N + 1);
		}
	}
}

CONST wchar_t CLASS_NAME[] = L"Tic-tac Toe";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Runs Notepad. 
VOID RunNotepad(VOID)
{
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&sInfo, sizeof(STARTUPINFO));

	puts("Starting Notepad...");
	CreateProcess(_T("C:\\Windows\\Notepad.exe"),
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}

// Create the mesh (Default color - red).
VOID CreateGrid(HWND hwnd, HDC hdc)
{ 
	hPen = CreatePen(PS_SOLID, 2, RGB(grid.first, grid.second, grid.third));
	SelectObject(hdc, hPen);
	for (double x = (double)app_info.width / (N + 1); x < app_info.width; x += (double)app_info.width / (N + 1))
	{
		MoveToEx(hdc, x, 0, NULL);
		LineTo(hdc, x, app_info.height);
	}
	for (double y = (double)app_info.height / (N + 1); y < app_info.height; y += (double)app_info.height / (N + 1))
	{
		MoveToEx(hdc, 0, y, NULL);
		LineTo(hdc, app_info.width, y);
	}
	DeleteObject(hPen);
}

VOID CreateCircle(HWND hwnd, INT i, INT j, HDC hdc) {
	hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Arc(hdc, matrix[i][j].x, matrix[i][j].y, matrix[i][j].x1, matrix[i][j].y1, 0, 0, 0, 0);			
	DeleteObject(hPen);
}

VOID CreateCross(HWND hwnd, INT i, INT j, HDC hdc) {
	hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	SelectObject(hdc, hPen);
	MoveToEx(hdc, matrix[i][j].x, matrix[i][j].y, NULL);
	LineTo(hdc, matrix[i][j].x1, matrix[i][j].y1);
	MoveToEx(hdc, matrix[i][j].x1, matrix[i][j].y, NULL);
	LineTo(hdc, matrix[i][j].x, matrix[i][j].y1);
	DeleteObject(hPen);
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	// Create the background.
	hBrush = CreateSolidBrush(RGB(0, 0, 255));
	wc.hbrBackground = hBrush;

	RegisterClass(&wc);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Tic-Tac Toe",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		app_info.height,
		app_info.width,
		HWND_DESKTOP,
		NULL,
		hInstance,
		NULL
	); 

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (uMsg)
	{

	// Left button of mouse.
	case WM_LBUTTONDOWN: {
		click.X = GET_X_LPARAM(lParam);
		click.Y = GET_Y_LPARAM(lParam);
		int i = 0, j = 0;
		for (DOUBLE x = (DOUBLE)app_info.width / (N + 1); x < app_info.width + 15; x += (double)app_info.width / (N + 1), ++j) {
			if (click.X < x) {
				for (DOUBLE y = (DOUBLE)app_info.height / (N + 1); y < app_info.height + 15; y += (DOUBLE)app_info.height / (N + 1), ++i) {
					if (click.Y < y) {
						matrix[i][j].number = 1;
						break;
					}
				}
			}
			else {
				continue;
			}
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
	}
	break;

	// Right button of mouse.
	case WM_RBUTTONUP: {
		click.X = GET_X_LPARAM(lParam);
		click.Y = GET_Y_LPARAM(lParam);
		int i = 0, j = 0;
		for (DOUBLE x = (DOUBLE)app_info.width / (N + 1); x < app_info.width + 15; x += (double)app_info.width / (N + 1), ++j) {
			if (click.X < x) {
				for (DOUBLE y = (DOUBLE)app_info.height / (N + 1); y < app_info.height + 15; y += (DOUBLE)app_info.height / (N + 1), ++i) {
					if (click.Y < y) {
						matrix[i][j].number = 2;
						break;
					}
				}
			}
			else {
				continue;
			}
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
	}
	break;

	// Mousewheel
	case WM_MOUSEWHEEL: {
		INT mousewhell_direction = GET_WHEEL_DELTA_WPARAM(wParam);
		if (mousewhell_direction != 0) {
			if (!Flag) {
				if (grid.first == 255) {
					if (grid.second == 255) {
						if (grid.third == 255) {
							Flag = TRUE;
						}
						else { grid.third += 5; }
					}
					else { grid.second += 5; }
				}
				else { grid.first += 5;  }
			}
			else {
				if (grid.first == 0) {
					if (grid.second == 0) {
						if (grid.third == 0) {
							Flag = FALSE;
						}
						else { grid.third -= 5; }
					}
					else { grid.second -= 5; }
				}
				else { grid.first -= 5; }
			}
		}
		else {
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
	}
	break;

	// Create Hot Keys.
	case WM_CREATE: {
		RegisterHotKey(hwnd, 0, MOD_SHIFT, 'S'); // 0 - SHIFT + S -  Run Notepad.
		RegisterHotKey(hwnd, 1, MOD_CONTROL, 'Q'); // 1 - CTRL + Q - CloseWindow.
	}
	break;

	// Used Hot Keys.
	case WM_HOTKEY: {

		if (wParam == 0) {
			RunNotepad();
		}

		if (wParam == 1) {
			DestroyWindow(hwnd);
		}
	}
	break;

	// Destroy the window and finish the programm.
	case WM_DESTROY:
		UnregisterHotKey(hwnd, 0);
		UnregisterHotKey(hwnd, 0);
		PostQuitMessage(0);
		return 0;
	
	// Paint Grid, Croses and Circles.
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		CreateGrid(hwnd, hdc);
		for (INT i = 0; i < N + 1; ++i) {
			for (INT j = 0; j < N + 1; ++j) {
				if (matrix[i][j].number == 1) {
					CreateCircle(hwnd, i, j, hdc);
				}
				else if (matrix[i][j].number == 2) {
					CreateCross(hwnd, i, j, hdc);
				}
			}
		}
		EndPaint(hwnd, &ps);
		DeleteObject(hdc);
	}
	break;

	// Resize the window and update the data about height and width.
	case WM_SIZE: {
		LONG width = LOWORD(lParam);
		LONG height = HIWORD(lParam);
		update_app_info(height, width);
		update_matrix_info();
		InvalidateRect(hwnd, NULL, TRUE);
	}
	break;

	// HotKey - only one button.
	case WM_KEYDOWN: {
		switch (wParam) {
		
		// Max value RGB - 255.
		case 13: { // id = 13 - 'ENTER'
			hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			SetClassLongPtrA(hwnd, GCLP_HBRBACKGROUND, (LONG)hBrush);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

		case 27: { // id = 27 - 'ESC'
			DestroyWindow(hwnd);
		}
		break;

		}
	}
	break;

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}