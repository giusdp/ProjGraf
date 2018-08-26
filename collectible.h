#ifndef PROJGRAF_PERSON_H
#define PROJGRAF_PERSON_H

#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glu.h>
#include <utility>

#endif

#include "mesh.h"

#define V_VBO 0;
#define T_VBO 1;
class Collectible
{
  public:
    Mesh *model;
    Texture *texture;
    GLuint vaoID;

    float px, py, pz;

    Collectible()
    {
        std::vector<Vertex> vertices;
        std::vector<Face> faces;

        Vertex p1 = Vertex(), p2 = Vertex(), p3 = Vertex(), p4 = Vertex();
        p1.p = {-1, -1, 0}, p2.p = {-1, 1, 0},
        p3.p = {1, 1, 0}, p4.p = {1, -1, 0};
        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
        vertices.push_back(p4);

        Face face1 = Face(&p1, &p2, &p3);
        Face face2 = Face(&p1, &p4, &p3);
        faces.push_back(face1); faces.push_back(face2);

        model = new Mesh(vertices, faces);
        px = 0;
        py = 5;
        pz = 0;
        texture = new Texture();
        texture->loadTexture2D((char *)"Assets/personTexture.png");
    }

    void update()
    {
        //pz += 0.3f;
    }

    void render()
    {
        glPushMatrix();
        glTranslatef(px, py, pz);
        model->RenderNxVxT(*texture);
        glPopMatrix();
    }

    virtual ~Collectible()
    {
        delete model;
        delete texture;
    }
};

#endif //PROJGRAF_PERSON_H
