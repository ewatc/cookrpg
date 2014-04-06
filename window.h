//
//  window.h
//  cookrpg
//
//  Created by Wayne Moorefield on 3/16/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#ifndef __cookrpg__window__
#define __cookrpg__window__

#include <memory>
#include <SDL2/SDL.h>
#include "textureinterface.h"
#include "surface.h"

class Window
{
public:
    virtual ~Window();
    
    static std::shared_ptr<Window> create();

    bool init();
    bool uninit();
    
    std::shared_ptr<TextureInterface> createTexture(std::shared_ptr<Surface> surface);
    
    void render(std::shared_ptr<TextureInterface> texture,
                const SDL_Rect *src,
                const SDL_Rect *dst);
    
    void render(std::shared_ptr<TextureInterface> texture,
                const SDL_Rect *src,
                const SDL_Rect *dst,
                SDL_RendererFlip flipFlags);
    
    void clear();
    
    void flip();
    
protected:
    Window();
    
private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    SDL_Surface* mPrimarySurface;
    
    bool mInitialized;
};

#endif /* defined(__cookrpg__window__) */
