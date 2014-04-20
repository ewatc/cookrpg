//
//  rendertexture.cpp
//  cookrpg
//
//  Created by Wayne Moorefield on 4/19/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#include "rendertexture.h"

RenderTexture::RenderTexture(SDL_Renderer* renderer) :
    mWidth(0),
    mHeight(0),
    mRenderer(renderer),
    mTexture(nullptr)
{
}

RenderTexture::~RenderTexture()
{
    if (!destroy()) {
        // error
    }
}

bool RenderTexture::create(const std::string& name, int width, int height, int format)
{
    if (mRenderer != nullptr) {
        mTexture = SDL_CreateTexture(mRenderer,
                                     format,
                                     SDL_TEXTUREACCESS_TARGET,
                                     width,
                                     height);
        if (!mTexture) {
            // error
            return false;
        }
        
        mName = name;
        
        // TODO: For now always assume alpha blend
        SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
        
        SDL_QueryTexture(mTexture, nullptr, nullptr, &mWidth, &mHeight);
        
        return true;
    }
    
    return false;
}

bool RenderTexture::destroy()
{
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
    
    return true;
}

