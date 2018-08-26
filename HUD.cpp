//
// Created by giuseppe on 21/08/18.
//

#include "HUD.h"
extern bool useWireframe;

HUD::HUD(int scrW, int scrH, TTF_Font *font) : scrW(scrW), scrH(scrH), font(font)
{

    scoreRect.x = 4;
    scoreRect.y = 12;
    scoreRect.w = 2;
    scoreRect.h = 1;

    stageRect.x = -8;
    stageRect.y = 12;
    stageRect.w = 2;
    stageRect.h = 1;

    SDL_Surface *surfaceTextScore = TTF_RenderText_Blended(font, scoreText.c_str(), {255, 0, 0, 0});
    textureScore = new Texture();
    textureScore->loadTexture2D(surfaceTextScore, true);

    SDL_Surface *surfaceText = TTF_RenderText_Blended(font, stageText.c_str(), {20, 0, 255, 0});
    textureStage = new Texture();
    textureStage->loadTexture2D(surfaceText, true);
}

void HUD::resize(int w, int h)
{
    // scrW = w;
    // scrH = h;
    // double x, y, z;
    // convertCoordsWintoObj(scrW, scrH, 0, &x, &y, &z);
    // scoreRect.x = x;
    // scoreRect.y = y;
}

void HUD::update()
{
    if (stageChanged)
    {
        std::string s = stageText;
        s += std::to_string(stage);
        SDL_Surface *surfaceText = TTF_RenderText_Blended(font, s.c_str(), {20, 0, 255, 0});
        textureStage->loadTexture2D(surfaceText, true);
        stageChanged = false;
    }
    if (counter == 100)
    {
        score++;
        counter = 0;
        std::string s = scoreText;
        s += std::to_string(score);
        SDL_Surface *surfaceTextScore = TTF_RenderText_Blended(font, s.c_str(), {255, 0, 0, 0});
        textureScore->loadTexture2D(surfaceTextScore, true);
    }
    counter++;
}

void HUD::render()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    // SCORE
    glBindTexture(GL_TEXTURE_2D, textureScore->getTextureID());
    drawQuad(scoreRect.x, scoreRect.y, scoreRect.w, scoreRect.h);

    // STAGE
    glBindTexture(GL_TEXTURE_2D, textureStage->getTextureID());
    drawQuad(stageRect.x, stageRect.y, stageRect.w, stageRect.h);

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
