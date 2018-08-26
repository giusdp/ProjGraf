//
// Created by giuseppe on 22/08/18.
//

#ifndef PROJGRAF_METEORSHOWER_H
#define PROJGRAF_METEORSHOWER_H

#include <vector>
#include "utils.h"
#include "plane.h"
#include "texture.h"

extern bool useWireframe;
extern Plane *plane;

class Meteor
{

  public:
    float px = 0, py = 0, pz = -50;

    float gravity = 0.25f; // velocità di caduta
    float vel_Z = 0.55f;   // velocità di avvicinamento alla camera.

    int minX = -30, maxX = 60;

    float r, lats, longs;

    void randomCoords()
    {
        px = 1 + rand() % maxX;
        px += minX;

        py = 1 + rand() % 50;
        py += 30;

        pz = 1 + rand() % 50;
        pz += 30;
        pz *= -1;
    }
    void update()
    {
        if (py < -30)
        {
            randomCoords();
        }

        // check for collision
        Point3 c = plane->lowPolyPlane.Center();
        if (c.coord[0] <= px + r && c.coord[0] >= px - r)
        {
            if (c.coord[1] <= py + r && c.coord[1] >= py - r)
            {
                if (c.coord[2] <= pz + r && c.coord[2] >= pz - r)
                {
                    std::printf("COLPITO\n");
                }
            }
        }
    }
    void render(GLuint textureID)
    {
        glPushMatrix();
        glTranslatef(px, py, pz);

        if (useWireframe)
        {
            glDisable(GL_TEXTURE_2D);
            glColor3f(0, 0, 0);
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            drawSphere(r, lats, longs);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(1, 1, 1);
            glEnable(GL_LIGHTING);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Env map
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glColor3f(1, 1, 1);
            glDisable(GL_LIGHTING);

            drawSphere(r, lats, longs);

            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        }

        glPopMatrix();

        py -= gravity;
        pz += vel_Z;
    }

    Meteor(float r, float lats, float longs) : r(r), lats(lats), longs(longs)
    {
        randomCoords();
    }
};

class MeteorSpawner
{

    std::vector<Meteor *> meteors; // Per tenere traccia delle meteore spawnate

    Texture *meteorTexture;

  public:
    int meteorNumber = 20;
    MeteorSpawner();

    void addMeteor();

    void render();

    virtual ~MeteorSpawner();
};

#endif //PROJGRAF_METEORSHOWER_H
