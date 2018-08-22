//
// Created by giuseppe on 21/08/18.
//

#ifndef PROJGRAF_TEXTURELOADER_H
#define PROJGRAF_TEXTURELOADER_H

#include <GL/gl.h>
#include <SDL_surface.h>
#include <SDL_image.h>
#include <cstdio>
#include <GL/glu.h>


class Texture {

    GLuint textureID;
public:

    bool loadTexture2D(char *filename);

    GLuint getTextureID() const;

};


#endif //PROJGRAF_TEXTURELOADER_H
