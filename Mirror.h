//
//  Mirror.h
//  project
//
//  Class that dreates and displayes mirror
//
//  Created by Alona Bas on 9/15/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Mirror__
#define __project__Mirror__

#define MIRROR_POINTS_HIGH 2
#define MIRROR_POINTS_WIDTH 2
#define MIRROR_COUNT MIRROR_POINTS_HIGH*MIRROR_POINTS_WIDTH
#define MIRROR_INDX(x,y) ((x * MIRROR_POINTS_WIDTH) + y)

#include <iostream>
#include "MeshObject.h"
#include "Defs.h"
#include "BoundingSphere.h"
#include "Object.h"
#include "Globals.h"
#include "Torch.h"

#define MIRROR_SCALE_FACTOR_X (float)(0.175f)
#define MIRROR_SCALE_FACTOR_Y (float)(0.0917f)

class Mirror : public Object{
    const char* _textureFiles[NUMBER_SKYBOX_TEXTURES] = {
        (char*) "skybox/a_stitch_Left.bmp",
        (char*) "skybox/a_stitch_Right.bmp",
        (char*) "skybox/a_stitch_Bottom.bmp",
        (char*) "skybox/a_stitch_Top.bmp",
        (char*) "skybox/a_stitch_Back.bmp",
        (char*) "skybox/a_stitch_Front.bmp" };
    int _time;
    GLuint _vao, _vbo, _ebo, _vboNormal, _vboUV, _textureBuffer[2];
    GLint _program;
    GLint _LightPositionUniform[3], _LightColorUniform[3];
	GLuint _MnormalUniform, _textureUniform[2], _Muniform, _Vuniform, _Puniform,_typeUniform, _timeUniform;
    GLint _attrib[3];
    MeshObject* _frame;
    mat4 _moveToCenter;
    size_t _size;
public:
    Mirror();
    virtual ~Mirror();
    void init(vec3 center);
    void draw();
};

#endif /* defined(__project__Mirror__) */
