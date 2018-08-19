#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <utility>

#endif

#include "mesh.h"

class Plane {
private:
    Mesh lowPolyPlane;

    void RenderAllParts(bool usecolor);

public:
    bool goForward, goBack, goLeft, goRight;

    // Metodi
    void Init();         // inizializza variabili
    void Render(); // disegna a schermo
    void DoStep();       // computa un passo del motore fisico
    explicit Plane(Mesh lowPolyPlane) : lowPolyPlane(std::move(lowPolyPlane)) {
        goForward = false;
        goBack = false;
        goLeft = false;
        goRight = false;
        Init();
    }  // costruttore

    // STATO DELLA MACCHINA
    // (DoStep fa evolvere queste variabili nel tempo)
    float px{}, py{}, pz{}, facing{};     // posizione e orientamento
    float mozzoA{}, mozzoP{}, sterzo{}; // stato interno
    float vx{}, vy{}, vz{};             // velocita' attuale

    // STATS DELLA MACCHINA
    // (di solito rimangono costanti)
    float velSterzo{}, velRitornoSterzo{}, accMax{}, attrito{},
            raggioRuotaA{}, raggioRuotaP{}, grip{},
            attritoX{}, attritoY{}, attritoZ{}; // attriti
};