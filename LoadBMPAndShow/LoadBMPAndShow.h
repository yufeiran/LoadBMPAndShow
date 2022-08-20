#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include<windows.h>
#include<iostream>
#include<tchar.h>
#include<fstream>

using namespace std;


unsigned char *pBmpBuf;//读入图像数据的指针
int picWidth;//图像的宽
int picHeight;//图像的高
RGBQUAD *pColorTable;//颜色表指针
int biBitCount;//图像类型，每像素位数



//显示位图文件头信息   
void showBmpHead(BITMAPFILEHEADER pBmpHead) {
	cout << "\n位图文件头:" << endl;
	cout << "文件大小:" << pBmpHead.bfSize << endl;
	cout << "保留字_1:" << pBmpHead.bfReserved1 << endl;
	cout << "保留字_2:" << pBmpHead.bfReserved2 << endl;
	cout << "实际位图数据的偏移字节数:" << pBmpHead.bfOffBits << endl << endl;
}
//显示位图信息头信息  
void showBmpInforHead(BITMAPINFOHEADER pBmpInforHead) {
	cout << "\n位图信息头:" << endl;
	cout << "结构体的长度:" << pBmpInforHead.biSize << endl;
	cout << "位图宽:" << pBmpInforHead.biWidth << endl;
	cout << "位图高:" << pBmpInforHead.biHeight << endl;
	cout << "biPlanes平面数:" << pBmpInforHead.biPlanes << endl;
	cout << "biBitCount采用颜色位数:" << pBmpInforHead.biBitCount << endl;
	cout << "压缩方式:" << pBmpInforHead.biCompression << endl;
	cout << "biSizeImage实际位图数据占用的字节数:" << pBmpInforHead.biSizeImage << endl;
	cout << "X方向分辨率:" << pBmpInforHead.biXPelsPerMeter << endl;
	cout << "Y方向分辨率:" << pBmpInforHead.biYPelsPerMeter << endl;
	cout << "使用的颜色数:" << pBmpInforHead.biClrUsed << endl;
	cout << "重要颜色数:" << pBmpInforHead.biClrImportant << endl;
}

//-----------------------------------------------------------------------------------------
//给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息,将其写到指定文件中
bool readBmp(const char *bmpName)
{
	ifstream is(bmpName);
	//FILE *fp = fopen(bmpName, "rb");//二进制读方式打开指定的图像文件
	//if (fp == 0)
		//return 0;
	if (is.is_open() == false)return false;

	//跳过位图文件头结构BITMAPFILEHEADER
	is.seekg(sizeof(BITMAPFILEHEADER), ios::beg);
	//fseek(fp, sizeof(BITMAPFILEHEADER), 0);
	/*
	BITMAPFILEHEADER filehead;
	fread(&filehead, 1, sizeof(BITMAPFILEHEADER), fp);
	showBmpHead(filehead);//显示文件头
*/

//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
	BITMAPINFOHEADER infohead;
	is.read((char*)&infohead, sizeof(BITMAPINFOHEADER));
	//fread(&infohead, sizeof(BITMAPINFOHEADER), 1, fp); //获取图像宽、高、每像素所占位数等信息
	picWidth = infohead.biWidth;
	picHeight = infohead.biHeight;
	biBitCount = infohead.biBitCount;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
	//showBmpInforHead(infohead);//显示信息头 


	//int lineByte = (picWidth * biBitCount / 8 + 3) / 4 * 4;//灰度图像有颜色表，且颜色表表项为256
	int lineByte = picWidth * 3;
	//if (biBitCount == 8)
	//{
	//	//申请颜色表所需要的空间，读颜色表进内存
	//	pColorTable = new RGBQUAD[256];
	//	fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	//}

	//申请位图数据所需要的空间，读位图数据进内存
	pBmpBuf = new unsigned char[lineByte * picHeight];
	//fread(pBmpBuf, 1, lineByte * picHeight, fp);
	is.read((char*)pBmpBuf, lineByte * picHeight);
	//fclose(fp);//关闭文件
	return 1;//读取文件成功
}





//用于显示窗体的全局变量
//----------------------------------

const int bits = 24;
extern BYTE* buffer;




class Color
{
public:
	Color(int R, int G, int B) :r(R), g(G), b(B) {};
	Color() :r(255), g(255), b(255) {};
	int r, g, b;
};


void CleanScreen();

void DrawPoint(int x, int y, Color color = Color(255, 255, 255));

void PutBufferToScreen();

void DrawPoint(int x, int y, const Color color)
{
	if (x < 0 || x >= picWidth)return;
	if (y < 0 || y >= picHeight)return;

	buffer[int(y) * picWidth * 3 + (int(x) + 1) * 3 - 3] = color.r;
	buffer[int(y) * picWidth * 3 + (int(x) + 1) * 3 - 2] = color.g;
	buffer[int(y) * picWidth * 3 + (int(x) + 1) * 3 - 1] = color.b;
}


void CleanScreen()
{
	for (int y = 0; y < picHeight; y++)
	{
		for (int x = 0; x < picWidth; x++)
		{

			buffer[int(y) * picWidth * 3 + (int(x) + 1) * 3 - 1] = 0;
			buffer[int(y) * picWidth * 3 + (int(x) + 1) * 3 - 2] = 0;
			buffer[int(y) * picWidth * 3 + (int(x) + 1) * 3 - 3] = 0;
		}
	}
}


void GameLoop();