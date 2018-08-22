//
// Created by giuseppe on 21/08/18.
//

#include "HUD.h"

void HUD::SetCoordToPixel() {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-1, -1, 0);
    glScalef(2.0f / scrW, 2.0f / scrH, 1);


}
