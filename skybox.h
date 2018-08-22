//
// Created by giuseppe on 21/08/18.
//

#ifndef PROJGRAF_SKYBOX_H
#define PROJGRAF_SKYBOX_H


#include "texture.h"

class SkyBox {

private:

    Texture *skyTex;
    float r = 100;
    int lats = 20;
    int longs = 20;

public:
    explicit SkyBox(char *sky_file_image);

    void render();

    virtual ~SkyBox();


};


#endif //PROJGRAF_SKYBOX_H
