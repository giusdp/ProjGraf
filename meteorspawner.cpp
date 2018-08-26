//
// Created by giuseppe on 22/08/18.
//

#include <cstdio>
#include "meteorspawner.h"

void MeteorSpawner::render()
{

    for (Meteor *m : meteors)
    {
        m->update();
        m->render(meteorTexture->getTextureID());
    }
}

MeteorSpawner::MeteorSpawner()
{
    meteorTexture = new Texture();
    meteorTexture->loadTexture2D((char *)"Assets/meteor_texture.jpg");

    for (int i = 0; i < meteorNumber; ++i)
    {
        addMeteor();
    }
}

MeteorSpawner::~MeteorSpawner()
{
    for (Meteor *m : meteors)
        delete m;
    meteors.clear();
    delete meteorTexture;
}

void MeteorSpawner::addMeteor()
{
    auto *m = new Meteor(2, 12, 12);
    meteors.push_back(m);
    //std::printf("Numero di meteore attive: %d\n", static_cast<int>(meteors.size()));
}
