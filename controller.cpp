#include <iostream>
#include "controller.h"

Controller::Controller()
{
    buttonW = new GoForwardCommand();
    buttonS = new GoBackCommand();
    buttonA = new GoLeftCommand();
    buttonD = new GoRightCommand();
    pressingW = false;
}
void Controller::handleInputs(Plane *plane, SDL_Event &e, bool button_down)
{
    if (e.key.keysym.sym == 'w')
    {
       // if (!pressingW && button_down) {
            buttonW->Execute(plane, button_down);
         //   pressingW = true;
        //}
        //else if (!button_down) pressingW = false;
    }
    if (e.key.keysym.sym == 's')
    {
        buttonS->Execute(plane, button_down);
    }
    if (e.key.keysym.sym == 'd')
    {
        buttonD->Execute(plane, button_down);
    }
    if (e.key.keysym.sym == 'a')
    {/* if (button_down) std::cout << "LEFT PREMUTO" << std::endl;
        else std::cout << "LEFT RILASCIATO" << std::endl;*/
        buttonA->Execute(plane, button_down);
    }
}

Controller::~Controller()
{
    delete buttonA, buttonD, buttonS, buttonW;
}