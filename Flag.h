//
//  Flag.h
//  project
//
//  Class that defines Flag
//
//  Created by Alona Bas on 9/15/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Flag__
#define __project__Flag__

#define FLAG_POINTS_HIGH 36
#define FLAG_POINTS_WIDTH 26
#define COUNT FLAG_POINTS_HIGH*FLAG_POINTS_WIDTH
#define FLAG_INDX(x,y) ((x * FLAG_POINTS_WIDTH) + y)

#include <iostream>
#include "MeshObject.h"
#include "Defs.h"
#include "Object.h"
#include "Globals.h"
#include "Torch.h"

#define KS 50.0f
#define KD 0.995f
#define SD (1.0f/40.0f)
#define SDD glm::sqrt(2.0f)*SD
#define SCALE_FACTOR (float)(1.0f/40.0f)

class Flag: public Object {
	GLuint _vao, _vbo, _ebo, _vboNormal, _vboUV, _textureBuffer;
	GLint _program;
	bool _found;
	const char* _programName, *_textureImage;
	GLint _LightPositionUniform[3], _LightColorUniform[3];
	GLuint _MnormalUniform, _textureUniform, _Muniform, _Vuniform, _Puniform,
			_typeUniform;
	GLint _attrib[3];
	MeshObject* _stand;
	vec3 _vertice[COUNT];
	vec3 _normals[COUNT];
	vec3 _velocities[COUNT];
	mat4 _moveToCenter;
	size_t _size;
	void update();
	vec3 calculateStep(int curIdx, int nexIdx, float initStep);
public:
	Flag(const char* programName, const char* texture);
	virtual ~Flag();
	void init(vec3 position);
	void draw();
	void found() {
		_found = true;
	}
	bool isFound() {
		return _found;
	}
};

#endif /* defined(__project__Flag__) */
