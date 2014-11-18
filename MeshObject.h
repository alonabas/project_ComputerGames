//
//  MeshObject.h
//  project
//
// Class that loads and draws the mesh file
//
//  Created by Alona Bas on 5/25/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__MeshObject__
#define __project__MeshObject__

#include <iostream>
#include "Defs.h"
#include "ShaderIO.h"
#include "Camera.h"
#include "TGALoader.h"
#include "bimage.h"


class MeshObject {
	GLuint _vao, _vbo, _ebo, _vboNormal;
    GLint _program;
	GLuint _MnormalUniform, _Muniform, _Vuniform, _Puniform;
	GLint _attrib[2];
    float _radius;
	size_t _numTriangles;
	mat4 _modelScale, _modelRotate, _modelTranslate, _moveToCenter;
	
public:
	MeshObject();
	virtual ~MeshObject();
    void init(GLint program, const char* meshFile);
    void setRotation(mat4 matrix);
    void setTranslation(mat4 matrix);
    void setScale(mat4 matrix);
	void draw();
    float getRadius();

};

#endif /* defined(__project__MeshObject__) */
