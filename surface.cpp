//
//  surface.cpp
//  cookrpg
//
//  Created by Wayne Moorefield on 3/16/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#include <SDL2/SDL_image.h>
#include "surface.h"
#include "log.h"

std::shared_ptr<Surface> Surface::create(std::string filename)
{
    std::shared_ptr<Surface> surf(new Surface(filename));

    return surf;
}

Surface::Surface(std::string filename) :
    mFilename(filename),
    mSurface(nullptr)
{
}

Surface::~Surface()
{
    if (mSurface != nullptr) {
        SDL_FreeSurface(mSurface);
        mSurface = nullptr;
    }
}

bool Surface::load()
{
    mSurface = IMG_Load(mFilename.c_str());
    if (mSurface == nullptr) {
        Log(LOG_ERROR, "Unable to load image [%s] error: %s",
            mFilename.c_str(), IMG_GetError());

        return false;
    } else {
        return true;
    }
}

