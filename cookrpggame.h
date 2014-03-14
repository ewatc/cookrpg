#ifndef _COOKRPGGAME_H
#define _COOKRPGGAME_H

#include "gameinterface.h"
#include "tmxmap.h"

class CookRPGGame : public GameInterface
{
public:
    CookRPGGame();
    ~CookRPGGame();

    virtual bool loadResources(SDL_Renderer* renderer);

    virtual bool unloadResources();

    virtual bool onLoopBegin();

    virtual bool onEvent(const SDL_Event& event);

    virtual bool onSimulation();

    virtual bool onRender(SDL_Renderer* renderer);

    virtual bool onLoopEnd();

private:
    TmxMap mMap;
};

#endif
