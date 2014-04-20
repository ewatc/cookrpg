#include "tmxmap.h"
#include "log.h"

TmxMap::TmxMap() : mLayerCount(0)
{

}

TmxMap::~TmxMap()
{

}

bool TmxMap::loadMap(std::shared_ptr<Window> window, const std::string& filename)
{
    std::unique_ptr<Tmx::Map> map(new Tmx::Map());

    // Open and parse map file
    map->ParseFile(filename.c_str());
    if (map->HasError()) {
        Log(LOG_ERROR, "Unable to parse map");
        return false;
    }

    // Iterate through the tilesets
    for (int i=0; i<map->GetNumTilesets(); ++i) {
        Log(LOG_DEBUG, "Tileset: %d", i);

        // Get ptr to tileset
        const Tmx::Tileset* tileset = map->GetTileset(i);

        // Print tileset info
        Log(LOG_DEBUG, "\tName: %s", tileset->GetName().c_str());
        Log(LOG_DEBUG, "\tMargin: %d", tileset->GetMargin());
        Log(LOG_DEBUG, "\tSpacing: %d", tileset->GetSpacing());
        Log(LOG_DEBUG, "\tTile Width: %d", tileset->GetTileWidth());
        Log(LOG_DEBUG, "\tTile Height: %d", tileset->GetTileHeight());
        Log(LOG_DEBUG, "\tImage Width: %d", tileset->GetImage()->GetWidth());
        Log(LOG_DEBUG, "\tImage Height: %d", tileset->GetImage()->GetHeight());
        Log(LOG_DEBUG, "\tImage Source: %s", tileset->GetImage()->GetSource().c_str());
        Log(LOG_DEBUG, "\tTransparent Color (hex): %s", tileset->GetImage()->GetTransparentColor().c_str());

        std::shared_ptr<TextureInterface> texture;
        std::shared_ptr<Surface> surface = Surface::create(tileset->GetImage()->GetSource());
        if (!surface->load()) {
            Log(LOG_ERROR, "Unable to load surface");
            return false;
        } else {
            texture = window->createTexture(surface);
            if (texture == nullptr) {
                Log(LOG_ERROR, "Unable to load texture [%s]", surface->getFilename().c_str());
                return false;
            }
        }
        
        mTilesetTexture.push_back(texture);
        
        if (tileset->GetTiles().size() > 0) {
            // Get a tile from the tileset
            const Tmx::Tile *tile = *(tileset->GetTiles().begin());

            Log(LOG_DEBUG, "\tProperties:");

            // Print the properties of a tile
            std::map< std::string, std::string > list = tile->GetProperties().GetList();
            std::map< std::string, std::string >::iterator iter;
            for (iter = list.begin(); iter != list.end(); ++iter) {
                Log(LOG_DEBUG, "\t\t%s = %s", iter->first.c_str(), iter->second.c_str());
            }
        }
    }

    // success
    mTmxMap = std::move(map);
    mLayerCount = static_cast<int>(mTmxMap->GetNumLayers());
    
    // Iterate through layers
    for (int i=0; i<mTmxMap->GetNumLayers(); ++i) {
        std::shared_ptr<TextureInterface> texture;
        const int width = mTmxMap->GetWidth() * mTmxMap->GetTileWidth();
        const int height = mTmxMap->GetHeight() * mTmxMap->GetTileHeight();
        
        // TODO: Make this better
        std::string temp = filename + " Layer" + (char)('0' + i);
        texture = window->createTexture(temp,
                                        width,
                                        height,
                                        SDL_PIXELFORMAT_ARGB8888);
        if (texture) {
            Log(LOG_DEBUG, "Created Layer %s - %dx%d",
                temp.c_str(), width, height);
        
            generateLayerTexture(window, texture, i);
            
            mLayers.push_back(texture);
        } else {
            Log(LOG_ERROR, "Unable to create layer texture %dx%d - %s",
                width, height,
                SDL_GetError());
        }
    }
    
    return true;
}

