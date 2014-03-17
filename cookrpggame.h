#ifndef _COOKRPGGAME_H
#define _COOKRPGGAME_H

#include "gameinterface.h"
#include "tmxmap.h"

class CookRPGGame : public GameInterface
{
public:
    CookRPGGame();
    ~CookRPGGame();

    virtual bool initialize(std::shared_ptr<Window> wnd);
    
    virtual bool shutdown();
    
    virtual bool loadResources();

    virtual bool unloadResources();

    virtual bool onLoopBegin();

    virtual bool onEvent(const SDL_Event& event);

    virtual bool onSimulation();

    virtual bool onRender();

    virtual bool onLoopEnd();

private:
    TmxMap mMap;
    std::shared_ptr<Window> mWindow;
};

#endif
