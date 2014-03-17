//
//  texture.h
//  cookrpg
//
//  Created by Wayne Moorefield on 3/16/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#ifndef __cookrpg__texture__
#define __cookrpg__texture__


#include <SDL2/SDL.h>
#include <string>
#include "textureinterface.h"
#include "surface.h"

class Texture : public TextureInterface
{
public:
    Texture(SDL_Renderer* renderer);
    virtual ~Texture();
    
    bool load(std::shared_ptr<Surface> surface);
    bool unload();
    
    unsigned int getWidth() const { return mWidth; }
    unsigned int getHeight() const { return mHeight; }

    SDL_Texture* getSDLTexture() { return mTexture; }
    
private:
    int mWidth;
    int mHeight;
    
    SDL_Renderer *mRenderer;
    SDL_Texture* mTexture;
};

#endif /* defined(__cookrpg__texture__) */
