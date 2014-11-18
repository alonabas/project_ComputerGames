//
//  Frame.h
//  project
//
//  Class that defines the frame for found items
//
//  Created by Alona Bas on 9/24/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Frame__
#define __project__Frame__

#define FRAME_POINTS_HIGH 2
#define FRAME_POINTS_WIDTH 2
#define FRAME_COUNT FRAME_POINTS_HIGH*FRAME_POINTS_WIDTH
#define FRAME_INDX(x,y) ((x * FRAME_POINTS_WIDTH) + y)

#include <iostream>
#include "Defs.h"
#include "Camera.h"
#include "ShaderIO.h"
#include "Object.h"

#define FRAME_SCALE_FACTOR_X (float)(2.0f)
#define FRAME_SCALE_FACTOR_Y (float)(2.0f)

class Frame {
	GLuint _vao, _vbo, _ebo, _vboUV, _textureBuffer;
	GLint _program;
	GLuint _textureUniform, _Muniform;
	GLint _attrib[2];
	static Frame* _frame;
	size_t _size;
	mat4 _transform;
	Frame();
public:
	static Frame* instance() {  // Singleton
		if (_frame == NULL) {
			_frame = new Frame();
		}
		return _frame;
	}
	virtual ~Frame();
	void init();
	void setTransform(mat4 transorm);
	void draw();
};

#endif /* defined(__project__Frame__) */
