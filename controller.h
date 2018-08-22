#ifndef PROJGRAF_CONTROLLER_H
#define PROJGRAF_CONTROLLER_H

#include "plane.h"

class Command
{
    public:
    virtual void Execute(Plane *plane, bool button_down) = 0;
};

class GoForwardCommand : public Command
{
    public:
    void Execute(Plane *plane, bool button_down)
    {
        plane->goForward = button_down;
    }
};

class GoBackCommand : public Command
{
    void Execute(Plane *plane, bool button_down)
    {
        plane->goBack = button_down;
    }
};

class GoLeftCommand : public Command
{
    void Execute(Plane *plane, bool button_down)
    {
        plane->goLeft = button_down;
    }
};

class GoRightCommand : public Command
{
    void Execute(Plane *plane, bool button_down)
    {
        plane->goRight = button_down;
    }
};

class Controller
{
  private:
    bool pressingW;
    Command *buttonW;
    Command *buttonS;
    Command *buttonA;
    Command *buttonD;

  public:
    void handleInputs(Plane *plane, SDL_Event &e, bool button_down);

    Controller();

    virtual ~Controller();
};

#endif //PROJGRAF_CONTROLLER_H
