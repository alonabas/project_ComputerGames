//
//  PPBObject.cpp
//  project
//
//  To create postprocessing effects
//
//  Created by Alona Bas on 5/5/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "PPBObject.h"

PPBObject::PPBObject() :
		_vao(0), vbo_fbo_vertices(0), fbo(0), fbo_texture(0), depth_texture(0), _timeAnimation(0) {
}

PPBObject::~PPBObject() {
    if(_vao != 0){
        glDeleteVertexArrays(1,&_vao);
    }
    if(vbo_fbo_vertices != 0){
        glDeleteBuffers(1, &vbo_fbo_vertices);
    }
    if(fbo_texture != 0){
        glDeleteTextures(1, &fbo_texture);
    }
    if(fbo != 0){
        glDeleteFramebuffers(1, &fbo);
    }
    if(depth_texture != 0){
        glDeleteTextures(1, &depth_texture);
    }
	glDeleteProgram(program_postproc);
}

void PPBObject::init(int screen_width, int screen_height) {

	_width = screen_width;
	_height = screen_height;
	// buffer
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	/* Texture  */

	// The texture we're going to render to
	glGenTextures(1, &fbo_texture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Depth texture. Slower, but you can sample it later in your shader
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, screen_width,
			screen_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* Framebuffer to link everything together */glFramebufferTexture(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo_texture, 0); //use more of these for MRT
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(1, DrawBuffers);

	// for MRT:  glDrawBuffers(2,{GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1});

	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER))
			!= GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "glCheckFramebufferStatus: error %d", status);
		exit(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// vertices
	GLfloat fbo_vertices[] = { -1, -1, 1, -1, -1, 1, 1, 1, };

	// Create and bind the object's Vertex Array Object:
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &vbo_fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices,
			GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	// shader
	programManager::sharedInstance().createProgram("pp",
			SHADERS_DIR "postproc.v.glsl", SHADERS_DIR "postproc.f.glsl");
	program_postproc = programManager::sharedInstance().programWithID("pp");

	attribute_v_coord_postproc = glGetAttribLocation(program_postproc,
			"v_coord");
	uniform_fbo_texture = glGetUniformLocation(program_postproc, "fbo_texture");
	uniform_size_x = glGetUniformLocation(program_postproc, "size_x");
	uniform_size_y = glGetUniformLocation(program_postproc, "size_y");
	uniform_depth_texture = glGetUniformLocation(program_postproc,
			"depth_texture");
	uniform_time_animation = glGetUniformLocation(program_postproc,
			"time_animation");
}

void PPBObject::setup() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void PPBObject::render() {
	_timeAnimation++;
	if (_timeAnimation == 4) {
		_timeAnimation = -4;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program_postproc);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(uniform_fbo_texture, /*GL_TEXTURE*/0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glUniform1i(uniform_depth_texture, /*GL_TEXTURE*/1);
	glUniform1f(uniform_size_x, float(_width));
	glUniform1f(uniform_size_y, float(_height));
	glUniform1f(uniform_time_animation, _timeAnimation % 5);
	glBindVertexArray(_vao);
	glEnableVertexAttribArray(attribute_v_coord_postproc);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glVertexAttribPointer(attribute_v_coord_postproc,  // attribute
			2,                  // number of elements per vertex, here (x,y)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0,                  // no extra data between each position
			0                   // offset of first element
			);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(attribute_v_coord_postproc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

}

void PPBObject::resize(int screen_width, int screen_height) {

	/* onReshape */
	// Rescale FBO
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	_width = screen_width;
	_height = screen_height;
}

