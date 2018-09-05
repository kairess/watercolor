#include "Geometry.h"

//double�Ŀ��Ա����������ֱ�long long(int64)ҪС�ܶ�,����double ���������Ч����,���Ҳ����㱬(ֻ�ǻ���ʧ����),����1.23*10^100 double�ǿ���װ�µ�
const double EPS = 1e-6;
const double MAXV = 1e100;
const double PI = acos(-1);
//�ȽϺ���
int Geometry::dcmp(const double &a, double b)
{
	if (a - b > EPS)
		return 1;
	if (b - a > EPS)
		return -1;
	return 0;
}
//�������뵽kλ,��Ϊֱ����ϵͳ�Դ������ʱ������-0.00013����2λ���-0.00�����
double Geometry::round(double x, int k)
{
	for (int i = 1; i <= k; i++)
		x *= 10;
	x = floor(x + 0.5);
	for (int i = 1; i <= k; i++)
		x /= 10;
	return x;
}

/*****************��******************/

//     ----|------>|---
//	   1   4   3   5 2    0
int Geometry::Line::position(const Point &P)
{
	if (dir == Point(0, 0))
		return 0;
	Point dir2(P - A);
	if (dir2 == Point(0, 0))
		return 4;
	if (dcmp(det(dir, dir2)) != 0)
		return 0;
	if (dcmp(dot(dir, dir2)) < 0)
		return 1;
	if (dcmp(dir2.len(), dir.len()) < 0)
		return 3;
	if (dcmp(dir2.len(), dir.len()) > 0)
		return 2;
	return 5;
}

bool Geometry::intersectLine(const Line &la, const Line &lb, Point &P)
{
	Point A(la.A), A1(la.A + la.dir),
			B(lb.A), B1(lb.A + lb.dir);
	double s1 = det(B - A, B1 - A), s2 = det(B - A1, B1 - A1), s = s1 - s2;
	if (dcmp(s) == 0)
		return false;
	P = A + la.dir * s1 / s;
	return true;
}
