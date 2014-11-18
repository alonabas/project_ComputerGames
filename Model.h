//
//  Model.h
//  project
//
//  Created by Alona Bas on 9/23/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project_temp__Model__
#define __project_temp__Model__

#include <iostream>

#define REGULAR 0
#define ANIMATION 1
#define ANIMATION_TIME 15
class Model
{
public:
    int _status;
    int _animation_count;
    Model(): _status(REGULAR),_animation_count(0){}
    virtual ~Model(){}
    virtual void init(){}
    virtual void draw(){}
    virtual bool isHit(int x, int y){return false;}
    virtual int getFound(){return -1;};
    virtual void remove(){}
    virtual int getStatus(){return _status;}
};

#endif /* defined(__project_temp__Model__) */
