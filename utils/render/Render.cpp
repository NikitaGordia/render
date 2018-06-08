#include "Render.h"

Camera::Camera(int _width, int _height, Ray _pos, Vector _light, Vector _rotV, float _FOP): width(_width), height(_height), pos(_pos), light(_light), rotV(_rotV), FOP(_FOP) {}

Ray Camera::pixel_ray(int x, int y) {
    Vector dV = rotV.normalize().multi((x - width / 2) * FOP);
    Vector dH = rotV.cross(pos.direction).normalize().multi((y - height / 2) * FOP);

    dV = pos.direction.add(dV);
    dH = dV.add(dH);

    return Ray(pos.origin, dH);
}


void Render::parse_obj(const char *file_name) {
    ifstream file(file_name);

    string str;
    vector<Vector> v;
    vector< pair<int, pair<int, int> > > f;
    vector<string> pts, coords;

    cout<<"Parsing : "<<endl;
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
        cout<<"    Error open : "<<file_name<<" !"<<endl;
        return;
    }

    pn = v.size();
    points = new Vector*[pn];
    for (int i = 0; i < pn; i++) points[i] = new Vector(v[i].x, v[i].y, v[i].z);

    fn = f.size();
    facets = new Facet*[fn];
    for (int i = 0; i < fn; i++) facets[i] = new Facet(points[f[i].first], points[f[i].second.first], points[f[i].second.second]);

    cout<<"    Vertex : "<<pn<<endl<<"    Facets : "<<fn<<endl<<endl;
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

Render::Render(const char *result): cam(512, 512, Ray(Vector(80, 48, 32), Vector(-8.2, -4.92, -3.25)), Vector(-1, -2, -3), Vector(0, 0, 1), 0.0008) {
    tm = clock();
    out = new ofstream(result, ios::binary);

    // file header
    unsigned short colorBit = 3;
    size = cam.width * cam.height * colorBit;
    unsigned int offset = 27;
    unsigned int shit = 12;
    unsigned short filehead[4] = { (unsigned short)cam.height, (unsigned short)cam.width, 1, (unsigned short)(colorBit * 8)};

    (*out) << "BM";
    out->write((char *)&size, sizeof(size));
    out->seekp(4, ios::cur);
    out->write((char *)&offset, sizeof(offset));
    out->write((char *)&shit, sizeof(shit));
    out->write((char *)&filehead, 12);
    out->seekp(offset, ios::beg);

    data = new char[size];
}

void Render::draw(int x, int y, float r, float g, float b) {
    *(data + (x * cam.width + y) * 3 + 2) = (char)(r);
    *(data + (x * cam.width + y) * 3 + 1) = (char)(g);
    *(data + (x * cam.width + y) * 3 + 0) = (char)(b);
}

void Render::write() {
    out->write(data, sizeof(char) * size);
    out->close();
    long end = clock();
    cout<<"Rendering over in : "<<((end - tm) / 1000000)<<"s "<<((end - tm) % 1000000) / 1000<<"ms"<<endl;
}

InterRes Render::ray_trace(Ray r) {
    float mnDist = INFINITY;
    float dist;
    InterRes ans, res;

    for (int k = 0; k < fn; k++) {
        res = facets[k]->intersect(r);

        dist = cam.pos.origin.dist(res.p);
        if (res && mnDist > dist) {
            mnDist = dist;
            ans = res;
        }
    }
    ans.i = (mnDist != INFINITY);
    return ans;
}

void Render::render(const char *file_name) {

    parse_obj(file_name);

    float shade;
    float dist, mnDist;
    Vector pt(-1, -1, -1);
    InterRes hit, light;
    Ray r;

    for (int i = 0; i < cam.height; i++)
        for (int j = 0; j < cam.width; j++) {

            r = cam.pixel_ray(i, j);
            hit = ray_trace(r);
            if (hit) {
                r = Ray(cam.light, hit.p.substract(cam.light));
                light = ray_trace(r);
                if (abs(light.p.dist(r.origin) - hit.p.dist(r.origin)) < EPS) {
                    draw(i, j, 255 * (1 - sinf(r.direction.angle(hit.normal))), 0, 0);
                } else draw(i, j, 0, 0, 0);
            } else draw(i, j, 255, 255, 255);
        }

    write();
}
