#ifndef _TMXMAP_H
#define _TMXMAP_H

#include <memory>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "tmxparser/Tmx.h"
#include "window.h"

class TmxMap
{
public:
    TmxMap();
    ~TmxMap();

    bool loadMap(std::shared_ptr<Window> window, const std::string& filename);

    bool unloadMap();

    bool renderLayer(std::shared_ptr<Window> window, unsigned int layer);
    
    int layers() const { return mLayers; }

private:
    std::unique_ptr<Tmx::Map> mTmxMap;
    int mLayers;
    
    std::vector<std::shared_ptr<TextureInterface>> mTilesetTexture;
};

#endif
