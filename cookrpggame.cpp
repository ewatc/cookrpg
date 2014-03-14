#include "cookrpggame.h"
#include "log.h"

CookRPGGame::CookRPGGame()
{
}

CookRPGGame::~CookRPGGame()
{
}

bool CookRPGGame::loadResources(SDL_Renderer* renderer)
{
    if (!mMap.loadMap("example.tmx")) {
        Log(LOG_ERROR, "unable to load %s map", "example.tmx");
    }

    return true;
}

bool CookRPGGame::unloadResources()
{
    if (!mMap.unloadMap()) {
        Log(LOG_ERROR, "unable to unload map");
    }

    return true;
}

bool CookRPGGame::onLoopBegin()
{
    return true;
}

bool CookRPGGame::onEvent(const SDL_Event& event)
{
    return true;
}

bool CookRPGGame::onSimulation()
{
    return true;
}

bool CookRPGGame::onRender(SDL_Renderer* renderer)
{
    // Clear previous frame
    SDL_RenderClear(renderer);

    // Render Map
    // TODO multiple layers
    mMap.renderLayer(renderer, 0);

    // Render current frame
    SDL_RenderPresent(renderer);

    return true;
}

bool CookRPGGame::onLoopEnd()
{
    return true;
}
