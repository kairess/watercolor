#ifndef TOOLBOX_H
#define TOOLBOX_H

//#ifdef DEBUG_H
#include "Debug.h"
//#endif

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
using namespace cv;
using namespace std;

//*************************************
//�ر�ע��
//1.Scalar�� / �������������, Ҫ���Լ���д��div(),
//2.Mat�� * / Ϊ�������,Ҫ��Mat::mul(),Mat::div()
//***************************************

struct myPoint
{
	int x, y;
	myPoint(int x = 0, int y = 0) : x(x), y(y){};
	void print();
	int dis(myPoint &a);
};

namespace ArraySpace
{
const int xo[] = {1, -1, 0, 0};
const int yo[] = {0, 0, 1, -1};

//�½���ά���鲢��ʼ��
template <typename T>
T **newArray(int n, int m)
{
	T **a = new T *[n];
	for (int i = 0; i < n; i++)
		a[i] = new T[m]();
	return a;
}

//ɾ����ά����
template <typename T>
void deleteArray(T **a, int n, int m)
{
	for (int i = n - 1; i >= 0; i--)
		delete[] a[i];
	delete[] a;
}

//��pλ��[startX,startY] --- (startX+lenX , startX+lenY)��,���ϱ�,���¿�
int inMap(myPoint p, int lenX, int lenY, int startX = 0, int startY = 0);
//����Mat��Χ�ľ�����
int inMap(Mat &a, myPoint p);
int inMap(Mat &a, int x, int y);

} // namespace ArraySpace

//-----����Random--------------
//����[0,x-1]
int rand(int x);
//����[a,b]
int rand(int a, int b);
//----------------------------

//��ͼdouble����,maxVal(255->1)
void ImageToDouble(const Mat &input, Mat &output);

//�ڰ�ͼ��תdouble����,maxVal(255->1)
void ArrayToDouble(const Mat &input, Mat &output);

//double����ת�ڰ�ͼ,maxVal(1->255)
void DoubleToImage(const Mat &input, Mat &output);

//double����ת��ͼ,maxVal(1->255)
void DoubleToArray(const Mat &input, Mat &output);

//�����һ��
void DoubleNomolization(Mat &a);

////ͼ��ľ�ֵ
//Scalar getMean(Mat &a);
//
////ͼ���׼��
//Scalar getStdDev(Mat &a);

//a��ÿһ��ͨ���ֱ����b��Ӧ��ֵ
Mat mul(const Mat &a, const Scalar &b);

//mul
inline Scalar mul(const Scalar &a, const Scalar &b)
{
	return Scalar(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
}

//div
inline Scalar divVec(const Scalar &a, const Scalar &b)
{
	return Scalar(a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]);
}
inline Scalar divVec(const Scalar &a, float x)
{
	return Scalar(a[0] / x, a[1] / x, a[2] / x, a[3] / x);
}

inline bool operator<(const Vec3b &a, const Vec3b &b)
{
	return a[0] < b[0] && a[1] < b[1] && a[2] < b[2];
}
inline bool operator>(const Vec3b &a, const Vec3b &b)
{
	//return true;
	//Debug() <<AllInt<< a[0] << " " << a[1] << " " << a[2];
	return a[0] > b[0] && a[1] > b[1] && a[2] > b[2];
}

