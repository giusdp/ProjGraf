#include "finishline.h"

void FinishLine::render()
{
    glPushMatrix();
    glTranslatef(0, 0, -10);
    glColor3f(255, 0, 0);
    finishLineModel.RenderNxV();
    glPopMatrix();
}