//
// Created by giuseppe on 21/08/18.
//

#include <cstdio>
#include "skybox.h"

extern bool useWireframe;

SkyBox::SkyBox(char *sky_file_image) {
    skyTex = new Texture();
    if (!skyTex->loadTexture2D(sky_file_image)) {
        std::fprintf(stderr, "Errore nel caricamento texture per la skybox!");
        exit(1);
    };
}

SkyBox::~SkyBox() {
    delete skyTex;
}


void SkyBox::render() {
    if (useWireframe) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0, 0, 0);
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawSphere(r, lats, longs);
        //drawQuad();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(1, 1, 1);
        glEnable(GL_LIGHTING);
    } else {
        glBindTexture(GL_TEXTURE_2D, skyTex->getTextureID());
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Env map
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glColor3f(1, 1, 1);
        glDisable(GL_LIGHTING);

        drawSphere(r, lats, longs);
        //drawQuad();
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }
}
