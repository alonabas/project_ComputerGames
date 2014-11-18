//
//  Statue.h
//  project
//
//  Create statue
//
//  Created by Alona Bas on 9/18/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Statue__
#define __project__Statue__

#include <iostream>
#include "Defs.h"
#include "ShaderIO.h"
#include "MeshObject.h"
#include "BoundingSphere.h"
#include "Object.h"
#include "Globals.h"
#include "Torch.h"

class Statue : public Object{
    GLint _program;
    MeshObject* _mesh;
    GLint _LightPositionUniform[3], _LightColorUniform[3];
public:
    Statue();
    virtual ~Statue();
    void init(vec3 center);
    void draw();
};

#endif /* defined(__project__Statue__) */
