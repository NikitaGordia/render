#include <iostream>
#include "utils/render/Render.h"

int main() {
    Render r("/Users/nikitagordia/Documents/Workspace/CLionProjects/render/src/imgs/result.bmp");

    r.render("/Users/nikitagordia/Documents/Workspace/CLionProjects/render/src/objects/sphere.obj");
}