bool TmxMap::unloadMap()
{
    mTmxMap = nullptr;
    mTilesetTexture.clear();
    mLayers.clear();

    return true;
}

void TmxMap::generateLayerTexture(std::shared_ptr<Window> window,
                                  std::shared_ptr<TextureInterface> layerTexture,
                                  unsigned int layer)
{
    if (layer >= mTmxMap->GetNumLayers()) {
        // nothing to draw
        return;
    }
    
    window->clear(layerTexture);
    
    const Tmx::Layer* tmxLayer = mTmxMap->GetLayer(layer);
    
    const int maxTileY = mTmxMap->GetHeight();
    const int maxTileX = mTmxMap->GetWidth();
    
    // This is not optimized, but it gets the job done
    for (int tileY=0; tileY<maxTileY; ++tileY) {
        for (int tileX=0; tileX<maxTileX; ++tileX) {
            Tmx::MapTile tile = tmxLayer->GetTile(tileX, tileY);
            
            if (tile.id == 0) {
                continue;
            }
            
            if (tile.tilesetId < mTmxMap->GetNumTilesets()) {
                const Tmx::Tileset* tileset;
                
                tileset = mTmxMap->GetTileset(tile.tilesetId);
                if (tile.id >= tileset->GetFirstGid()) {
                    // Found the right tileset
                    const int id = tile.id - tileset->GetFirstGid() + 1;
                    const int tilesetWidth = tileset->GetImage()->GetWidth();
                    const int tileWidth = tileset->GetTileWidth();
                    const int tileHeight = tileset->GetTileHeight();
                    const int numColumns = tilesetWidth / tileWidth;
                    
                    const int column = id % numColumns;
                    const int row = id / numColumns;
                    
                    SDL_Rect srcRect;
                    srcRect.x = tileset->GetMargin() +
                    (tileset->GetTileWidth() +
                     tileset->GetSpacing()) * column;
                    srcRect.y = tileset->GetMargin() +
                    (tileset->GetTileHeight() +
                     tileset->GetSpacing()) * row;
                    srcRect.w = tileset->GetTileWidth();
                    srcRect.h = tileset->GetTileHeight();
                    
                    SDL_Rect dstRect;
                    dstRect.x = tileX * tileWidth;
                    dstRect.y = tileY * tileHeight;
                    dstRect.w = tileWidth;
                    dstRect.h = tileHeight;
                    
                    SDL_RendererFlip hflip = SDL_FLIP_NONE;
                    SDL_RendererFlip vflip = SDL_FLIP_NONE;
                    if (tmxLayer->IsTileFlippedHorizontally(tileX, tileY)) {
                        hflip = SDL_FLIP_HORIZONTAL;
                    }
                    
                    if (tmxLayer->IsTileFlippedVertically(tileX, tileY)) {
                        vflip = SDL_FLIP_VERTICAL;
                    }
                    
                    // Render the tile, the last parameter has
                    // to be casted due to the enum not having
                    // a value for diagonal.
                    window->render(layerTexture,
                                   mTilesetTexture[0],
                                   &srcRect,
                                   &dstRect,
                                   static_cast<SDL_RendererFlip>(hflip | vflip));
                }
            }
        }
    }
}

