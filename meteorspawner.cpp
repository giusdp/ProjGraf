//
// Created by giuseppe on 22/08/18.
//

#include <cstdio>
#include "meteorspawner.h"


void MeteorSpawner::render() {

    for (Meteor *m : meteors){
        m->render(meteorTexture->getTextureID());
    }

}

MeteorSpawner::MeteorSpawner() {
    meteorTexture = new Texture();
    meteorTexture->loadTexture2D((char *)"Assets/meteor_texture.jpg");

    addMeteor(0);
    addMeteor(5);
    addMeteor(10);
    addMeteor(15);

}

MeteorSpawner::~MeteorSpawner() {
    for (Meteor *m : meteors) delete m;
    meteors.clear();
    delete meteorTexture;
}

void MeteorSpawner::addMeteor(float x) {
    auto *m = new Meteor(3, 12, 12);
    m->px = x;
    meteors.push_back(m);
    //std::printf("Numero di meteore attive: %d\n", static_cast<int>(meteors.size()));
}
