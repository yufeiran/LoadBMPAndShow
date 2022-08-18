#include"LoadBMPAndShow.h"


bool readPPM(const string filepath)
{
	ifstream is(filepath,ios::in);
	cout << "readPPM :" << filepath << endl;
	if (is.is_open() == false)return false;

	string type;
	is >> type;
	if (type == "p3") {
		is >> picWidth >> picHeight;

		int colorMax;
		is >> colorMax;
		if (colorMax != 255)return false;
		pBmpBuf=new unsigned char[picWidth * picHeight*3];
		for (int i = picHeight-1; i >=0 ; --i) {
			for (int j = 0; j < picWidth; ++j) {
				int nowPos = i * picWidth + j;
				int nowData;
				is >> nowData;
				//cout << nowData << " ";
				pBmpBuf[nowPos * 3] =static_cast<unsigned char>(nowData);
				is >> nowData;
				//cout << nowData << " ";
				pBmpBuf[nowPos * 3 + 1] = static_cast<unsigned char>(nowData);
				is >> nowData;
				//cout << nowData << endl;
				pBmpBuf[nowPos * 3 + 2] = static_cast<unsigned char>(nowData);

			}
		}
		return true;
	}
	else {
		return false;
	}


}


bool readFromCmd(const string filepath)
{
	if ((filepath.find("ppm") != string::npos)|( filepath.find("txt") != string::npos)) {
		if (readPPM(filepath))
		{
			cout << "readOK!" << endl;
			cout << "\nwidth=" << picWidth << "\nheight=" << picHeight << endl;
			return true;
		}
		else
		{
			cout << "file no found!" << endl;
			return false;
		}
	}

	if (readBmp(filepath.c_str()))
	{
		cout << "readOK!" << endl;
		cout << "\nwidth=" << picWidth << "\nheight=" << picHeight << endl;
		return true;
	}
	else
	{
		cout << "file no found!" << endl;
		return false;
	}
}

bool read()
{
	char readPath[999];
	cout << "Enter file name:";
	cin >> readPath;

	if (readFromCmd(readPath)) {
		return true;
	}
	else {
		return false;
	}
}



BYTE* buffer;

HDC screen_hdc;
HDC hCompatibleDC;
HBITMAP hCompatibleBitmap;
HBITMAP hOldBitmap;
BITMAPINFO binfo;

HINSTANCE hInstance;
WNDCLASS Draw;
HWND hwnd;
MSG msg;

bool isGameOver = false;


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
		isGameOver = true;
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
	SetDIBits(screen_hdc, hCompatibleBitmap, 0, picHeight, buffer, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
	BitBlt(screen_hdc, -1, -1, picWidth, picHeight, hCompatibleDC, 0, 0, SRCCOPY);
}

int main(int argc,char* argv[])
{
	if (argc == 2) {
		if (readFromCmd(argv[1]) == false)
		{
			return -1;
		}
	}
	else {
		if (read()==false)
		{
			return -1;
		}
	}



	buffer = (BYTE*)malloc(sizeof(BYTE) * picWidth * picHeight * bits / 8);
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
		picWidth + 15,
		picHeight + 38,
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
	binfo.bmiHeader.biHeight = -picHeight;
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biSizeImage = 0;
	binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biWidth = picWidth;

	screen_hdc = GetDC(hwnd);
	hCompatibleDC = CreateCompatibleDC(screen_hdc);
	hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, picWidth, picHeight);
	hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);

	while (isGameOver==false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		GameLoop();
	}
	return 0;

}

void GameLoop()
{
	CleanScreen();
	for(int y=0;y<picHeight;y++)
		for (int x = 0; x < picWidth; x++)
		{
			Color color = Color(pBmpBuf[y * 3 * picWidth + (x+1) * 3 - 1], pBmpBuf[y * 3 * picWidth + (x + 1) * 3 - 2], pBmpBuf[y * 3 * picWidth + (x + 1) * 3-3]);
			DrawPoint(x,picHeight- y, color);
		}

	PutBufferToScreen();
}