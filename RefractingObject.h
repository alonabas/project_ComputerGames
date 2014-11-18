//
//  RefractingObject.h
//  project
//
//  Class that defines Refracting (glass) object
//
//  Created by Alona Bas on 9/15/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__RefractingObject__
#define __project__RefractingObject__

#include <iostream>
#include "Defs.h"
#include "ShaderIO.h"
#include "Camera.h"
#include "bimage.h"
#include "MeshObject.h"
#include "BoundingSphere.h"
#include "Object.h"
#include "Globals.h"
#include "Torch.h"

class RefractingObject : public Object{
    const char* _textureFiles[NUMBER_SKYBOX_TEXTURES] = {
        (char*) "skybox/a_stitch_Left.bmp",
        (char*) "skybox/a_stitch_Right.bmp",
        (char*) "skybox/a_stitch_Bottom.bmp",
        (char*) "skybox/a_stitch_Top.bmp",
        (char*) "skybox/a_stitch_Back.bmp",
        (char*) "skybox/a_stitch_Front.bmp" };
    GLuint _textureBuffer;
    GLint _program;
	GLuint _textureUniform;
    GLint _LightPositionUniform[3], _LightColorUniform[3];
    MeshObject* _mesh;
public:
    RefractingObject();
    virtual ~RefractingObject();
    void init(vec3 center);
    void draw();
};

#endif /* defined(__project__RefractingObject__) */
