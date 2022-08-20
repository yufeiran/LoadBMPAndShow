#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include<windows.h>
#include<iostream>
#include<tchar.h>
#include<fstream>

using namespace std;


unsigned char *pBmpBuf;//����ͼ�����ݵ�ָ��
int picWidth;//ͼ��Ŀ�
int picHeight;//ͼ��ĸ�
RGBQUAD *pColorTable;//��ɫ��ָ��
int biBitCount;//ͼ�����ͣ�ÿ����λ��



//��ʾλͼ�ļ�ͷ��Ϣ   
void showBmpHead(BITMAPFILEHEADER pBmpHead) {
	cout << "\nλͼ�ļ�ͷ:" << endl;
	cout << "�ļ���С:" << pBmpHead.bfSize << endl;
	cout << "������_1:" << pBmpHead.bfReserved1 << endl;
	cout << "������_2:" << pBmpHead.bfReserved2 << endl;
	cout << "ʵ��λͼ���ݵ�ƫ���ֽ���:" << pBmpHead.bfOffBits << endl << endl;
}
//��ʾλͼ��Ϣͷ��Ϣ  
void showBmpInforHead(BITMAPINFOHEADER pBmpInforHead) {
	cout << "\nλͼ��Ϣͷ:" << endl;
	cout << "�ṹ��ĳ���:" << pBmpInforHead.biSize << endl;
	cout << "λͼ��:" << pBmpInforHead.biWidth << endl;
	cout << "λͼ��:" << pBmpInforHead.biHeight << endl;
	cout << "biPlanesƽ����:" << pBmpInforHead.biPlanes << endl;
	cout << "biBitCount������ɫλ��:" << pBmpInforHead.biBitCount << endl;
	cout << "ѹ����ʽ:" << pBmpInforHead.biCompression << endl;
	cout << "biSizeImageʵ��λͼ����ռ�õ��ֽ���:" << pBmpInforHead.biSizeImage << endl;
	cout << "X����ֱ���:" << pBmpInforHead.biXPelsPerMeter << endl;
	cout << "Y����ֱ���:" << pBmpInforHead.biYPelsPerMeter << endl;
	cout << "ʹ�õ���ɫ��:" << pBmpInforHead.biClrUsed << endl;
	cout << "��Ҫ��ɫ��:" << pBmpInforHead.biClrImportant << endl;
}

//-----------------------------------------------------------------------------------------
//����һ��ͼ��λͼ���ݡ����ߡ���ɫ��ָ�뼰ÿ������ռ��λ������Ϣ,����д��ָ���ļ���
bool readBmp(const char *bmpName)
{
	ifstream is(bmpName);
	//FILE *fp = fopen(bmpName, "rb");//�����ƶ���ʽ��ָ����ͼ���ļ�
	//if (fp == 0)
		//return 0;
	if (is.is_open() == false)return false;

	//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER
	is.seekg(sizeof(BITMAPFILEHEADER), ios::beg);
	//fseek(fp, sizeof(BITMAPFILEHEADER), 0);
	/*
	BITMAPFILEHEADER filehead;
	fread(&filehead, 1, sizeof(BITMAPFILEHEADER), fp);
	showBmpHead(filehead);//��ʾ�ļ�ͷ
*/

//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��
	BITMAPINFOHEADER infohead;
	is.read((char*)&infohead, sizeof(BITMAPINFOHEADER));
	//fread(&infohead, sizeof(BITMAPINFOHEADER), 1, fp); //��ȡͼ����ߡ�ÿ������ռλ������Ϣ
	picWidth = infohead.biWidth;
	picHeight = infohead.biHeight;
	biBitCount = infohead.biBitCount;//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����
	//showBmpInforHead(infohead);//��ʾ��Ϣͷ 


	//int lineByte = (picWidth * biBitCount / 8 + 3) / 4 * 4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256
	int lineByte = picWidth * 3;
	//if (biBitCount == 8)
	//{
	//	//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�
	//	pColorTable = new RGBQUAD[256];
	//	fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	//}

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
	pBmpBuf = new unsigned char[lineByte * picHeight];
	//fread(pBmpBuf, 1, lineByte * picHeight, fp);
	is.read((char*)pBmpBuf, lineByte * picHeight);
	//fclose(fp);//�ر��ļ�
	return 1;//��ȡ�ļ��ɹ�
}





//������ʾ�����ȫ�ֱ���
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