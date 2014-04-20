//
//  sprite.h
//  cookrpg
//
//  Created by Wayne Moorefield on 4/11/14.
//  Copyright (c) 2014 East Wake Academy Tech Club. All rights reserved.
//

#ifndef __cookrpg__sprite__
#define __cookrpg__sprite__

#include <string>

class Sprite
{
public:
    Sprite();
    virtual ~Sprite();
    
    
    
    void getAnchor(int &x, int &y) const;
    void setAnchor(int x, int y);
    
    unsigned int getWidth() const;
    unsigned int getHeight() const;
};

#endif /* defined(__cookrpg__sprite__) */
