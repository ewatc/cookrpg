#include "sdl2app.h"
#include "log.h"
#include "window.h"

//#define PERFORMANCE_CHECKING

#ifdef PERFORMANCE_CHECKING
#define PERFCHECK_BEGIN() \
    do {   \
        unsigned int performanceTicks = SDL_GetTicks()
#define PERFCHECK_END(tag) \
        Log(LOG_DEBUG, "%s took %dms",  \
            tag, SDL_GetTicks() - performanceTicks); \
    } while(0)
#else
#define PERFCHECK_BEGIN()
#define PERFCHECK_END(tag)
#endif

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
    mWindow(nullptr)
{
    mUpdateCount = 0;
    mLastFPSTicks = 0;
    mTicksPerUpdate = 1000.0f / (float)30;
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

    mWindow = Window::create();
    
    return true;
}

bool SDL2Application::shutdown()
{
    mWindow = nullptr;

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

    if (!game->initialize(mWindow)) {
        Log(LOG_ERROR, "Unable to initialize game");
        return;
    }
    
    if (!game->loadResources()) {
        Log(LOG_ERROR, "Unable to load game resources");
        return;
    }

    while (keepRunning) {
        unsigned int tempTick = SDL_GetTicks();
        unsigned int elapsedTicks;
        
        elapsedTicks = tempTick - mLastTicks;
        mLastTicks = elapsedTicks;
        
        // begin loop
        PERFCHECK_BEGIN();
        if (!game->onLoopBegin()) {
            keepRunning = false;
        }
        PERFCHECK_END("onLoopBegin");

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
        PERFCHECK_BEGIN();
        if (!game->onSimulation()) {
            keepRunning = false;
        }
        PERFCHECK_END("onSimulation");

        // do rendering
        PERFCHECK_BEGIN();
        if (!game->onRender()) {
            keepRunning = false;
        }
        PERFCHECK_END("onRender");

        // end loop
        PERFCHECK_BEGIN();
        if (!game->onLoopEnd()) {
            keepRunning = false;
        }
        PERFCHECK_END("onLoopEnd");
        
        // Handle overall rate limiter
        {
            unsigned int nextUpdate;
            unsigned int currTick;
            
            ++mUpdateCount;
            
            currTick = SDL_GetTicks();
            nextUpdate = mLastFPSTicks + ((float)mUpdateCount + mTicksPerUpdate);
            
            if (currTick <= nextUpdate) {
                const unsigned int ticks = nextUpdate - currTick;
                SDL_Delay(ticks);
            } else {
                mUpdateCount = 0;
                mLastFPSTicks = SDL_GetTicks();
            }
        }
    }

    if (!game->unloadResources()) {
        Log(LOG_ERROR, "Unable to unload game resources");
        return;
    }
    
    if (!game->shutdown()) {
        Log(LOG_ERROR, "Unable to shutdown game");
        return;
    }
}
