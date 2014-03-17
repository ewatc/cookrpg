#include "cookrpggame.h"
#include "log.h"

CookRPGGame::CookRPGGame() :
    mWindow(nullptr)
{
}

CookRPGGame::~CookRPGGame()
{
    if (!unloadResources()) {
        // error
    }
    
    if (!shutdown()) {
        // error
    }
}

bool CookRPGGame::initialize(std::shared_ptr<Window> wnd)
{
    if (wnd == nullptr) {
        // error
        return false;
    }
    
    // TODO pass in options
    if (!wnd->init()) {
        // error
        return false;
    }
    
    mWindow = wnd;
    
    return true;
}

bool CookRPGGame::shutdown()
{
    if (mWindow != nullptr) {
        if (!mWindow->uninit()) {
            // error
        }
    }
    
    return true;
}

bool CookRPGGame::loadResources()
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

bool CookRPGGame::onRender()
{
    // Clear previous frame
    mWindow->clear();

    // Render Map
    // TODO multiple layers
    mMap.renderLayer(mWindow, 0);

    // Render current frame
    mWindow->flip();

    return true;
}

bool CookRPGGame::onLoopEnd()
{
    return true;
}
