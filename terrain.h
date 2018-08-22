//
// Created by giuseppe on 19/08/18.
//

#ifndef PROJGRAF_TERRAIN_H
#define PROJGRAF_TERRAIN_H

#include "point3.h"
#include "mesh.h"
#include "perlinnoise.h"


class Terrain {

private:
    const float SIZE = 3, VERTEX_COUNT = 90, xOffset = -150;
    std::vector<Vertex> grid; // vettore di vertici
    std::vector<Face> faces;   // vettore di facce

    PerlinNoise perlinNoise = PerlinNoise();

    float maxHeight = 15;

    void generateTerrain();

    float mapRange(float x, float a, float b, float c, float d);

    void vertexColor(float y);

    Mesh *terrain;
public:

    Terrain();

    void render();

    virtual ~Terrain();

};


#endif //PROJGRAF_TERRAIN_H
