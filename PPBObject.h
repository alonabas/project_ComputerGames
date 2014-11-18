//
//  PPBObject.h
//  project
//
//  To create postprocessing effects
//
//  Created by Alona Bas on 5/5/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__PPBObject__
#define __project__PPBObject__

#include "Defs.h"
#include "ShaderIO.h"

class PPBObject {
	GLuint _vao;
	GLuint vbo_fbo_vertices;
	GLuint fbo, fbo_texture, depth_texture;
	GLuint program_postproc, attribute_v_coord_postproc, uniform_fbo_texture,
			uniform_size_x, uniform_size_y, uniform_depth_texture,
			uniform_time_animation;
	float _width, _height, _offsetX, _offsetY;
	int _timeAnimation;
public:
	PPBObject();
	virtual ~PPBObject();
	void init(int screen_width, int screen_height);
	void setup();
	void render();
	void resize(int width, int height);
};

#endif /* defined(__project__PPBObject__) */
