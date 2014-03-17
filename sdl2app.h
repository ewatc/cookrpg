#ifndef _SDL2APP_H
#define _SDL2APP_H

#include <memory>
#include <SDL2/SDL.h>
#include "gameinterface.h"
#include "window.h"

class SDL2Application
{
public:
    ~SDL2Application();

    static std::shared_ptr<SDL2Application> create(int argc, char* argv[]);

    void runGame(GameInterface* game);

protected:
    bool init();
    bool shutdown();

    bool processCommandLineArgs(int argc, char* argv[]);

private:
    SDL2Application();

    std::shared_ptr<Window> mWindow;
};


#endif
