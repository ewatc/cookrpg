//
//  statictexture.h
//  cookrpg
//
//  Created by Wayne Moorefield on 4/19/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#ifndef __cookrpg__statictexture__
#define __cookrpg__statictexture__

#include <SDL2/SDL.h>
#include <string>
#include "textureinterface.h"
#include "surface.h"

class StaticTexture : public TextureInterface
{
public:
    StaticTexture(SDL_Renderer* renderer);
    virtual ~StaticTexture();
    
    bool create(std::shared_ptr<Surface> surface);
    bool destroy();
    
    unsigned int getWidth() const { return mWidth; }
    unsigned int getHeight() const { return mHeight; }
    
    std::string getName() const { return mName; }
    
    SDL_Texture* getSDLTexture() { return mTexture; }
    
private:
    std::string mName;
    int mWidth;
    int mHeight;
    
    SDL_Renderer *mRenderer;
    SDL_Texture* mTexture;
};

#endif /* defined(__cookrpg__statictexture__) */
