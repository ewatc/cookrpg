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
    std::string tmxFile = "game_world.tmx";
    
    // Testing
    // TODO: Read this from a file
    //std::string tmxFile = "example.tmx";
    
    if (!mMap.loadMap(mWindow, tmxFile.c_str())) {
        Log(LOG_ERROR, "unable to load %s map", tmxFile.c_str());
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
    for (int i=0; i<mMap.layers(); ++i) {
        mMap.renderLayer(mWindow, i);
    }
    
    // Render current frame
    mWindow->flip();

    return true;
}

bool CookRPGGame::onLoopEnd()
{
    return true;
}
