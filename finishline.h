#ifndef PROJGRAF_FINISHLINE_H
#define PROJGRAF_FINISHLINE_H

#include "mesh.h"

class FinishLine
{
public:
  float pz = -150, vzf = 0.03f;

  void render();

  void checkCollision();
};

#endif //PROJGRAF_FINISHLINE_H
