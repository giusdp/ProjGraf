#include <cmath>
#include <iostream>
#include "controller.h"
#include "terrain.h"
#include "texture.h"
#include "skybox.h"
#include "meteorspawner.h"
#include "HUD.h"
#include "collectable.h"
#include "camera.h"
#include "finishline.h"

#define FREE_MODE 0
#define PLAY_MODE 1

bool useWireframe = false;

int scrW = 800, scrH = 600; // altezza e larghezza viewport (in pixels)

// Frames Per Seconds
const int fpsSampling = 3000; // lunghezza intervallo di calcolo fps
float fps = 0;                // valore di fps dell'intervallo precedente
int fpsNow = 0;               // quanti fotogrammi ho disegnato fin'ora nell'intervallo attuale
Uint32 timeLastInterval = 0;  // quando e' cominciato l'ultimo intervallo

int nstep = 0;                     // numero di passi di FISICA fatti fin'ora
const int PHYS_SAMPLING_STEP = 10; // numero di millisec che un passo di fisica simula

int cameraType = 1;

Plane *plane;
Texture texturePlane;
Terrain *terrain;
SkyBox *sky;
MeteorSpawner *meteorShower;
HUD *hud;
Camera *camera;
FinishLine *finishLine;
std::vector<Collectable *> collectables;

void incrCollectedItems(){
    hud->score += 500;
    hud->collectedItems++;
    hud->justCollected = true;
}

/* Esegue il Rendering della scena */
void render(SDL_Window *win)
{

    // un nuovo frame
    fpsNow++;

    glLineWidth(3); // linee larghe

    // settiamo il viewport
    glViewport(0, 0, scrW, scrH);

    // settiamo la matrice di proiezione
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70,                   //fovy,
                   ((float)scrW) / scrH, //aspect Y/X,
                   0.2,                  //distanza del NEAR CLIPPING PLANE in coordinate vista
                   1000                  //distanza del FAR CLIPPING PLANE in coordinate vista
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // riempe tutto lo screen buffer di pixel color sfondo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);

    // setto la posizione luce
    float tmpv[4] = {0, 1, 2, 0}; // ultima comp=0 => luce direzionale
    glLightfv(GL_LIGHT0, GL_POSITION, tmpv);

    // settiamo matrice di vista
    camera->update();

    static float tmpcol[4] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpcol);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127);

    glEnable(GL_LIGHTING);

    sky->render();     // disegna il cielo come sfondo
    terrain->render(); // disegna il terreno
    plane->Render();   // disegna il giocatore
    //meteorShower->render(); // disegna le meteore
    for (auto c : collectables)
        c->render();
    finishLine->render();
    hud->render();

    // attendiamo la fine della rasterizzazione di
    // tutte le primitive mandate
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glFlush();
    // ho finito: buffer di lavoro diventa visibile
    SDL_GL_SwapWindow(win);
}

int main(int argc, char *argv[])
{
    srand(time(NULL)); // seed rng per gli oggetti da evitare e raccogliere
    rand();

    SDL_Window *win;
    SDL_GLContext mainContext;
    Uint32 windowID;

    // inizializzazione di SDL
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        std::fprintf(stderr, "Errore inizializzazione sdl.\n");
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // facciamo una finestra di scrW x scrH pixels
    win = SDL_CreateWindow("Progetto Grafica", 0, 0, scrW, scrH,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (win == nullptr)
    {
        std::fprintf(stderr, "Errore creazione window.\n");
        SDL_Quit();
        return 1;
    }

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

    // INIT TESTO PER HUD
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("Assets/neuropol.ttf", 12);
    if (font == nullptr)
    {
        std::fprintf(stderr, "Errore lettura font.\n");
        SDL_GL_DeleteContext(mainContext);
        SDL_DestroyWindow(win);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // *** ALLOCAZIONE ENTITA ***
    plane = new Plane(Mesh((char *)"Assets/lowpolyplane.obj")); // l'aereoplanino
    camera = new Camera(*plane);
    terrain = new Terrain();
    sky = new SkyBox((char *)"Assets/hills_ft.tga");
    finishLine = new FinishLine(Mesh((char *)"Assets/Finish.obj"));
    meteorShower = new MeteorSpawner();
    collectables.push_back(new Collectable((char *)"Assets/PersonalPic.jpeg"));
    collectables.push_back(new Collectable((char *)"Assets/PersonalPicAndrew.jpeg"));
    //collectables.push_back(new Collectable((char *)"Assets/PersonalPic.jpeg"));

    //Creo il controller per gestire gli input con il Command Pattern
    Controller controller = Controller();

    hud = new HUD(scrW, scrH, font);

    bool done = false;
    SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE); // Per non floodare la coda

    std::cout << "Game loop avviato." << std::endl;
    while (!done)
    {
        SDL_Event e;
        // guardo se c'e' un evento:
        if (SDL_PollEvent(&e))
        {
            // se si: processa evento
            switch (e.type)
            {
            case SDL_QUIT:
                std::cout << "QUIT Premuto" << std::endl;
                done = true;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    std::cout << "ESC premuto" << std::endl;
                    done = true;
                }
                else if (e.key.keysym.sym == 'c')
                    cameraType = !cameraType;
                else if (e.key.keysym.sym == 'v')
                    useWireframe = !useWireframe;
                controller.handleInputs(plane, e, true);
                break;
            case SDL_KEYUP:
                controller.handleInputs(plane, e, false);
                break;

            case SDL_WINDOWEVENT:
                // dobbiamo ridisegnare la finestra
                if (e.window.event == SDL_WINDOWEVENT_EXPOSED)
                {
                    //hud->resize(scrW, scrH);
                    render(win);
                }
                else
                {
                    windowID = SDL_GetWindowID(win);
                    if (e.window.windowID == windowID)
                    {
                        switch (e.window.event)
                        {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        {
                            scrW = e.window.data1;
                            scrH = e.window.data2;
                            glViewport(0, 0, scrW, scrH);
                            //hud->resize(scrW, scrH);
                            render(win);
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
        }
        else
        {
            // nessun evento: siamo IDLE

            Uint32 timeNow = SDL_GetTicks(); // che ore sono?

            if (timeLastInterval + fpsSampling < timeNow)
            {
                fps = static_cast<float>(1000.0 * ((float)fpsNow) / (timeNow - timeLastInterval));
                fpsNow = 0;
                timeLastInterval = timeNow;
            }

            bool doneSomething = false;
            int guardia = 0; // sicurezza da loop infinito

            // finche' il tempo simulato e' rimasto indietro rispetto
            // al tempo reale...
            while (nstep * PHYS_SAMPLING_STEP < timeNow)
            {
                plane->DoStep();
                hud->update();
                for (auto c : collectables) c->update();
                nstep++;
                doneSomething = true;
                timeNow = SDL_GetTicks();
                if (guardia++ > 1000)
                {
                    done = true;
                    break;
                } // siamo troppo lenti!
            }

            if (doneSomething)
                render(win);
            //redraw();
            else
            {
                // tempo libero!!!
            }
        }
    }
    std::cout << "Uscito dal game loop." << std::endl;

    // *** CLEANING UP ****
    delete plane, terrain, sky, hud, camera, finishLine;
    for (auto c : collectables)
        delete c;
    collectables.clear();
    delete meteorShower;
    TTF_CloseFont(font);
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    std::cout << "Memoria e processi puliti." << std::endl;
    return (0);
}
