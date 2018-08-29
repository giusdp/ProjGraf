#ifndef PROJGRAF_FINISHLINE_H
#define PROJGRAF_FINISHLINE_H

#include "mesh.h"
#include <GL/gl.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include "texture.h"
#include <string>
class FinishLine
{
public:
  Texture *texture;
  TTF_Font *font;
  float pz = -150, vzf = 0.03f;

  FinishLine(TTF_Font *font) : font(font)
  {
    texture = new Texture();
    std::string s = "FINISH LINE";

    SDL_Surface *surface = TTF_RenderText_Blended(font, s.c_str(), {0, 0, 0, 0});
    texture->loadTexture2D(surface, true);
  }

  void render();

  bool hasArrived();

  void reset();

  virtual ~FinishLine()
  {
    delete texture;
  }
};

#endif //PROJGRAF_FINISHLINE_H
