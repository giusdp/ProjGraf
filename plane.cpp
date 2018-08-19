#include <iostream>
#include "plane.h"


extern bool useEnvmap; // var globale esterna: per usare l'evnrionment mapping
extern bool useHeadlight; // var globale esterna: per usare i fari
extern bool useShadow; // var globale esterna: per generare l'ombra

void Plane::Init() {
    // inizializzo lo stato della macchina
    px=pz=facing=0; // posizione e orientamento
    py=0.0;

    mozzoA=mozzoP=sterzo=0;   // stato
    vx=vy=vz=0;      // velocita' attuale

    //velSterzo=3.4;         // A
    velSterzo=2;         // A
    velRitornoSterzo=0.93; // B, sterzo massimo = A*B / (1-B)

    accMax = 0.0011;

    // attriti: percentuale di velocita' che viene mantenuta
    // 1 = no attrito
    // <<1 = attrito grande
    attritoZ = 0.991;  // piccolo attrito sulla Z (nel senso di rotolamento delle ruote)
    attritoX = 0.8;  // grande attrito sulla X (per non fare slittare la macchina)
    attritoY = 1.0;  // attrito sulla y nullo

    // Nota: vel max = accMax*attritoZ / (1-attritoZ)

    raggioRuotaA = 0.25;
    raggioRuotaP = 0.35;

    grip = 0.45; // quanto il facing macchina si adegua velocemente allo sterzo
}

float turning_rotation, max_turn = 20;
// disegna a schermo
void Plane::Render() {
    // sono nello spazio mondo

    glPushMatrix();

    glTranslatef(px, py, pz);
    if (goLeft){
        turning_rotation += 0.2f;
    }
    else if (goRight){
        turning_rotation -= 0.2f;
    }
    else{
        if (turning_rotation < -0.1f){
            turning_rotation += 0.2f;
        }
        else if (turning_rotation > 0.1f){
            turning_rotation -= 0.2f;
        } else turning_rotation = 0;
    }
    if (turning_rotation > max_turn) turning_rotation = max_turn;
    else if (turning_rotation < -max_turn) turning_rotation = -max_turn;

    glRotatef(facing, 0, 1, 0);
    glRotatef(turning_rotation, 0, 0, 1);

    //std::printf("%f %f %f %f \n", px, py, pz, facing);

    // TODO DrawHeadlight(-0.3,0,-1, 0, useHeadlight); // accendi faro sinistro
    // TODO DrawHeadlight(+0.3,0,-1, 1, useHeadlight); // accendi faro destro

    RenderAllParts(true);


  // ombra!
  if(useShadow)
  {
    glColor3f(0.4,0.4,0.4); // colore fisso
    glTranslatef(0,0.01,0); // alzo l'ombra di un epsilon per evitare z-fighting con il pavimento
    glScalef(1.01,0,1.01);  // appiattisco sulla Y, ingrandisco dell'1% sulla Z e sulla X 
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
// la struttura controller da DoStep
void Plane::DoStep() {
    // computiamo l'evolversi della macchina

    float vxm, vym, vzm; // velocita' in spazio macchina

    // da vel frame mondo a vel frame macchina
    auto cosf = static_cast<float>(cos(facing * M_PI / 180.0));
    auto sinf = static_cast<float>(sin(facing * M_PI / 180.0));
    vxm = +cosf * vx - sinf * vz;
    vym = vy;
    vzm = +sinf * vx + cosf * vz;

    // gestione dello sterzo
    if (goLeft){
        sterzo+=velSterzo;
    }
    if (goRight){sterzo-=velSterzo;}
    sterzo*=velRitornoSterzo; // ritorno a volante dritto

    if (goForward)
        vzm -= accMax; // accelerazione in avanti
    if (goBack)
        vzm += accMax; // accelerazione indietro

    // attirti (semplificando)
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

// funzione che disegna tutti i pezzi della macchina
// (carlinga, + 4 route)
// (da invocarsi due volte: per la macchina, e per la sua ombra)
// (se usecolor e' falso, NON sovrascrive il colore corrente
//  e usa quello stabilito prima di chiamare la funzione)
void Plane::RenderAllParts(bool usecolor) {
    // disegna la carliga con una mesh
    glPushMatrix();
    // if (!useEnvmap)
    // {
    //     if (usecolor)
    //         glColor3f(1, 0, 0); // colore rosso, da usare con Lighting
    // }
    // else
    // {
    //     if (usecolor)
    //         SetupEnvmapTexture();
    // }
    lowPolyPlane.RenderNxV(); // rendering delle mesh carlinga usando normali per vertice
    if (usecolor)
        glEnable(GL_LIGHTING);

    glPopMatrix();
}