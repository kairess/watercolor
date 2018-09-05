#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <algorithm>
#define PolygonN 15
using namespace std;

namespace Geometry
{
//double�Ŀ��Ա����������ֱ�long long(int64)ҪС�ܶ�,����double ���������Ч����,���Ҳ����㱬(ֻ�ǻ���ʧ����),����1.23*10^100 double�ǿ���װ�µ�
const double EPS = 1e-6;
const double MAXV = 1e100;
const double PI = std::acos(-1);
//�ȽϺ���
int dcmp(const double &a, double b = 0);
//�������뵽kλ,��Ϊֱ����ϵͳ�Դ������ʱ������-0.00013����2λ���-0.00�����
double round(double x, int k);
inline double sqr(double a) { return a * a; }

//��,����
struct Point
{
	double x, y;
	Point(double x = 0, double y = 0) : x(x), y(y){};
	double len() const { return sqrt(sqr(x) + sqr(y)); }
	double dis(const Point &A) const { return sqrt(sqr(A.x - x) + sqr(A.y - y)); }
};

inline double det(const Point &A, const Point &B) { return A.x * B.y - A.y * B.x; }
inline double dot(const Point &A, const Point &B) { return A.x * B.x + A.y * B.y; } //Atention not '-'!
inline Point operator+(const Point &A, const Point &B) { return Point(A.x + B.x, A.y + B.y); }
inline Point operator-(const Point &A, const Point &B) { return Point(A.x - B.x, A.y - B.y); }
inline Point operator*(const double &k, const Point &A) { return Point(A.x * k, A.y * k); }
inline Point operator*(const Point &A, const double &k) { return Point(A.x * k, A.y * k); }
inline Point operator/(const Point &A, const double &k)
{
	if (!k)
		return Point(MAXV, MAXV);
	return Point(A.x / k, A.y / k);
}
inline bool operator==(const Point &A, const Point &B) { return dcmp(A.x, B.x) == 0 && dcmp(A.y, B.y) == 0; }
inline bool operator!=(const Point &A, const Point &B) { return dcmp(A.x, B.x) != 0 || dcmp(A.y, B.y) != 0; }

/*****************��******************/

struct Line
{
	Point A, dir;
	Line(Point A = Point(0, 0), Point dir = Point(0, 0)) : A(A), dir(dir){};
	int position(const Point &P);
	double disSeg(const Point &P);
	double disLine(const Point &P);
	//void read(){A.read();dir.read();dir=dir-A;}
};

bool intersectLine(const Line &la, const Line &lb, Point &P);

/**************�����****************/

//��yΪ��һ�ؼ���,xΪ�ڶ��ؼ�������
inline bool cmpY(const Point &A, const Point &B) { return A.y < B.y || (A.y == B.y && A.x < B.x); }
struct Polygon
{
	int n;
	Point a[PolygonN];
	double diameter();
	double wide();
	double MinimumAreaBoundingRectangle();
	double MinimumCircumBoundingRectangle();
	double MaximumInscribedTriangle();
	double dis(const Point &B);
	void core();
	Polygon narrow(const double &x);
	//��͹��
	void convex()
	{
		sort(a + 1, a + n + 1, cmpY);
		static Point d[PolygonN];
		int p1 = 0;
		for (int i = 1; i <= n; i++)
		{
			while (p1 > 1 && dcmp(det(d[p1] - d[p1 - 1], a[i] - d[p1])) <= 0)
				p1--;
			d[++p1] = a[i];
		}
		int p2 = p1;
		for (int i = n; i >= 1; i--)
		{
			while (p2 > p1 && dcmp(det(d[p2] - d[p2 - 1], a[i] - d[p2])) <= 0)
				p2--;
			d[++p2] = a[i];
		}
		n = p2 - 1;
		for (int i = 1; i <= n; i++)
			a[i] = d[i];
	}
	//�����
	double getArea()
	{
		double area = 0;
		a[n + 1] = a[1];
		for (int i = 1; i <= n; i++)
			area += (a[i].x - a[i + 1].x) * (a[i].y + a[i + 1].y);
		return fabs(area) * 0.5;
	}
	//���ܳ�
	double getCircum()
	{
		double cir = 0;
		a[n + 1] = a[1];
		for (int i = 1; i <= n; i++)
			cir += a[i].dis(a[i + 1]);
		return cir;
	}
	//������
	Point getGravity()
	{
		Point G;
		double S = 0, tot = 0;
		a[n + 1] = a[1];
		for (int i = 1; i <= n; i++)
		{
			S = det(a[i], a[i + 1]);
			G = G + S * (a[i] + a[i + 1]) / 3;
			tot += S;
		}
		G = G / tot;
		return G;
	}
	//�жϵ��Ƿ��ڶ������
	bool pointIn(const Point &P)
	{
		a[n + 1] = a[1];
		for (int i = 1; i <= n; i++)
			if (dcmp(dot(a[i] - P, a[i + 1] - P)) <= 0 && dcmp(det(a[i] - P, a[i + 1] - P)) == 0)
				return true;
		int tot = 0;
		for (int i = 1; i <= n; i++)
		{
			int k1 = dcmp(a[i].x - P.x);
			int k2 = dcmp(a[i + 1].x - P.x);
			int k3 = dcmp(det(a[i + 1] - a[i], P - a[i]));
			if (k1 <= 0 && k2 > 0 && k3 > 0)
				tot++;
			if (k2 <= 0 && k1 > 0 && k3 < 0)
				tot++;
		}
		return tot & 1;
	}
};

} // namespace Geometry

#endif