#include "sdl2app.h"
#include "log.h"

std::shared_ptr<SDL2Application> SDL2Application::create(int argc, char* argv[])
{
    std::shared_ptr<SDL2Application> app(new SDL2Application());
    if (app == nullptr) {
        Log(LOG_ERROR, "Unable to create SDL2 application");
        return nullptr;
    }

    if (!app->processCommandLineArgs(argc, argv)) {
        Log(LOG_ERROR, "Processing command line arguments error");
        return nullptr;
    }

    if (!app->init()) {
        Log(LOG_ERROR, "Unable to initialize SDL2 application");
        app = nullptr;
    }

    return app;
}

SDL2Application::SDL2Application() :
    mWindow(nullptr),
    mRenderer(nullptr),
    mPrimarySurface(nullptr)
{
}

SDL2Application::~SDL2Application()
{
    if (!shutdown()) {
        Log(LOG_ERROR, "Unable to shutdown SDL2 Application properly");
    }
}

bool SDL2Application::processCommandLineArgs(int argc, char* argv[])
{
    return true;
}

bool SDL2Application::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Log(LOG_ERROR, "SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        Log(LOG_ERROR, "SDL_SetHint failed: %s", SDL_GetError());
        return false;
    }

    // TODO: Retrieve from Application Options
    mWindow = SDL_CreateWindow("Cook RPG",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               640,
                               480,
                               SDL_WINDOW_SHOWN);
    if (mWindow == nullptr) {
        Log(LOG_ERROR, "SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    mPrimarySurface = SDL_GetWindowSurface(mWindow);
    if (mPrimarySurface == nullptr) {
        Log(LOG_ERROR, "SDL_GetWindowSurface failed: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow,
                                   -1,
                                   SDL_RENDERER_ACCELERATED);
    if (mRenderer == nullptr) {
        Log(LOG_ERROR, "SDL_CreateRenderer failed: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);

    return true;
}

bool SDL2Application::shutdown()
{
    if (mRenderer) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }

    if (mWindow) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    SDL_Quit();

    return true;
}

void SDL2Application::runGame(GameInterface* game)
{
    bool keepRunning = true;
    SDL_Event event;

    if (game == nullptr) {
        Log(LOG_ERROR, "Invalid arguments");
        return;
    }

    // TODO: initialize the game
    if (!game->loadResources(mRenderer)) {
        Log(LOG_ERROR, "Unable to load game resources");
        return;
    }

    while (keepRunning) {
        // begin loop
        if (!game->onLoopBegin()) {
            keepRunning = false;
        }

        // process events
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                keepRunning = false;
                break;
            }

            if (!game->onEvent(event)) {
                keepRunning = false;
            }
        }

        // do simulation
        if (!game->onSimulation()) {
            keepRunning = false;
        }

        // do rendering
        if (!game->onRender(mRenderer)) {
            keepRunning = false;
        }

        // end loop
        if (!game->onLoopEnd()) {
            keepRunning = false;
        }
    }

    if (!game->unloadResources()) {
        Log(LOG_ERROR, "Unable to unload game resources");
        return;
    }
}
