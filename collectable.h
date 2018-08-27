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
class Collectable
{
  private:
    bool checkCollision();

  public:
    Texture *texture;

    bool collected = false;

    float px = 0, py = 5, pz = -100;
    float x = 0, y = 0, w = 2, h = 2;

    explicit Collectable(char *texturename);

    void update();

    void render();

    void randomSpawn();

    virtual ~Collectable();
};

#endif //PROJGRAF_PERSON_H
