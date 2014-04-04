//
//  textureinterface.h
//  cookrpg
//
//  Created by Wayne Moorefield on 3/16/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#ifndef cookrpg_textureinterface_h
#define cookrpg_textureinterface_h

#include <SDL2/SDL.h>
#include <string>

class TextureInterface
{
public:
    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;
    
    virtual SDL_Texture* getSDLTexture() = 0;
    
    virtual std::string getFilename() const = 0;
};


#endif
