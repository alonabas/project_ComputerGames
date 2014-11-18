//
//  Flame.h
//  project
//
//  Class that defines the fire as mouse cursor
//
//  Created by Alona Bas on 6/8/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Flame__
#define __project__Flame__

#include <iostream>
#include "ShaderIO.h"
#include "Camera.h"
class Flame {
	int _time;
	GLuint _program;
	GLuint _Muniform, _timeUniform, _resolutionUniform;
	GLuint _vao, _vbo;
	GLuint _attrib;
	mat4 _modelTranslate;
public:
	Flame();
	void init();
	void draw();
	virtual ~Flame();
	void setTranslation(mat4 matrix);

};

#endif /* defined(__project__Flame__) */
