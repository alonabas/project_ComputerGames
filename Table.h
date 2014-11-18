//
//  Table.h
//  project
//
//  Draws table in flags scene
//
//  Created by Alona Bas on 9/18/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Table__
#define __project__Table__

#include <iostream>
#include "Defs.h"
#include "ShaderIO.h"
#include "MeshObject.h"
#include "Torch.h"
#include "Globals.h"

class Table{
    GLint _program;
    MeshObject* _obj;
    GLint _LightPositionUniform[3], _LightColorUniform[3];
public:
    Table();
    virtual ~Table();
    void init();
    void draw();
    
};

#endif /* defined(__project__Table__) */
