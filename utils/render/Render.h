#include <stdio.h>
#include <fstream>
#include <vector>
#include "../geometry/Geometry.cpp"

struct Camera {

    Vector *rotV;
    Ray *pos;

    int width, height;

    float FOV;

    Camera(int width, int _height, Ray *_pos, Vector *_rotV, float _FOW);

};


class Render {

private:

    int fn, pn;

    Facet **facets;
    Vector **points;

    void parse_obj(const char* file_name);

    template <class T> vector<T> strsplit(string input, char delimiter);

public:

    void render(const char *file_name, const char *result);
};
