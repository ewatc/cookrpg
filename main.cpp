#include <stdio.h>
#include "sdl2app.h"
#include "log.h"
#include "cookrpggame.h"

int main(int argc, char **argv)
{
    std::shared_ptr<SDL2Application> app = SDL2Application::create(argc, argv);
    if (app == nullptr) {
        Log(LOG_ERROR, "Unable to create SDL2 application");
        return -1;
    }

    CookRPGGame game;

    Log(LOG_INFO, "running game");

    app->runGame(&game);

    Log(LOG_INFO, "main exiting");

	return 0;
}
