//
// Created by giuseppe on 21/08/18.
//

#ifndef PROJGRAF_HUD_H
#define PROJGRAF_HUD_H


#include <GL/gl.h>

class HUD {

public:
    HUD(int scrW, int scrH) : scrW(scrW), scrH(scrH) {}

private:

    int scrW, scrH;

    void SetCoordToPixel();
};


#endif //PROJGRAF_HUD_H
