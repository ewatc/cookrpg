#ifndef _GAMEINTERFACE_H
#define _GAMEINTERFACE_H

#include <SDL2/SDL.h>
#include "window.h"

class GameInterface
{
public:
    virtual ~GameInterface() {}

    virtual bool initialize(std::shared_ptr<Window> wnd) = 0;
    
    virtual bool shutdown() = 0;
    
    virtual bool loadResources() = 0;

    virtual bool unloadResources() = 0;

    virtual bool onLoopBegin() = 0;

    virtual bool onEvent(const SDL_Event& event) = 0;

    virtual bool onSimulation() = 0;

    virtual bool onRender() = 0;

    virtual bool onLoopEnd() = 0;

protected:
    GameInterface() {}
};

#endif
