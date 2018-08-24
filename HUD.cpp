//
// Created by giuseppe on 21/08/18.
//

#include "HUD.h"

extern bool useWireframe;

HUD::HUD(int scrW, int scrH, TTF_Font *font) : scrW(scrW), scrH(scrH), font(font)
{
    SDL_Surface *surfaceText = TTF_RenderText_Blended(font, "SCORE ", {255, 0, 0, 0});

    scoreRect.w = 4;
    scoreRect.h = 2;
    scoreRect.x = 4;
    scoreRect.y = 12;

    text = new Texture();
    text->loadTexture2D(surfaceText, true);
    resize(scrW, scrH);

}
void HUD::resize(int w, int h)
{
    scrW = w;
    scrH = h;    
}
void HUD::update()
{
}

void HUD::render()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, text->getTextureID());

    // SCORE
    drawQuad(scoreRect.x, scoreRect.y, scoreRect.w, scoreRect.h);
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void HUD::drawQuad(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(x, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + w, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + w, y + h);
    glTexCoord2f(0, 0);
    glVertex2f(x, y + h);

    glEnd();
}
