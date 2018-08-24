#include "plane.h"

#define FREE_MODE 0
#define PLAY_MODE 1

extern bool useEnvmap; // var globale esterna: per usare l'evnrionment mapping
extern bool useShadow; // var globale esterna: per generare l'ombra
extern int cameraType; // var globale esterna per fare i movimenti opportuni

float turning_rotation_Z = 0, max_turn_Z = 20, turn_speed_Z = 0.5f;
float turning_rotation_X, turn_speed_X = 0.4f, max_turn_X = 15;

void Plane::Init() {

    // inizializzo lo stato della macchina
    px = pz = facing = 0; // posizione e orientamento
    py = 2.0;

    mozzoA = mozzoP = sterzo = 0;   // stato
    vx = vy = vz = 0;      // velocita' attuale

    //velSterzo=3.4;         // A
    velSterzo = 2;         // A
    velRitornoSterzo = 0.93; // B, sterzo massimo = A*B / (1-B)

    accMax = 0.0011;

    // attriti: percentuale di velocita' che viene mantenuta
    // 1 = no attrito
    // <<1 = attrito grande
    attritoZ = 0.991;  // piccolo attrito sulla Z (nel senso di rotolamento delle ruote)
    attritoX = 0.8;  // grande attrito sulla X (per non fare slittare la macchina)
    attritoY = 1;  // attrito sulla y nullo

    // Nota: vel max = accMax*attritoZ / (1-attritoZ)


    grip = 0.45; // quanto il facing macchina si adegua velocemente allo sterzo

}

// disegna a schermo
void Plane::Render() {
    // sono nello spazio mondo

    glPushMatrix();

    glTranslatef(px, py, pz);

    glRotatef(facing, 0, 1, 0);
    glRotatef(turning_rotation_Z, 0, 0, 1);
    glRotatef(turning_rotation_X, 1, 0, 0);

    //std::printf("%f %f %f %f \n", px, py, pz, facing);

    // TODO DrawHeadlight(-0.3,0,-1, 0, useHeadlight); // accendi faro sinistro
    // TODO DrawHeadlight(+0.3,0,-1, 1, useHeadlight); // accendi faro destro

    RenderAllParts(true);

    // ombra!
    if (useShadow) {
        glColor3f(0.4, 0.4, 0.4); // colore fisso
        glTranslatef(0, 0.01, 0); // alzo l'ombra di un epsilon per evitare z-fighting con il pavimento
        glScalef(1.01, 0, 1.01);  // appiattisco sulla Y, ingrandisco dell'1% sulla Z e sulla X
        glDisable(GL_LIGHTING); // niente lighing per l'ombra
        RenderAllParts(false);  // disegno la macchina appiattita

        glEnable(GL_LIGHTING);
    }
    glPopMatrix();


    glPopMatrix();
}

// DoStep: facciamo un passo di fisica (a delta_t costante)
//
// Indipendente dal rendering.
//
// ricordiamoci che possiamo LEGGERE ma mai SCRIVERE
// il controller da DoStep
void Plane::DoStep() {
    calcTurningAnimation();
    if (cameraType == PLAY_MODE) {
        doStepPlayMode();
    } else {
        vy = 0;
        doStepFreeMode();
    }

}

// funzione che disegna tutti i pezzi
// (da invocarsi due volte: per la mesh, e per la sua ombra)
// (se usecolor e' falso, NON sovrascrive il colore corrente
//  e usa quello stabilito prima di chiamare la funzione)
void Plane::RenderAllParts(bool usecolor) {
    // disegna la carliga con una mesh
    glPushMatrix();
    if (!useEnvmap) {
        if (usecolor)
            glColor3f(1, 0, 0); // colore rosso, da usare con Lighting
    } else {
        if (usecolor)
            SetupEnvmapTexture();
    }
    glScalef(1.5, 1.5, 1.5);
    lowPolyPlane.RenderNxV(); // rendering delle mesh carlinga usando normali per vertice
    if (usecolor)
        glEnable(GL_LIGHTING);

    glPopMatrix();
}


// Funzione che prepara tutto per usare un env map
void Plane::SetupEnvmapTexture() {
    // facciamo binding con la texture 1
    glBindTexture(GL_TEXTURE_2D, envMapTexture.getTextureID());

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S); // abilito la generazione automatica delle coord texture S e T
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Env map
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glColor3f(1, 1, 1); // metto il colore neutro (viene moltiplicato col colore texture, componente per componente)
    glDisable(GL_LIGHTING); // disabilito il lighting OpenGL standard (lo faccio con la texture)
}