bool TmxMap::renderLayer(std::shared_ptr<Window> window, unsigned int layer)
{
    if (layer >= mTmxMap->GetNumLayers()) {
        // nothing to draw
        return true;
    }
    
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = window->getWidth();
    srcRect.h = window->getHeight();
    
    SDL_Rect dstRect;
    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = window->getWidth();
    dstRect.h = window->getHeight();
    
    window->render(mLayers[layer],
                   &srcRect,
                   &dstRect);
    
#if 0
    const Tmx::Layer* tmxLayer = mTmxMap->GetLayer(layer);
    
    const int maxTileY = mTmxMap->GetHeight();
    const int maxTileX = mTmxMap->GetWidth();
    
    // This is not optimized, but it gets the job done
    for (int tileY=0; tileY<maxTileY; ++tileY) {
        for (int tileX=0; tileX<maxTileX; ++tileX) {
            Tmx::MapTile tile = tmxLayer->GetTile(tileX, tileY);
            
            if (tile.id == 0) {
                continue;
            }
            
            if (tile.tilesetId < mTmxMap->GetNumTilesets()) {
                const Tmx::Tileset* tileset;
                
                tileset = mTmxMap->GetTileset(tile.tilesetId);
                if (tile.id >= tileset->GetFirstGid()) {
                    // Found the right tileset
                    const int id = tile.id - tileset->GetFirstGid() + 1;
                    const int tilesetWidth = tileset->GetImage()->GetWidth();
                    const int tileWidth = tileset->GetTileWidth();
                    const int tileHeight = tileset->GetTileHeight();
                    const int numColumns = tilesetWidth / tileWidth;
                    
                    const int column = id % numColumns;
                    const int row = id / numColumns;
                    
                    SDL_Rect srcRect;
                    srcRect.x = tileset->GetMargin() +
                                (tileset->GetTileWidth() +
                                tileset->GetSpacing()) * column;
                    srcRect.y = tileset->GetMargin() +
                                (tileset->GetTileHeight() +
                                tileset->GetSpacing()) * row;
                    srcRect.w = tileset->GetTileWidth();
                    srcRect.h = tileset->GetTileHeight();
                    
                    SDL_Rect dstRect;
                    dstRect.x = tileX * tileWidth;
                    dstRect.y = tileY * tileHeight;
                    dstRect.w = tileWidth;
                    dstRect.h = tileHeight;
                    
                    SDL_RendererFlip hflip = SDL_FLIP_NONE;
                    SDL_RendererFlip vflip = SDL_FLIP_NONE;
                    if (tmxLayer->IsTileFlippedHorizontally(tileX, tileY)) {
                        hflip = SDL_FLIP_HORIZONTAL;
                    }
                    
                    if (tmxLayer->IsTileFlippedVertically(tileX, tileY)) {
                        vflip = SDL_FLIP_VERTICAL;
                    }
                    
                    // Render the tile, the last parameter has
                    // to be casted due to the enum not having
                    // a value for diagonal.
                    window->render(mTilesetTexture[0],
                                   &srcRect,
                                   &dstRect,
                                   static_cast<SDL_RendererFlip>(hflip | vflip));
                }
            }
        }
    }
#endif
#if 0
    // Iterate through the layers
    for (int i=0; i<mTmxMap->GetNumLayers(); ++i) {
        std::string tileInfo;
        char temp[16];
        Log(LOG_DEBUG, "Layer: %d Name: %s", i, map->GetLayer(i)->GetName().c_str());
        
        // Get a layer
        const Tmx::Layer* layer = map->GetLayer(i);
        
        for (int y=0; y<layer->GetHeight(); ++y) {
            for (int x=0; x<layer->GetWidth(); ++x) {
                char flippedHorizontally = ' ';
                char flippedVertically = ' ';
                char flippedDiagonally = ' ';
                
                if (layer->IsTileFlippedHorizontally(x, y)) {
                    flippedHorizontally = 'h';
                }
                
                if (layer->IsTileFlippedVertically(x, y)) {
                    flippedVertically = 'v';
                }
                
                if (layer->IsTileFlippedDiagonally(x, y)) {
                    flippedDiagonally = 'd';
                }
                
                sprintf(temp, "%03d%c%c%c ",
                        layer->GetTileId(x, y),
                        flippedHorizontally,
                        flippedVertically,
                        flippedDiagonally);
                
                tileInfo += temp;
                
                // Find a tileset for that id
                const Tmx::Tileset* tileset = map->FindTileset(layer->GetTileId(x, y));
            }
            
            // Print out one row
            Log(LOG_DEBUG, "%s", tileInfo.c_str());
            tileInfo.clear();
        }
    }
#endif
    return true;
}
