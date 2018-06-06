#include <iostream>

using namespace std;

struct Vector {

    float x, y, z;

    Vector(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
};

struct Ray {

    Vector *origin, *direction;

    Ray(Vector *_origin, Vector *_direction): origin(_origin), direction(_direction) {}
};

struct Facet {

    Vector *v[3];

    Facet(Vector *a, Vector *b, Vector *c) {
        v[0] = a;
        v[1] = b;
        v[2] = c;
    }
};
