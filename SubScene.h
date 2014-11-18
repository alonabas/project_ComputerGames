//
//  SubScene.h
//  project
//
//  Image of flags scene in main scene
//  Created by Alona Bas on 9/15/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__SubScene__
#define __project__SubScene__

#define SUB_POINTS_HIGH 2
#define SUB_POINTS_WIDTH 2
#define SUB_COUNT SUB_POINTS_HIGH*SUB_POINTS_WIDTH
#define SUB_INDX(x,y) ((x * SUB_POINTS_WIDTH) + y)

#include <iostream>
#include "Defs.h"
#include "Camera.h"
#include "ShaderIO.h"
#include "BoundingSphere.h"
#include "Object.h"
#include "Globals.h"
#include "Torch.h"

#define SUBSCENE_SCALE_FACTOR_X (float)(0.15)
#define SUBSCENE_SCALE_FACTOR_Y (float)(0.105)

class SubScene : public Object{
    GLuint _vao, _vbo, _ebo, _vboUV, _textureBuffer;
    GLint _program;
    GLint _LightPositionUniform, _LightColorUniform;
	GLuint _textureUniform, _Muniform, _Vuniform, _Puniform;
    GLint _attrib[2];
    size_t _size;
public:
    SubScene();
    virtual ~SubScene();
    void init(vec3 position);
    void draw();
};

#endif /* defined(__project__Subscene__) */
