//
// Created by giuseppe on 21/08/18.
//

#include "HUD.h"
extern bool useWireframe;

SDL_Color scoreColor = {255, 0, 20, 0},
stageColor ={20, 0, 255, 0}, itemsColor ={20, 255, 0, 0};


HUD::HUD(int scrW, int scrH, TTF_Font *font) : scrW(scrW), scrH(scrH), font(font)
{

    scoreRect.x = 5;
    scoreRect.y = 12;
    scoreRect.w = 3;
    scoreRect.h = 1;

    stageRect.x = -8;
    stageRect.y = 12;
    stageRect.w = 2;
    stageRect.h = 1;

    itemsRect.x = 5;
    itemsRect.y = 0;
    itemsRect.w = 3;
    itemsRect.h = 1;

    SDL_Surface *surfaceTextScore = TTF_RenderText_Blended(font, scoreText.c_str(), scoreColor);
    textureScore = new Texture();
    textureScore->loadTexture2D(surfaceTextScore, true);

    SDL_Surface *surfaceText = TTF_RenderText_Blended(font, stageText.c_str(), stageColor);
    textureStage = new Texture();
    textureStage->loadTexture2D(surfaceText, true);

    SDL_Surface *surfaceTextItem = TTF_RenderText_Blended(font, itemsText.c_str(), itemsColor);
    textureItems = new Texture();
    textureItems->loadTexture2D(surfaceTextItem, true);
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
        SDL_Surface *surfaceText = TTF_RenderText_Blended(font, s.c_str(), stageColor);
        textureStage->loadTexture2D(surfaceText, true);
        stageChanged = false;
        collectedItems = 0;
        justCollected=true;
    }
    if (justCollected)
    {
        std::string s = itemsText;
        s += std::to_string(collectedItems);
        s +="/10";
        if (collectedItems == 10) {stage++; stageChanged = true;}
        SDL_Surface *surfaceText = TTF_RenderText_Blended(font, s.c_str(), itemsColor);
        textureItems->loadTexture2D(surfaceText, true);
        justCollected = false;
    }
    if (counter == 100)
    {
        score++;
        counter = 0;
        std::string s = scoreText;
        s += std::to_string(score);
        SDL_Surface *surfaceTextScore = TTF_RenderText_Blended(font, s.c_str(), scoreColor);
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

    glColor3f(1, 1, 1);

    // SCORE
    glBindTexture(GL_TEXTURE_2D, textureScore->getTextureID());
    drawQuad(scoreRect.x, scoreRect.y, scoreRect.w, scoreRect.h);

    // STAGE
    glBindTexture(GL_TEXTURE_2D, textureStage->getTextureID());
    drawQuad(stageRect.x, stageRect.y, stageRect.w, stageRect.h);

     // ITEMS
    glBindTexture(GL_TEXTURE_2D, textureItems->getTextureID());
    drawQuad(itemsRect.x, itemsRect.y, itemsRect.w, itemsRect.h);

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
