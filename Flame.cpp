//
//  Flame.h
//  project
//
//  Class that defines the fire as mouse cursor
//
//  Created by Alona Bas on 6/8/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Flame.h"

Flame::Flame() :
		_time(0), _vao(0), _vbo(0), _modelTranslate(mat4(1.0f)) {
}

Flame::~Flame() {
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}

void Flame::init() {
	programManager::sharedInstance().createProgram("Flame",
			SHADERS_DIR "FlameShader.vert", SHADERS_DIR "FlameShader.geom",
			SHADERS_DIR "FlameShader.frag");
	_program = programManager::sharedInstance().programWithID("Flame");
	// get uniforms
	_Muniform = glGetUniformLocation(_program, "modelMat");
	_timeUniform = glGetUniformLocation(_program, "time");
	_resolutionUniform = glGetUniformLocation(_program, "resolution");

	_attrib = glGetAttribLocation(_program, "position");

	vec3 points[1] = { vec3(0.0f, 0.0f, 0.0f) };
	// Create and bind the object's Vertex Array Object:
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(_attrib);
	glVertexAttribPointer(_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Flame::setTranslation(mat4 matrix) {
	_modelTranslate = matrix;
}

void Flame::draw() {
	glUseProgram(_program);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Also try using GL_FILL and GL_POINT
	glUniform1f(_timeUniform, (_time++) / 20.0f);
	glUniform2fv(_resolutionUniform, 1, value_ptr(vec2(100.0f, 100.0f)));
	glUniformMatrix4fv(_Muniform, 1, GL_FALSE, value_ptr(_modelTranslate));

	glBindVertexArray(_vao);
	glDrawArraysInstanced(GL_POINTS, 0, 1, 1);
	glDisable(GL_BLEND);
	glUseProgram(0);
}
