#ifndef PROJGRAF_CAMERA_H
#define PROJGRAF_CAMERA_H

#include "plane.h"


extern int cameraType;

class Camera
{
#define FREE_MODE 0
#define PLAY_MODE 1
  public:
    Plane *plane;

    Camera(Plane &plane) : plane(&plane) {}

    // setto la posizione della camera
    void update()
    {
        double px = plane->px;
        double py = plane->py;
        double pz = plane->pz;
        double angle = plane->facing;
        //std::printf("%f %f %f %f \n", px, py, pz, angle);
        double cosf = cos(angle * M_PI / 180.0);
        double sinf = sin(angle * M_PI / 180.0);
        double camd, camh, ex, ey, ez, cx, cy, cz;
        double cosff, sinff;

        float viewAlpha = 20, viewBeta = 2; // angoli che definiscono la vista
        float eyeDist = 10.0;               // distanza dell'occhio dall'origine

        // controllo la posizione della camera a seconda dell'opzione selezionata
        switch (cameraType)
        {
        case FREE_MODE:

            // if (plane->goForward) glRotatef(viewBeta, 1, 0, 0);
            // if (plane->goBack) glRotatef(-viewBeta*2, 1, 0, 0);
            // if (plane->goRight) glRotatef(-viewBeta*2, 0, 1, 0);
            // if (plane->goLeft) glRotatef(viewBeta*2, 0, 1, 0);
            camd = 10;
            camh = 5;
            ex = px + camd * sinf;
            ey = py + camh;
            ez = pz + camd * cosf;
            cx = px - camd * sinf;
            cy = py + camh;
            cz = pz - camd * cosf;
            gluLookAt(ex, ey, ez, cx, cy, cz, 0.0, 1.0, 0.0);
            break;
        case PLAY_MODE:
            glTranslatef(0, -7, -eyeDist);
            //glRotatef(viewBeta, 1, 0, 0);

            //glRotatef(viewAlpha, 0,1,0);
            break;
        default:
            break;
        }
    }
};
#endif //PROJGRAF_CAMERA_H
