#ifndef _GAMEINTERFACE_H
#define _GAMEINTERFACE_H

#include <SDL2/SDL.h>

class GameInterface
{
public:
    virtual ~GameInterface() {}

    virtual bool loadResources(SDL_Renderer* renderer) = 0;

    virtual bool unloadResources() = 0;

    virtual bool onLoopBegin() = 0;

    virtual bool onEvent(const SDL_Event& event) = 0;

    virtual bool onSimulation() = 0;

    virtual bool onRender(SDL_Renderer* renderer) = 0;

    virtual bool onLoopEnd() = 0;

protected:
    GameInterface() {}
};

#endif
