#include"LoadBMPAndShow.h"

bool read()
{
	char readPath[999];
	cout << "Enter file name:";
	cin >> readPath;

	if (readBmp(readPath))
	{
		cout << "readOK!" << endl;
		cout << "\nwidth=" << bmpWidth << "\nheight=" << bmpHeight << endl;
		return true;
	}
	else
	{
		cout << "file no found!" << endl;
		return false;
	}
}


BYTE buffer[SCREEN_WIDTH*SCREEN_HEIGHT*bits / 8];

HDC screen_hdc;
HDC hCompatibleDC;
HBITMAP hCompatibleBitmap;
HBITMAP hOldBitmap;
BITMAPINFO binfo;

HINSTANCE hInstance;
WNDCLASS Draw;
HWND hwnd;
MSG msg;


LRESULT CALLBACK WindowProc(
	_In_	HWND hwnd,
	_In_	UINT uMsg,
	_In_	WPARAM wParam,
	_In_	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
			exit(0);
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void PutBufferToScreen()
{
	SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, buffer, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
	BitBlt(screen_hdc, -1, -1, SCREEN_WIDTH, SCREEN_HEIGHT, hCompatibleDC, 0, 0, SRCCOPY);
}

int main()
{
	if (read())
	{
		hInstance = GetModuleHandle(NULL);

		Draw.cbClsExtra = 0;
		Draw.cbWndExtra = 0;
		Draw.hCursor = LoadCursor(hInstance, IDC_ARROW);
		Draw.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		Draw.lpszMenuName = NULL;
		Draw.style = WS_MINIMIZEBOX | CS_HREDRAW | CS_VREDRAW;
		Draw.hbrBackground = (HBRUSH)COLOR_WINDOW;
		Draw.lpfnWndProc = WindowProc;
		Draw.lpszClassName = _T("DDraw");
		Draw.hInstance = hInstance;

		RegisterClass(&Draw);

		hwnd = CreateWindow(
			_T("DDraw"),
			L"Draw",
			WS_OVERLAPPEDWINDOW,
			38,
			20,
			SCREEN_WIDTH + 15,
			SCREEN_HEIGHT + 38,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		//init bitbuffer
		ZeroMemory(&binfo, sizeof(BITMAPINFO));
		binfo.bmiHeader.biBitCount = bits;
		binfo.bmiHeader.biCompression = BI_RGB;
		binfo.bmiHeader.biHeight = -SCREEN_HEIGHT;
		binfo.bmiHeader.biPlanes = 1;
		binfo.bmiHeader.biSizeImage = 0;
		binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		binfo.bmiHeader.biWidth = SCREEN_WIDTH;

		screen_hdc = GetDC(hwnd);
		hCompatibleDC = CreateCompatibleDC(screen_hdc);
		hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
		hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);

		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			GameLoop();
		}
	}
	else
	{
		cin.get();
		cin.get();
	}
	return 0;

}

void GameLoop()
{
	CleanScreen();
	for(int y=0;y<bmpHeight;y++)
		for (int x = 0; x < bmpWidth; x++)
		{
			Color color = Color(pBmpBuf[y * 3 * bmpWidth + x * 3 + 2], pBmpBuf[y * 3 * bmpWidth + x * 3 + 1], pBmpBuf[y * 3 * bmpWidth + x * 3]);
			DrawPoint(x,bmpHeight- y, color);
		}

	PutBufferToScreen();
}