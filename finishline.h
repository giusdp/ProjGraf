#ifndef PROJGRAF_FINISHLINE_H
#define PROJGRAF_FINISHLINE_H

#include "mesh.h"

class FinishLine
{
  public:
    Mesh finishLineModel;

    FinishLine(Mesh mesh) : finishLineModel(mesh){}

    void render();
};

#endif //PROJGRAF_FINISHLINE_H
