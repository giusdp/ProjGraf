//
// Created by giuseppe on 21/08/18.
//

#ifndef PROJGRAF_TEXTURELOADER_H
#define PROJGRAF_TEXTURELOADER_H

#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#include <SDL2_image/SDL_surface.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <utility>

#endif

class Texture
{
  private:
    GLuint textureID;

  public:
    bool loadTexture2D(char *filename);
    bool loadTexture2D(SDL_Surface *surface, bool isText);

    GLuint getTextureID() const;

    virtual ~Texture();
};

#endif //PROJGRAF_TEXTURELOADER_H
