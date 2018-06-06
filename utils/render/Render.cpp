#include "Render.h"

Camera::Camera(int _width, int _height, Ray *_pos, Vector *_rotV, float _FOW): width(_width), height(_height), pos(_pos), rotV(_rotV), FOW(_FOW) {}

void Render::parse_obj(const char *file_name) {
    ifstream file(file_name);

    string str;
    vector<Vector> v;
    vector< pair<int, pair<int, int> > > f;
    vector<string> pts, coords;

    if (file.is_open()) {
        while (file.good()) {
            getline(file, str);
            switch (str[0]) {
                case 'v': {
                    if (str[1] == ' ') {
                        coords.clear();
                        coords = strsplit<string>(str, ' ');
                        v.push_back(Vector(Vector(stof(coords[1]), stof(coords[2]), stof(coords[3]))));
                    }
                    break;
                }
                case 'f': {
                    pts.clear();
                    pts = strsplit<string>(str, ' ');
                    f.push_back( {stoi(strsplit<string>(pts[1], '/')[0]) - 1, { stoi(strsplit<string>(pts[2], '/')[0]) - 1, stoi(strsplit<string>(pts[3], '/')[0]) - 1 } } );
                    break;
                }
                default:
                    break;
            }
        }
    } else {
        cout<<"Error open : "<<file_name<<" !"<<endl;
    }

    pn = v.size();
    points = new Vector*[pn];
    for (int i = 0; i < pn; i++) points[i] = new Vector(v[i].x, v[i].y, v[i].z);

    fn = f.size();
    facets = new Facet*[fn];
    for (int i = 0; i < fn; i++) facets[i] = new Facet(points[f[i].first], points[f[i].second.first], points[f[i].second.second]);
}

template<class T> vector<T> Render::strsplit(string input, char delimiter) {
    vector<T> res;
    string sub;
    size_t pos = input.find_first_of(delimiter);
    while (pos < input.length() && pos > 0)
    {
        sub = input.substr(0, pos);
        input = input.substr(pos + 1);
        pos = input.find_first_of(' ');
        res.push_back(static_cast<T>(sub));
    }
    res.push_back(static_cast<T>(input.substr(0, pos)));
    return res;
}

void Render::render(const char *file_name, const char *result) {

    parse_obj(file_name);


}