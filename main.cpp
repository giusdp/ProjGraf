#include <cmath>
#include <iostream>
#include "controller.h"

#define CAMERA_BACK_CAR 0
#define CAMERA_MOUSE 1

bool useWireframe = false;
bool useEnvmap = true;
bool useShadow = true;

int scrW = 800, scrH = 600; // altezza e larghezza viewport (in pixels)

// Frames Per Seconds
const int fpsSampling = 3000; // lunghezza intervallo di calcolo fps
float fps = 0;                // valore di fps dell'intervallo precedente
int fpsNow = 0;               // quanti fotogrammi ho disegnato fin'ora nell'intervallo attuale
Uint32 timeLastInterval = 0;  // quando e' cominciato l'ultimo intervallo

int nstep = 0;                     // numero di passi di FISICA fatti fin'ora
const int PHYS_SAMPLING_STEP = 10; // numero di millisec che un passo di fisica simula

int cameraType = 1;

// setto la posizione della camera
void setCamera(Plane plane) {

    double px = plane.px;
    double py = plane.py;
    double pz = plane.pz;
    double angle = plane.facing;
    //std::printf("%f %f %f %f \n", px, py, pz, angle);
    double cosf = cos(angle * M_PI / 180.0);
    double sinf = sin(angle * M_PI / 180.0);
    double camd, camh, ex, ey, ez, cx, cy, cz;
    double cosff, sinff;


    float viewAlpha=20, viewBeta=40; // angoli che definiscono la vista
    float eyeDist=10.0; // distanza dell'occhio dall'origine

    // controllo la posizione della camera a seconda dell'opzione selezionata
    switch (cameraType) {
        case CAMERA_BACK_CAR:
            camd = 50;
            camh = 15.0;
            ex = px + camd * sinf;
            ey = py + camh;
            ez = pz + camd * cosf;
            cx = px - camd * sinf;
            cy = py + camh;
            cz = pz - camd * cosf;
            gluLookAt(ex, ey, ez, cx, cy, cz, 0.0, 1.0, 0.0);
            break;
        case CAMERA_MOUSE:
            glTranslatef(0,0,-eyeDist);
            glRotatef(viewBeta,  1,0,0);
            glRotatef(viewAlpha, 0,1,0);
            break;
        default:
            break;
    }
}

/* Esegue il Rendering della scena */
void render(SDL_Window *win, Plane plane) {

    // un nuovo frame
    fpsNow++;

    glLineWidth(3); // linee larghe

    // settiamo il viewport
    glViewport(0, 0, scrW, scrH);

    // settiamo la matrice di proiezione
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70,                   //fovy,
                   ((float) scrW) / scrH, //aspect Y/X,
                   0.2,                  //distanza del NEAR CLIPPING PLANE in coordinate vista
                   1000                  //distanza del FAR CLIPPING PLANE in coordinate vista
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // riempe tutto lo screen buffer di pixel color sfondo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,1,1, 1);

    // setto la posizione luce
    float tmpv[4] = {0, 1, 2, 0}; // ultima comp=0 => luce direzionale
    glLightfv(GL_LIGHT0, GL_POSITION, tmpv);

    // settiamo matrice di vista
    setCamera(plane);

    static float tmpcol[4] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpcol);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127);

    glEnable(GL_LIGHTING);

    // settiamo matrice di modellazione
    // TODO drawSky(); // disegna il cielo come sfondo

    // TODO drawFloor(); // disegna il suolo
    // TODO drawPista(); // disegna la pista

    plane.Render(); // disegna la macchina

    // attendiamo la fine della rasterizzazione di
    // tutte le primitive mandate
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // disegnamo i fps (frame x sec) come una barra a sinistra.
    // (vuota = 0 fps, piena = 100 fps)
    // TODO SetCoordToPixel();

    /*
    glBegin(GL_QUADS);
    float y = scrH * fps / 100;
    float ramp = fps / 100;
    glColor3f(1 - ramp, 0, ramp);
    glVertex2d(10, 0);
    glVertex2d(10, y);
    glVertex2d(0, y);
    glVertex2d(0, 0);
    glEnd();
    */

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glFinish();
    // ho finito: buffer di lavoro diventa visibile
    SDL_GL_SwapWindow(win);
}

