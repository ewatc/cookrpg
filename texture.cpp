//
//  texture.cpp
//  cookrpg
//
//  Created by Wayne Moorefield on 3/16/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#include "texture.h"

Texture::Texture(SDL_Renderer* renderer) :
    mRenderer(renderer),
    mTexture(nullptr)
{
}

Texture::~Texture()
{
    if (!unload()) {
        // error
    }
}

bool Texture::load(std::shared_ptr<Surface> surface)
{
    if (mRenderer != nullptr && surface != nullptr) {
        mTexture = SDL_CreateTextureFromSurface(mRenderer,
                                                surface->getSDLSurface());
        if (!mTexture) {
            // error
            return false;
        }
    
        mFilename = surface->getFilename();
        
        SDL_QueryTexture(mTexture, nullptr, nullptr, &mWidth, &mHeight);
        
        return true;
    }
    
    return false;
}

bool Texture::unload()
{
    if (mTexture) {
        
    }
    return true;
}

