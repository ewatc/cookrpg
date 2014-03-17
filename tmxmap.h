#ifndef _TMXMAP_H
#define _TMXMAP_H

#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include "tmxparser/Tmx.h"
#include "window.h"

class TmxMap
{
public:
    TmxMap();
    ~TmxMap();

    bool loadMap(const std::string& filename);

    bool unloadMap();

    bool renderLayer(std::shared_ptr<Window> window, int layer);

private:
    std::unique_ptr<Tmx::Map> mTmxMap;
};

#endif
