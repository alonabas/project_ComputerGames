//
//  Sky.h
//  project
//
//  Sky animation
//
//  Created by Alona Bas on 9/28/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Sky__
#define __project__Sky__

#define SKY_POINTS_HIGH 2
#define SKY_POINTS_WIDTH 2
#define SKY_COUNT SKY_POINTS_HIGH*SKY_POINTS_WIDTH
#define SKY_INDX(x,y) ((x * SKY_POINTS_WIDTH) + y)
#define SKY_SCALE_FACTOR_X (float)(2.0f)
#define SKY_SCALE_FACTOR_Y (float)(2.0f)


#include "ShaderIO.h"
#include <iostream>
#include "Defs.h"
#include "Camera.h"

class Sky{
    int _time;
    GLuint _vao, _vbo, _ebo, _vboUV;
    GLint _program;
	GLuint _timeUniform, _resolutionUniform, _Muniform, _Vuniform, _Puniform;
    GLint _attrib[2];
    size_t _size;
    mat4 _modelMat;
public:
    Sky();
    virtual ~Sky();
    void init();
    void draw();
};


#endif /* defined(__project__Sky__) */