int main(int argc, char *argv[]) {
    //Plane *plane = new Plane(Mesh((char *) "Ferrari_chassis.obj"));// l'aereoplanino
    Plane *plane = new Plane(Mesh((char *) "Assets/lowpolyplane.obj"));// l'aereoplanino
    SDL_Window *win;
    SDL_GLContext mainContext;
    Uint32 windowID;

    // inizializzazione di SDL
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // facciamo una finestra di scrW x scrH pixels
    win = SDL_CreateWindow("Progetto Grafica", 0, 0, scrW, scrH,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    //Create our opengl context and attach it to our window
    mainContext = SDL_GL_CreateContext(win);

    std::cout << "SDL avviato e finestra con contesto creati." << std::endl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // rinormalizza le normali prima di usarle

    glFrontFace(GL_CW); // consideriamo Front Facing le facce ClockWise
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_POLYGON_OFFSET_FILL); // sposta frammenti generati dalla
    glPolygonOffset(1, 1);            // rasterizzazione poligoni indietro di 1

    //if (!LoadTexture(0,(char *)"logo.jpg")) return 0;
    //if (!LoadTexture(1,(char *)"envmap_flipped.jpg")) return 0;
    //if (!LoadTexture(2,(char *)"sky_ok.jpg")) return -1;

    //Creo il controller per gestire gli input con il Command Pattern
    Controller controller = Controller();
    bool done = false;
    SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE); // Per non floodare la coda
    std::cout << "Game loop avviato." << std::endl;
    while (!done) {
        SDL_Event e;
        // guardo se c'e' un evento:
        if (SDL_PollEvent(&e)) {
            // se si: processa evento
            switch (e.type) {
                case SDL_QUIT:
                    std::cout << "QUIT Premuto" << std::endl;
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        std::cout << "ESC premuto" << std::endl;
                        done = true;
                    }
                    controller.handleInputs(plane, e, true);
                    break;
                case SDL_KEYUP:
                    controller.handleInputs(plane, e, false);
                    break;

                case SDL_WINDOWEVENT:
                    // dobbiamo ridisegnare la finestra
                    if (e.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        render(win, *plane);
                    } else {
                        windowID = SDL_GetWindowID(win);
                        if (e.window.windowID == windowID) {
                            switch (e.window.event) {
                                case SDL_WINDOWEVENT_SIZE_CHANGED: {
                                    scrW = e.window.data1;
                                    scrH = e.window.data2;
                                    glViewport(0, 0, scrW, scrH);
                                    render(win, *plane);
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
            // nessun evento: siamo IDLE

            Uint32 timeNow = SDL_GetTicks(); // che ore sono?

            if (timeLastInterval + fpsSampling < timeNow) {
                fps = static_cast<float>(1000.0 * ((float) fpsNow) / (timeNow - timeLastInterval));
                fpsNow = 0;
                timeLastInterval = timeNow;
            }

            bool doneSomething = false;
            int guardia = 0; // sicurezza da loop infinito

            // finche' il tempo simulato e' rimasto indietro rispetto
            // al tempo reale...
            while (nstep * PHYS_SAMPLING_STEP < timeNow) {
                plane->DoStep();
                nstep++;
                doneSomething = true;
                timeNow = SDL_GetTicks();
                if (guardia++ > 1000) {
                    done = true;
                    break;
                } // siamo troppo lenti!
            }

            if (doneSomething)
                render(win, *plane);
                //redraw();
            else {
                // tempo libero!!!
            }
        }
    }
    std::cout << "Uscito dal game loop." << std::endl;

    delete plane;
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(win);
    SDL_Quit();
    std::cout << "Memoria e processi puliti." << std::endl;
    return (0);
}
