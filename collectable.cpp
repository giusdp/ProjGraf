#include "collectable.h"
#include "plane.h"
#include <cstdio>

extern void incrCollectedItems();

void Collectable::render()
{
    if (!collected)
    {
        glPushMatrix();

        glTranslatef(px, py, pz);
        glRotatef(180, 0, 0, 1);
        glRotatef(180, 0, 1, 0);

        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
        glColor3f(1, 1, 1);
        glActiveTexture(GL_TEXTURE0);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, 0);

        glTexCoord2f(0, 1);
        glVertex3f(x, y + h, 0);

        glTexCoord2f(1, 1);
        glVertex3f(x + w, y + h, 0);

        glTexCoord2f(1, 0);
        glVertex3f(x + w, y, 0);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);

        glPopMatrix();
    }
}

extern Plane *plane;
bool Collectable::checkCollision()
{
    Point3 center = plane->lowPolyPlane.Center();
    center.coord[0] += plane->px;
    center.coord[1] += plane->py;
    center.coord[2] += plane->pz;
    Point3 bbmin = plane->lowPolyPlane.bbmin;
    Point3 bbmax = plane->lowPolyPlane.bbmax;
    if (center.X() >= px && center.X() <= px + w)
    {
        if (center.Y() >= py - h && center.Y() <= py)
        {
            if (pz >= center.Z() - 1 && pz <= center.Z() + 1)
            {
                return true;
            }
        }
    }
    return false;
}

void Collectable::randomSpawn()
{
    px = 1 + rand() % 20;
    px += -10;

    py = 1 + rand() % 15;
    // py += -1;

    pz = 1 + rand() % 80;
    pz += 50;
    pz *= -1;
}

bool justCollided = false;
void Collectable::update()
{

    // check collision per mettere collected = true;
    if (!collected && checkCollision())
    {
        collected = true;
        justCollided = true;
    }
    // aumenta pz, magari con qualche movimento interessante
    pz += 0.3f;

    // se pz > threshold
    if (pz > 10)
    {
        randomSpawn();
        collected = false;
    }
    if (justCollided && collected)
    {
        incrCollectedItems();
        justCollided = false;
        // notifica il main
    }
}

Collectable::Collectable(char *texturename)
{
    texture = new Texture();
    if (!texture->loadTexture2D(texturename))
    {
        exit(1);
    }
    randomSpawn();
}

Collectable::~Collectable()
{
    delete texture;
}