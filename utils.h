//
// Created by giuseppe on 22/08/18.
//

#ifndef PROJGRAF_UTILS_H
#define PROJGRAF_UTILS_H

#include <GL/gl.h>
#include <cmath>
#include "point3.h"

static void drawSphere(float r, float lats, float longs)
{

    glPushMatrix();
    glTranslatef(0, 0, -64);
    int i, j;
    for (i = 0; i <= lats; i++)
    {
        auto lat0 = static_cast<float>(M_PI * (-0.5 + (double)(i - 1) / lats));
        float z0 = std::sin(lat0);
        float zr0 = std::cos(lat0);

        auto lat1 = static_cast<float>(M_PI * (-0.5 + (double)i / lats));
        float z1 = std::sin(lat1);
        float zr1 = std::cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++)
        {
            auto lng = static_cast<float>(2 * M_PI * (double)(j - 1) / longs);
            float x = std::cos(lng);
            float y = std::sin(lng);

            //le normali servono per l'EnvMap
            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(r * x * zr0, r * y * zr0, r * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(r * x * zr1, r * y * zr1, r * z1);
        }
        glEnd();
    }
    glPopMatrix();
}

static float mapRange(float x, float a, float b, float c, float d)
{
    float y = (x - a) * ((d - c) / (b - a)) + c;
    return y;
}

static void convertCoordsOBJtoWin(double ox, double oy, double oz, double *wx, double *wy, double *wz)
{
    double mv[16], p[16];
    int vp[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, mv);
    glGetDoublev(GL_PROJECTION_MATRIX, p);
    glGetIntegerv(GL_VIEWPORT, vp);
    gluProject(ox, oy, oz, mv, p, vp, wx, wy, wz); // mappa coordinate oggetto a coordinate finestra
}

static void convertCoordsWintoObj(double wx, double wy, double wz, double *ox, double *oy, double *oz)
{
    double mv[16], p[16];
    int vp[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, mv);
    glGetDoublev(GL_PROJECTION_MATRIX, p);
    glGetIntegerv(GL_VIEWPORT, vp);
    gluUnProject(wx, wy, wz, mv, p, vp, ox, oy, oz);
}

/*void drawQuad() {
    glPushMatrix();

    glTranslatef(0,0,-64);

    Point3 p1 = Point3(-64, -10, 0);
    Point3 p2 = Point3(64, -10, 0);
    Point3 p3 = Point3(-64, 40, 0);
    Point3 p4 = Point3(64, 40, 0);

    glBegin(GL_QUAD_STRIP);

    p1.SendAsNormal();
    p1.SendAsVertex();
    p2.SendAsNormal();
    p2.SendAsVertex();
    p3.SendAsNormal();
    p3.SendAsVertex();
    p4.SendAsNormal();
    p4.SendAsVertex();

    glEnd();

    glPopMatrix();

}*/

#endif //PROJGRAF_UTILS_H
