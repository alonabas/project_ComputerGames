//
//  Torch.h
//  project
//
//  Creates torch as cursor
//
//  Created by Alona Bas on 9/23/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Torch__
#define __project__Torch__

#include <iostream>
#include "Defs.h"
#include "Camera.h"
#include "ShaderIO.h"
#include "MeshObject.h"
#include "Flame.h"
#include "Globals.h"

#define LIGHT 0
#define REG 1

class Torch{
    static Torch* _torch;
    GLint _program;
    GLint _LightPositionUniform[3], _LightColorUniform[3];
    Flame* _flame;
    MeshObject* _object;
    vec2 _globalPos;
    bool _mode;
    Torch();
public:
    static Torch * instance(){
        if(_torch == NULL){
            _torch = new Torch();
        }
        return _torch;
    }
    virtual ~Torch();
    void init();
    void draw();
    void update(int globalX, int globalY);
    vec3 getPosition();
    void switchMode();
};

#endif /* defined(__project__Torch__) */
