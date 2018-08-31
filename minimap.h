#ifndef PROJGRAF_MINIMAP_H
#define PROJGRAF_MINIMAP_H

#include <stdio.h>
#include "plane.h"
#include "utils.h"
#include "finishline.h"
extern int scrW, scrH;

class MiniMap
{
  public:
    Plane *plane;
    FinishLine *fl;
    float px = 5, py = 0.2, pz = 0;
    float x = 0, y = 0, w = 3, h = 3;

    float minWx, maxWx, minWy, maxWy;

    MiniMap(Plane *plane, FinishLine *fl) : plane(plane), fl(fl)
    {
        update();
    }

    void update()
    {
        double ox, oy, oz;
        convertCoordsWintoObj(-100, -100, 0, &ox, &oy, &oz);
        minWx = static_cast<float>(ox);
        minWy = static_cast<float>(oy);

        convertCoordsWintoObj(scrW + 100, scrH - 100, 0, &ox, &oy, &oz);
        maxWx = static_cast<float>(ox);
        maxWy = static_cast<float>(oy);

        std::cout << minWx << " " << maxWx << std::endl;
    }

    void render()
    {
        glPushMatrix();
        glTranslatef(px, py, pz);

        glDisable(GL_LIGHTING);

        drawMap();

        float plane_x;
        plane_x = mapRange(plane->px, minWx * 10, maxWx * 10, x, w);
        // std::cout << plane->px  << " "<< plane_x << " " << px <<" "<< px+w << std::endl;
        glPushMatrix();
        glColor3f(0.3, 0.3, 0.3);
        glTranslatef(plane_x, py, pz);
        drawPlayerIcon();
        glPopMatrix();

        float yfl = mapRange(fl->pz, -150, 5, y, h);
        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(x, -yfl, pz);

        drawFLIcon();
        glPopMatrix();

        glEnable(GL_LIGHTING);

        glPopMatrix();
    }

    void drawMap()
    {
        glBegin(GL_LINE_LOOP);
        glVertex3f(x, y, 0);

        glVertex3f(x, y + h, 0);

        glVertex3f(x + w, y + h, 0);

        glVertex3f(x + w, y, 0);
        glEnd();
    }

    void drawPlayerIcon()
    {
        drawSphere(.1, 4, 4);
    }

    void drawFLIcon()
    {
        glBegin(GL_LINE_LOOP);
        glVertex3f(w / 2 - 0.5, h, 0);

        glVertex3f(w / 2 + 0.5, h, 0);

        glEnd();
    }
};

#endif //PROJGRAF_MINIMAP_H