inline Vec3b operator-(const Vec3b &a, const Vec3b &b)
{
	return Vec3b(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
inline Vec3b operator+(const Vec3b &a, const Vec3b &b)
{
	return Vec3b(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
inline Vec3b operator/(const Vec3b &a, const Vec3b &b)
{
	return Vec3b(a[0] / b[0], a[1] / b[1], a[2] / b[2]);
}
inline Vec3b operator/(const Vec3b &a, double x)
{
	return Vec3b(a[0] / x, a[1] / x, a[2] / x);
}
inline Vec3b abs(const Vec3b &a)
{
	return Vec3b(abs(a[0]), abs(a[1]), abs(a[2]));
}
Vec3b mean(const Vec3b &a, const Vec3b &b);

//sqr
float sqr(float x);
int sqr(int x);

//sqrt
inline Scalar sqrt(const Scalar &a);

//ͼ��ĳһ�������ֵ(��Mat::at()��)  eg. dataAt<Vec3b>(img,3,5)
template <typename T>
T &dataAt(cv::Mat &src, int i, int j)
{
	return src.at<T>(i, j);
	T *curRow = src.ptr<T>(i);
	return *(curRow + j * src.channels());
}

//ĳ���mean
template <typename T>
Scalar getMeanPoint(Mat &src, int x, int y, int kernel = 3)
{
	int count = 0;
	Scalar sum = Scalar::all(0);
	for (int i = x - kernel; i <= x + kernel; i++)
	{
		for (int j = y - kernel; j <= y + kernel; j++)
			if (ArraySpace::inMap(src, myPoint(i, j)))
			{
				count++;
				sum = sum + Scalar(dataAt<T>(src, i, j));
			}
	}
	return divVec(sum, (float)count);
}

//------------------------------------------------------FOR Debug----------------------------------------------------

//�ڿ���̨��ӡMat��(startX,startY)Ϊ���,����Ϊ(lenX,lenY)��Χ������ֵ
template <typename T>
void printMat(Mat &a, int lenX = 10, int lenY = 10, int startX = 0, int startY = 0)
{
	DebugStatus temp = Debug().getStatus();
	Debug().setStatus(StdOut);
	Debug() << "Mat(" << a.rows << "," << a.cols << "):";
	Debug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
	for (int i = startX; i < startX + lenX; i++)
	{
		Debug() << "(" << NoEnter;
		for (int j = startY; j < startY + lenY; j++)
		{
			Debug() << int(dataAt<T>(a, i, j)) << NoEnter;
			if (j != startY + lenY - 1)
				Debug() << ", " << NoEnter;
			//cout<<a.at<T>(i, j) << " ";
		}
		Debug() << ")";
	}
	Debug() << "";
	Debug().setStatus(temp);
}

//�ڿ���̨��ӡMat��(startX,startY)Ϊ���,����Ϊ(lenX,lenY)��Χ������ֵ,����1λС��
template <typename T>
void printMatF(Mat &a, int lenX = 10, int lenY = 10, int startX = 0, int startY = 0, int digit = 1)
{
	DebugStatus temp = Debug().getStatus();
	Debug().setStatus(StdOut);
	Debug() << "Mat(" << a.rows << "," << a.cols << "):";
	Debug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
	for (int i = startX; i < startX + lenX; i++)
	{
		Debug() << "(" << NoEnter;
		for (int j = startY; j < startY + lenY; j++)
		{
			Debug() << float(dataAt<T>(a, i, j)) << NoEnter;
			if (j != startY + lenY - 1)
				Debug() << ", " << NoEnter;
		}
		Debug() << ")";
	}
	Debug() << "";
	Debug().setStatus(temp);
}

//���ļ���ӡMat��(startX,startY)Ϊ���,����Ϊ(lenX,lenY)��Χ������ֵ,Ĭ�ϴ�ӡ����Mat
template <typename T>
void writeMat(Mat &a, int lenX = 0, int lenY = 0, int startX = 0, int startY = 0, string fileName = "debug.txt")
{
	DebugStatus temp = Debug().getStatus();
	Debug().setStatus(FileOut);
	if (lenX == 0 && lenY == 0)
	{
		lenX = 50; // a.rows;
		lenY = 50; // a.cols;
	}
	Debug() << "Mat(" << a.rows << "," << a.cols << "):";
	Debug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
	for (int i = startX; i < startX + lenX; i++)
	{
		Debug() << "(" << NoEnter;
		for (int j = startY; j < startY + lenY; j++)
		{
			Debug() << int(dataAt<T>(a, i, j)) << NoEnter;
			if (j != startY + lenY - 1)
				Debug() << ", " << NoEnter;
			//cout<<a.at<T>(i, j) << " ";
		}
		Debug() << ")";
	}
	Debug() << "";
	Debug().setStatus(temp);
}

//���ļ���ӡMat��(startX,startY)Ϊ���,����Ϊ(lenX,lenY)��Χ������ֵ,Ĭ�ϴ�ӡ����Mat,����һλС��
#include <fstream>
template <typename T>
void writeMatF(Mat &a, int lenX = 0, int lenY = 0, int startX = 0, int startY = 0, string fileName = "debug.txt", int digit = 1)
{
	DebugStatus temp = Debug().getStatus();
	Debug().setStatus(FileOut);
	if (lenX == 0 && lenY == 0)
	{
		lenX = 50; // a.rows;
		lenY = 50; // a.cols;
	}
	Debug() << "Mat(" << a.rows << "," << a.cols << "):";
	Debug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
	for (int i = startX; i < startX + lenX; i++)
	{
		Debug() << "(" << NoEnter;
		for (int j = startY; j < startY + lenY; j++)
		{
			Debug() << float(dataAt<T>(a, i, j)) << NoEnter;
			if (j != startY + lenY - 1)
				Debug() << ", " << NoEnter;
		}
		Debug() << ")";
	}
	Debug() << "";
	Debug().setStatus(temp);
}

//----------------------------------------------------END FOR Debug---------------------------------------------------

#endif
