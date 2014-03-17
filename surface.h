//
//  surface.h
//  cookrpg
//
//  Created by Wayne Moorefield on 3/16/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#ifndef __cookrpg__surface__
#define __cookrpg__surface__

#include <SDL2/SDL.h>
#include <string>
#include <memory>

class Surface
{
public:
    virtual ~Surface();
    
    static std::shared_ptr<Surface> create(std::string filename);
    
    bool load();

    std::string getFilename() const { return mFilename; }
    
    SDL_Surface* getSDLSurface() { return mSurface; }
    
protected:
    Surface(std::string filename);

private:
    std::string mFilename;
    SDL_Surface* mSurface;
};

#endif /* defined(__cookrpg__surface__) */
