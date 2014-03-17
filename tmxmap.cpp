#include "tmxmap.h"
#include "log.h"

TmxMap::TmxMap()
{

}

TmxMap::~TmxMap()
{

}

bool TmxMap::loadMap(const std::string& filename)
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

    // Iterate through the layers
    for (int i=0; i<map->GetNumLayers(); ++i) {
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

    // success
    mTmxMap = std::move(map);

    return true;
}

bool TmxMap::unloadMap()
{
    mTmxMap = nullptr;

    return true;
}

bool TmxMap::renderLayer(std::shared_ptr<Window> window, int layer)
{
    return true;
}
