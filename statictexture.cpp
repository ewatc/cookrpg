//
//  statictexture.cpp
//  cookrpg
//
//  Created by Wayne Moorefield on 4/19/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#include "statictexture.h"

StaticTexture::StaticTexture(SDL_Renderer* renderer) :
    mWidth(0),
    mHeight(0),
    mRenderer(renderer),
    mTexture(nullptr)
{
}

StaticTexture::~StaticTexture()
{
    if (!destroy()) {
        // error
    }
}

bool StaticTexture::create(std::shared_ptr<Surface> surface)
{
    if (mRenderer != nullptr && surface != nullptr) {
        mTexture = SDL_CreateTextureFromSurface(mRenderer,
                                                surface->getSDLSurface());
        if (!mTexture) {
            // error
            return false;
        }
        
        mName = surface->getFilename();
        
        SDL_QueryTexture(mTexture, nullptr, nullptr, &mWidth, &mHeight);
        
        return true;
    }
    
    return false;
}

bool StaticTexture::destroy()
{
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
    
    return true;
}


