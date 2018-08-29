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
#include <string>

class HUD
{

public:
  int scrW, scrH;
  int score = 0, stage = 1;

  int collectedItems = 0;
  bool justCollected = true;

  bool stageChanged = true;

  void resize(int w, int h);
  // Costruttore
  HUD(int scrW, int scrH, TTF_Font *font);

  void update();

  void gameOver();
  bool isGameOver = false;

  void reset();

  // metodo render
  void render();

  virtual ~HUD()
  {
    delete textureScore, textureStage, textureItems, textureGameOver, textureGameOver2;
  }

private:
  int counter = 0; // timing per aumentare lo score

  bool useHUD = true;
  TTF_Font *font;

  const std::string scoreText = "SCORE: ";
  const std::string stageText = "STAGE: ";
  const std::string itemsText = "ITEMS: ";
  const std::string gameOverText1 = "GAME OVER!    YOUR SCORE IS ";
  const std::string gameOverText2 = "Press R to start again!"; 

  Texture *textureScore;
  Texture *textureStage;
  Texture *textureItems;
  Texture *textureGameOver;
  Texture *textureGameOver2;

  SDL_Rect scoreRect;
  SDL_Rect stageRect;
  SDL_Rect itemsRect;
  SDL_Rect gameOverRect;

  void drawQuad(float x, float y, float w, float h);
};

#endif //PROJGRAF_HUD_H
