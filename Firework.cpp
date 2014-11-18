//
//  Firework.cpp
//  project
//
//  Class that defines firework in final scene
//
//  Created by Alona Bas on 8/25/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Firework.h"

Firework::Firework() :
		_vao(0), _vbo { 0 } {
	srand((unsigned int) time(NULL));
}

Firework::~Firework() {
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(2, _vbo);
}

void Firework::init() {
	programManager::sharedInstance().createProgram("fire",
			SHADERS_DIR "FireShader.vert", SHADERS_DIR "FireShader.geom",
			SHADERS_DIR "FireShader.frag");
	_program = programManager::sharedInstance().programWithID("fire");
	glUseProgram(_program);
	_WVPuniform = glGetUniformLocation(_program, "WVP");
	_camUniform = glGetUniformLocation(_program, "cameraTarget");
	_attribute[0] = glGetAttribLocation(_program, "position");
	_attribute[1] = glGetAttribLocation(_program, "color");
	srand((int) time(NULL));
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(2, _vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	// Unbind vertex array:
	glBindVertexArray(0);
}
void Firework::update() {
	vec3 acceleration;
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (Particles[i].life > rand() % 500) {
			Particles[i].renew();
		}
		Particles[i].life++;
		acceleration = vec3(0.0f, -9.8f, 0.0f) - WIND_POSITION1
				+ Particles[i].position;
		Particles[i].speed += acceleration * TIME_UNIT * 0.1f;
		Particles[i].position += Particles[i].speed * TIME_UNIT * 0.1f;
        
	}

}

void Firework::draw() {
	update();
	Camera * camera = Camera::instance();
	glUseProgram(_program);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Also try using GL_FILL and GL_POINT

	glUniform3fv(_camUniform, 1, value_ptr(camera->getTarget()));
	glUniformMatrix4fv(_WVPuniform, 1, GL_FALSE, value_ptr(camera->getVP()));
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glEnableVertexAttribArray(_attribute[0]);
	glVertexAttribPointer(_attribute[0], // attribute handle
			3,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, sizeof(Particle), (GLvoid*) offsetof(Particle, position));
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), &Particles,
			GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glEnableVertexAttribArray(_attribute[1]);
	glVertexAttribPointer(_attribute[1], // attribute handle
			3,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, sizeof(Particle), (GLvoid*) offsetof(Particle, color));
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), &Particles,
			GL_DYNAMIC_DRAW);

	// Draw using the state stored in the Vertex Array object:
	glDrawArraysInstanced(GL_POINTS, 0, MAX_PARTICLES, 1);
	glUseProgram(0);
}
