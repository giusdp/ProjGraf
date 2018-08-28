#include "finishline.h"

void FinishLine::render()
{

glPushMatrix();
    glTranslatef(0, 3, pz);
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);


    glVertex2f(-14, 10);
    glVertex2f(-12, 10);
    glVertex2f(-12, -2);
    glVertex2f(-14, -2);

    glVertex2f(-12, 10);
    glVertex2f(12, 10);
    glVertex2f(12, 8);
    glVertex2f(-12, 8);

    glVertex2f(14, 10);
    glVertex2f(12, 10);
    glVertex2f(12, -2);
    glVertex2f(14, -2);

    glEnd();
    glEnable(GL_LIGHTING);

    glPopMatrix();

    pz += vzf;
}