void Plane::calcTurningAnimation() {
    // Piccola rotazione per quando si va a destra o sinistra
    animationStep(goLeft, goRight, turning_rotation_Z, turn_speed_Z, max_turn_Z);

    // Rotazione per quando si va sopra o sotto (o avanti e dietro)
    if (cameraType == PLAY_MODE) animationStep(goForward, goBack, turning_rotation_X, turn_speed_X, max_turn_X);
    else animationStep(goBack, goForward, turning_rotation_X, turn_speed_X, max_turn_X);
}

void Plane::animationStep(bool move1, bool move2, float &turning, float speed, float max){
    if (move1 || move2) {
        if (move1) {
            turning += speed;
        }
        if (move2) {
            turning -= speed;
        }
    } else {
        if (turning < -0.1f) {
            turning += 0.3f;
        } else if (turning > 0.1f) {
            turning -= 0.3f;
        } else turning = 0;
    }
    if (turning > max) turning = max;
    else if (turning < -max) turning = -max;
}

void Plane::doStepFreeMode() {
    // computiamo l'evolversi della macchina

    float vxm, vym, vzm; // velocita' in spazio macchina

    // da vel frame mondo a vel frame macchina
    auto cosf = static_cast<float>(cos(facing * M_PI / 180.0));
    auto sinf = static_cast<float>(sin(facing * M_PI / 180.0));
    vxm = +cosf * vx - sinf * vz;
    vym = vy;
    vzm = +sinf * vx + cosf * vz;

    // gestione dello sterzo
    if (goLeft) sterzo += velSterzo;
    if (goRight) sterzo -= velSterzo;
    sterzo *= velRitornoSterzo; // ritorno a volante dritto

    if (goForward) vzm -= accMax; // accelerazione in avanti
    if (goBack) vzm += accMax;  // accelerazione indietro

    // attriti (semplificando)
    vxm *= attritoX;
    vym *= attritoY;
    vzm *= attritoZ;

    // l'orientamento della macchina segue quello dello sterzo
    // (a seconda della velocita' sulla z)
    facing = facing - (vzm * grip) * sterzo;

    // rotazione mozzo ruote (a seconda della velocita' sulla z)

    // ritorno a vel coord mondo
    vx = +cosf * vxm + sinf * vzm;
    vy = vym;
    vz = -sinf * vxm + cosf * vzm;

    // posizione = posizione + velocita * delta t (ma delta t e' costante)
    px += vx;
    py += vy;
    pz += vz;

}

extern int scrW, scrH;

void Plane::doStepPlayMode() {

    float vxm = vx, vym = vy;

    if (goLeft) vxm -= accMax;
    if (goRight) vxm += accMax;

    vxm *= 0.995;

    if (goForward) vym += accMax;
    if (goBack) vym -= accMax;

    vym *= 0.995;

    // rotazione mozzo ruote (a seconda della velocita' sulla z)

    // ritorno a vel coord mondo
    vx = vxm;
    vy = vym;

    // posizione = posizione + velocita * delta t (ma delta t e' costante)
    px += vx;
    py += vy;
    pz += vz;


    // CONTROLLO BORDI
    double wx, wy,wz;
    convertCoordsOBJtoWin(px, py, pz, &wx, &wy,&wz); // mappa coordinate oggetto a coordinate finestra
    if (wx > scrW + 100) { // 100 è l'offset per far muovere la navicella quasi del tutto fuori dallo schermo prima di
        // tornare all'inizio
        double ox, oy, oz;
        convertCoordsWintoObj(0, wy, wz, &ox, &oy, &oz);
        px = static_cast<float>(ox);
    }
    else if (wx < -100) {
        double ox, oy, oz;
        convertCoordsWintoObj(scrW, wy, wz, &ox, &oy, &oz);
        px = static_cast<float>(ox);
    }

    if (wy > scrH + 100) { // 100 è l'offset per far muovere la navicella quasi del tutto fuori dallo schermo prima di
        // tornare all'inizio
        double ox, oy, oz;
        convertCoordsWintoObj(wx, 0, wz, &ox, &oy, &oz);
        py = static_cast<float>(oy);
    }
    else if (wy < -50) {
        double ox, oy, oz;
        convertCoordsWintoObj(wx, scrH, wz, &ox, &oy, &oz);
        py = static_cast<float>(oy);
    }



}

