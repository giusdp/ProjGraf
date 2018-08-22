//
// Created by giuseppe on 22/08/18.
//

#include "meteorshower.h"

void MeteorShower::launchMeteor() {
    meteors.push_back(new Meteor(3, 12, 12));
    std::printf("Numero di meteore attive: %d\n", static_cast<int>(meteors.size()));
}

void MeteorShower::render() {

    for (Meteor *m : meteors){
        m->render(meteorTexture->getTextureID());
    }

}

MeteorShower::MeteorShower() {
    meteorTexture = new Texture();
    meteorTexture->loadTexture2D((char *)"Assets/meteor_texture.jpg");
}

MeteorShower::~MeteorShower() {
    for (Meteor *m : meteors) delete m;
    meteors.clear();
    delete meteorTexture;
}
