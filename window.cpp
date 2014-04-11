//
//  window.cpp
//  cookrpg
//
//  Created by Wayne Moorefield on 3/16/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#include "window.h"
#include "log.h"
#include "texture.h"

std::shared_ptr<Window> Window::create()
{
    std::shared_ptr<Window> wnd(new Window());
    
    return wnd;
}

Window::Window() :
    mInitialized(false)
{
    
}

Window::~Window()
{
    if (!uninit()) {
        // error
    }
}

bool Window::init()
{
    if (mInitialized) {
        return true;
    }
    
    Log(LOG_DEBUG, "Creating window");
    
    // TODO: Retrieve from Application Options
    mWindow = SDL_CreateWindow("Cook RPG",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               640,
                               480,
                               SDL_WINDOW_SHOWN);
    if (mWindow == nullptr) {
        Log(LOG_ERROR, "SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }
    
    mPrimarySurface = SDL_GetWindowSurface(mWindow);
    if (mPrimarySurface == nullptr) {
        Log(LOG_ERROR, "SDL_GetWindowSurface failed: %s", SDL_GetError());
        return false;
    }
    
    // OS X already has a renderer attached to the Window
    // and unable to create a new one.
    mRenderer = SDL_GetRenderer(mWindow);
    if (mRenderer == nullptr) {
        mRenderer = SDL_CreateRenderer(mWindow,
                                       -1,
                                       SDL_RENDERER_ACCELERATED);
        if (mRenderer == nullptr) {
            Log(LOG_ERROR, "SDL_CreateRenderer failed: %s", SDL_GetError());
            return false;
        }
    }
    
    SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
    
    mInitialized = true;
    
    return true;
}

bool Window::uninit()
{
    if (mRenderer) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
    
    if (mWindow) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    
    mInitialized = false;
    
    return true;
}

std::shared_ptr<TextureInterface> Window::createTexture(std::shared_ptr<Surface> surface)
{
    std::shared_ptr<TextureInterface> tex;
    TextureMap::iterator it;
    
    it = mTextures.find(surface->getFilename());
    if (it == mTextures.end()) {
        std::shared_ptr<Texture> newtex(new Texture(mRenderer));
        
        if (newtex != nullptr) {
            if (!newtex->load(surface)) {
                // error
                tex = nullptr;
            } else {
                // success
                tex = newtex;
                
                mTextures[surface->getFilename()] = tex;
            }
        }
    } else {
        // texture found
        tex = it->second;
    }
    
    return tex;
}

void Window::render(std::shared_ptr<TextureInterface> texture,
                    const SDL_Rect *src,
                    const SDL_Rect *dst)
{
    SDL_RenderCopy(mRenderer, texture->getSDLTexture(), src, dst);
}

void Window::render(std::shared_ptr<TextureInterface> texture,
                    const SDL_Rect *src,
                    const SDL_Rect *dst,
                    SDL_RendererFlip flipFlags)
{
    SDL_RenderCopyEx(mRenderer,
                     texture->getSDLTexture(),
                     src,
                     dst,
                     0,
                     nullptr,
                     flipFlags);
}

void Window::clear()
{
    SDL_RenderClear(mRenderer);
}

void Window::flip()
{
    SDL_RenderPresent(mRenderer);
}
