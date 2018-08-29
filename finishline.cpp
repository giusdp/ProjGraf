#include "finishline.h"

void FinishLine::render()
{

    glPushMatrix();
    glTranslatef(0, 3, pz);
    glDisable(GL_LIGHTING);

    glColor3f(0.55, 0.27, 0.07);
    glBegin(GL_QUADS);
    glVertex2f(-14, 10);
    glVertex2f(-12, 10);
    glVertex2f(-12, -2);
    glVertex2f(-14, -2);
    glColor3f(1, 1, 1);

    glVertex3f(-12, 10, -0.1);
    glVertex3f(12, 10, -0.1);
    glVertex3f(12, 8, -0.1);
    glVertex3f(-12, 8, -0.1);
    glEnd();

    glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(-12, 10);
    glTexCoord2f(1, 0);
    glVertex2f(12, 10);
    glTexCoord2f(1, 1);
    glVertex2f(12, 8);
    glTexCoord2f(0, 1);
    glVertex2f(-12, 8);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);

    glColor3f(0.55, 0.27, 0.07);
    glBegin(GL_QUADS);
    glVertex2f(14, 10);
    glVertex2f(12, 10);
    glVertex2f(12, -2);
    glVertex2f(14, -2);

    glEnd();
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

bool FinishLine::hasArrived()
{
    if (pz > 5)
        return true;
    pz += vzf;
    return false;
}

void FinishLine::reset()
{
    pz = -150;
}