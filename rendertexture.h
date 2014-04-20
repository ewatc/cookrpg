//
//  rendertexture.h
//  cookrpg
//
//  Created by Wayne Moorefield on 4/19/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#ifndef __cookrpg__rendertexture__
#define __cookrpg__rendertexture__

#include <SDL2/SDL.h>
#include <string>
#include "textureinterface.h"
#include "surface.h"

class RenderTexture : public TextureInterface
{
public:
    RenderTexture(SDL_Renderer* renderer);
    virtual ~RenderTexture();
    
    bool create(const std::string& name, int width, int height, int format);
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


#endif /* defined(__cookrpg__rendertexture__) */
