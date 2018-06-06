#include <stdio.h>
#include <fstream>
#include <vector>
#include "../geometry/Geometry.cpp"

struct Camera {

    Vector rotV;
    Ray pos;

    int width, height;

    float FOP;

    Camera(int width, int _height, Ray _pos, Vector _rotV, float _FOP);

    Ray pixel_ray(int x, int y);

};


class Render {

private:

    int fn, pn;
    Camera cam;

    Facet **facets;
    Vector **points;

    char *data;
    int size;
    ofstream *out;

    long tm;

    void parse_obj(const char* file_name);

    template <class T> vector<T> strsplit(string input, char delimiter);

    void write();

    void draw(int x, int y, float r, float g, float b);

public:

    Render(const char *result);

    void render(const char *file_name);
};
