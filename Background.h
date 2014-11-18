//
//  Background.h
//  project
//  Background for flags scene
//
//  Created by Alona Bas on 9/23/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Background__
#define __project__Background__

#include <iostream>
#define BG_POINTS_HIGH 2
#define BG_POINTS_WIDTH 2
#define BG_COUNT BG_POINTS_HIGH*BG_POINTS_WIDTH
#define BG_INDX(x,y) ((x * BG_POINTS_WIDTH) + y)

#include "Torch.h"
#include "Defs.h"
#include "Camera.h"
#include "ShaderIO.h"
#include "Globals.h"

#define SCALE_FACTOR_X (float)(48.0f)
#define SCALE_FACTOR_Y (float)(27.0f)

class Background {
	int _time;
	GLuint _vao, _vbo, _ebo, _vboUV, _textureBuffer[4];
	GLint _program;
	GLuint _textureUniform[4], _Muniform, _Vuniform, _Puniform, _camUniform,
			_cameraDirUniform, _timeUniform, _resolutionUniform,
			_LightPosUniform, _LightColorUniform;
	GLint _attrib[2];
	mat4 _modelScale, _modelRotate, _modelTranslate, _modelMat;
	size_t _size;
public:
	Background();
	virtual ~Background();
	void init();
	void draw();
};

#endif /* defined(__project__Background__) */
