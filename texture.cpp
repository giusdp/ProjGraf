//
// Created by giuseppe on 21/08/18.
//

#include "texture.h"

bool Texture::loadTexture2D(char *filename) {

    // 1. Generare un nome per un texture object -> glGenTextures(n, &texName)
    glGenTextures(1, &textureID);

    // 2. Associare texture object ad una texture image -> glBindTexture(GL_TEXTURE_2D, texName);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 3. Dichiarare l’array di texels: GLubyte my_texels[512][512];
    // In questo caso faccio tutto con una SDL_Surface poiche' ho visto che va bene
    SDL_Surface *image;

    // 4. Leggere una immagine e metterla in my_texels
    // In questo caso leggo l'immagine con sdl image e carico nella surface
    image = IMG_Load(filename);
    if (!image){
        std::fprintf(stderr, "ERROR: Failed to load image at: %s\n", filename);
        return false;
    }

    GLenum texture_format;

    if (image->format->BytesPerPixel == 4) {     // contiene canale alpha
        if (image->format->Rmask == 0x000000ff) {
            texture_format = GL_RGBA;
        } else {
            texture_format = GL_BGRA;
        }
    } else if (image->format->BytesPerPixel == 3) {     // non contiene canale alpha
        if (image->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    } else {
        std::fprintf(stderr, "Errore nel caricamente della immagine %s the image is not truecolor\n", filename);
        exit(1);
    }

    // 5. Definire una texture image 2D da un array di texel in texture RAM;
    // Di nuovo, uso la surface per dare i dati necessari per la creazione della texture
    glTexImage2D(GL_TEXTURE_2D, 0, texture_format, image->w, image->h, 0, texture_format, GL_UNSIGNED_BYTE, image->pixels);

    // Altra roba glTex
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->w, image->h, texture_format, GL_UNSIGNED_BYTE, image->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    SDL_FreeSurface(image);
return true;
}

GLuint Texture::getTextureID() const {
    return textureID;
}
