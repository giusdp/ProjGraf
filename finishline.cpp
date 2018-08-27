#include "finishline.h"

void FinishLine::render()
{
    glPushMatrix();
    glTranslatef(0, 0, -20);
    glColor3f(1,1,1);
    finishLineModel.RenderNxF();
    glPopMatrix();
}