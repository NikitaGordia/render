#include <iostream>
#include <cmath>

using namespace std;

template <typename T> inline T sqr(T t) { return t * t; }

const float EPS = 0.0001;

struct Vector {

    float x, y, z;

    Vector() {}
    Vector(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}

    Vector substract(Vector &v) {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    Vector multi(float sz) {
        return Vector(x * sz, y * sz, z * sz);
    }

    Vector add(Vector &v) {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector cross(Vector &v) {
        return Vector(
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x);
    }

    float dot(Vector &v) {
        return x * v.x + y * v.y + z * v.z;
    }

    float dist(Vector &v) { return sqrt(sqr(x - v.x) + sqr(y - v.y) + sqr(z - v.z)); }

    float size() { return sqrtf(sqr(x) + sqr(y) + sqr(z)); }

    Vector normalize() {
        float l = size();
        return Vector(x /= l, y /= l, z /= l);
    }

    float angle(Vector &v) {
        return acosf(dot(v) / (size() * v.size()));
    }

    string dbg() {
        string res = " ( ";
        res += to_string(x); res += "; ";
        res += to_string(y); res += "; ";
        res += to_string(z); res += ") ";
        return res;
    }
};

struct Ray {

    Vector origin, direction;

    Ray() {}
    Ray(Vector _origin, Vector _direction): origin(_origin), direction(_direction) {}

    string dbg() {
        return (origin.dbg() + " " + direction.dbg());
    }
};

struct InterRes {

    bool i = false;
    Vector p;
    Vector normal;

    operator bool() { return i; }

    InterRes() {}
    InterRes(Vector _p, Vector _normal): p(_p), normal(_normal) {}
};

struct Facet {

    Vector *v[3];

    Facet(Vector *a, Vector *b, Vector *c) {
        v[0] = a;
        v[1] = b;
        v[2] = c;
    }

    bool pointInTriangle(Vector x, Vector a, Vector b, Vector c) {
        return squareOfTriangle(a, b, c) == squareOfTriangle(x, a, b) +
                                            squareOfTriangle(x, b, c) +
                                            squareOfTriangle(x, a, c);
    }

    float squareOfTriangle(Vector a, Vector b, Vector c) {
        Vector x = b.substract(a), y = c.substract(a);
        float ans = abs(x.cross(y).size()) / 2;
        return ans;
    }

    InterRes intersect(Ray &r) {

        Vector normal((v[1]->y - v[0]->y) * (v[2]->z - v[0]->z) - (v[2]->y - v[0]->y) * (v[1]->z - v[0]->z),
                      (v[2]->x - v[0]->x) * (v[1]->z - v[0]->z) - (v[1]->x - v[0]->x) * (v[2]->z - v[0]->z),
                      (v[1]->x - v[0]->x) * (v[2]->y - v[0]->y) - (v[1]->y - v[0]->y) * (v[2]->x - v[0]->x));

        float
                t =
                -(normal.x * r.origin.x + normal.y * r.origin.y + normal.z * r.origin.z + v[0]->x * ((v[2]->y - v[0]->y) * (v[1]->z - v[0]->z) - (v[1]->y - v[0]->y) * (v[2]->z - v[0]->z)) +
                                                                     v[0]->y * ((v[1]->x - v[0]->x) * (v[2]->z - v[0]->z) - (v[2]->x - v[0]->x) * (v[1]->z - v[0]->z)) +
                                                                     v[0]->z * ((v[1]->y - v[0]->y) * (v[2]->x - v[0]->x) - (v[1]->x - v[0]->x) * (v[2]->y - v[0]->y))) /
                (normal.x * r.direction.x + normal.y * r.direction.y + normal.z * r.direction.z);

        normal = normal.normalize();

        Vector res(t * r.direction.x + r.origin.x, t * r.direction.y + r.origin.y, t * r.direction.z + r.origin.z);
        Vector edge1 = v[1]->substract(*v[0]),
                edge2 = v[2]->substract(*v[0]),
                h = r.direction.cross(edge2);
        t = edge1.dot(h);
        if (t > -EPS && t < EPS)
            return {res, normal};
        float f = 1 / t;
        Vector s = r.origin.substract(*v[0]);
        float u = f * s.dot(h);
        if (u < 0.0 || u > 1.0)
            return {res, normal};
        Vector q = s.cross(edge1);
        float v = f * r.direction.dot(q);

        InterRes ans(res, normal);

        if (v < 0.0 || u + v > 1.0) return ans;
        float tt = f * edge2.dot(q);
        ans.i = (tt > EPS);
        return ans;
    }
};