#ifndef PROJGRAF_PLANE_H
#define PROJGRAF_PLANE_H

#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <utility>

#endif

#include "mesh.h"
#include "texture.h"
#include "utils.h"
class Plane
{
private:
  Texture *envMapTexture;

  void calcTurningAnimation();
  void animationStep(bool move1, bool move2, float &turning, float speed, float max);

  void doStepFreeMode();
  void doStepPlayMode();
  void renderShipLight();

public:
  Mesh lowPolyPlane;
  bool goForward = false, goBack = false, goLeft = false, goRight = false;

  // Metodi
  void Init();   // inizializza variabili
  void Render(); // disegna a schermo
  void DoStep(); // computa un passo del motore fisico
  explicit Plane(Mesh lowPolyPlane) : lowPolyPlane(std::move(lowPolyPlane))
  {
    envMapTexture = new Texture();
    envMapTexture->loadTexture2D((char *)"Assets/envmap_flipped.jpg");
    Init();
  } // costruttore

  // (DoStep fa evolvere queste variabili nel tempo)
  float px{}, py{}, pz{}, facing{};   // posizione e orientamento
  float mozzoA{}, mozzoP{}, sterzo{}; // stato interno
  float vx{}, vy{}, vz{};             // velocita' attuale

  float velSterzo{}, velRitornoSterzo{}, accMax{}, grip{};

  float attritoX = 0.8, attritoY = 1.0, attritoZ = 0.991; // attriti

  virtual ~Plane()
  {
    delete envMapTexture;
  }
};

#endif //PROJGRAF_PLANE_H
