//
// Created by giuseppe on 21/08/18.
//

#ifndef PROJGRAF_HUD_H
#define PROJGRAF_HUD_H

#include <GL/gl.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include "texture.h"
#include "point3.h"
#include "utils.h"
class HUD
{

  public:
    int scrW, scrH;

    void resize(int w, int h);
    // Costruttore
    HUD(int scrW, int scrH, TTF_Font *font);

    void update();

    // metodo render
    void render();

    virtual ~HUD()
    {
        delete text;
    }

  private:
    bool useHUD = true;

    TTF_Font *font;

    Texture *text;

    int score = 0;
    SDL_Rect scoreRect;

    void SetCoordToPixel();
    void drawQuad(float x, float y,float w,float h);
};

#endif //PROJGRAF_HUD_H