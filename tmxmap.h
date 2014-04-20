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
    
    int layerCount() const { return mLayerCount; }

private:
    void generateLayerTexture(std::shared_ptr<Window> window,
                              std::shared_ptr<TextureInterface> layerTexture,
                              unsigned int layer);
    
    std::unique_ptr<Tmx::Map> mTmxMap;
    int mLayerCount;
    
    std::vector<std::shared_ptr<TextureInterface>> mTilesetTexture;
    std::vector<std::shared_ptr<TextureInterface>> mLayers;
};

#endif
