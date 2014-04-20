//
//  window.cpp
//  cookrpg
//
//  Created by Wayne Moorefield on 3/16/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#include "window.h"
#include "log.h"
#include "statictexture.h"
#include "rendertexture.h"

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
    
    mWidth = 640;
    mHeight = 480;
    
    // TODO: Retrieve from Application Options
    mWindow = SDL_CreateWindow("Cook RPG",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               mWidth,
                               mHeight,
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
    
    const int windowPixelFormat = SDL_GetWindowPixelFormat(mWindow);
    Log(LOG_DEBUG, "Window pixel format %s",
        SDL_GetPixelFormatName(windowPixelFormat));
    
    const int displayIndex = SDL_GetWindowDisplayIndex(mWindow);
    Log(LOG_DEBUG, "Window display index %d",
        displayIndex);
    
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
        std::shared_ptr<StaticTexture> newtex(new StaticTexture(mRenderer));
        
        if (newtex != nullptr) {
            if (!newtex->create(surface)) {
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

std::shared_ptr<TextureInterface> Window::createTexture(const std::string &name,
                                                        int width,
                                                        int height,
                                                        int pixelFormat)
{
    std::shared_ptr<TextureInterface> tex;
    TextureMap::iterator it;
    
    it = mTextures.find(name);
    if (it == mTextures.end()) {
        std::shared_ptr<RenderTexture> newtex(new RenderTexture(mRenderer));
        
        if (newtex != nullptr) {
            if (!newtex->create(name, width, height, pixelFormat)) {
                // error
                tex = nullptr;
            } else {
                // success
                tex = newtex;
                
                mTextures[name] = tex;
            }
        }
    }
    
    return tex;
}

int Window::getPixelFormat() const
{
    return SDL_GetWindowPixelFormat(mWindow);
}

void Window::pruneTextures()
{
    TextureMap::iterator it;
    
    for (it = mTextures.begin(); it != mTextures.end();) {
        if (it->second.use_count() == 1) {
            Log(LOG_DEBUG, "Texture[%s] removing texture",
                it->second->getName().c_str());
            
            mTextures.erase(it++);
        } else {
            Log(LOG_DEBUG, "Texture[%s] keeping texture use_cout=%ld",
                it->second->getName().c_str(),
                it->second.use_count());
            it++;
        }
    }
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

void Window::render(std::shared_ptr<TextureInterface> dstTexture,
                    std::shared_ptr<TextureInterface> srcTexture,
                    const SDL_Rect* src,
                    const SDL_Rect* dst)
{
    // dstTexture must have been create with SDL_TEXTUREACCESS_TARGET
    
    // Change render target
    SDL_SetRenderTarget(mRenderer, dstTexture->getSDLTexture());
    
    // render the texture
    SDL_RenderCopy(mRenderer,
                   srcTexture->getSDLTexture(),
                   src,
                   dst);
    
    // Change render target back to window
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void Window::render(std::shared_ptr<TextureInterface> dstTexture,
                    std::shared_ptr<TextureInterface> srcTexture,
                    const SDL_Rect* src,
                    const SDL_Rect* dst,
                    SDL_RendererFlip flipFlags)
{
    // Change render target
    SDL_SetRenderTarget(mRenderer, dstTexture->getSDLTexture());
    
    // render the texture
    SDL_RenderCopyEx(mRenderer,
                     srcTexture->getSDLTexture(),
                     src,
                     dst,
                     0,
                     nullptr,
                     flipFlags);
    
    // Change render target back to window
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void Window::clear()
{
    SDL_RenderClear(mRenderer);
}

void Window::clear(std::shared_ptr<TextureInterface> texture)
{
    // Change render target
    SDL_SetRenderTarget(mRenderer, texture->getSDLTexture());
    
    // TODO fix this
    SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
    
    // clear the texture
    SDL_RenderClear(mRenderer);
    
    // Change render target back to window
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void Window::flip()
{
    SDL_RenderPresent(mRenderer);
}
