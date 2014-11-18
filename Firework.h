//
//  Firework.cpp
//  project
//
//  Class that defines firework in final scene
//
//  Created by Alona Bas on 8/25/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Firework__
#define __project__Firework__

#include <iostream>

#include "Camera.h"
#include "ShaderIO.h"
#include "Globals.h"

#define MAX_PARTICLES 1000

class Firework {
	GLuint _program;
	GLuint _WVPuniform, _camUniform; // uniforms
	GLuint _vao;
	GLuint _vbo[2], _attribute[2];
public:
	struct Particle {
		vec3 position; // Current position
		vec3 speed;
		float mass;
		vec3 color;
		int life;
		Particle() {
			position = vec3(0.0f, -1.0f, 0.0f);
			speed = vec3(0.0f, 9.5f, 0.0f);
			speed[2] += ((float) rand() / (float) (RAND_MAX) - 0.5f) * 6.0f;
			speed[1] += ((float) rand() / (float) (RAND_MAX) - 0.5f) * 6.0f;
			speed[0] += ((float) rand() / (float) (RAND_MAX) - 0.5f) * 6.0f;
			color = vec3((float) rand() / (float) (RAND_MAX),
					(float) rand() / (float) (RAND_MAX),
					(float) rand() / (float) (RAND_MAX));
			mass = ((float) rand() / (float) (RAND_MAX));
			life = 0;
		}
		void renew() {
			position = vec3(0.0f, -1.0f, 0.0f);
			speed = vec3(0.0f, 9.5f, 0.0f);
			speed[2] += ((float) rand() / (float) (RAND_MAX) - 0.5f) * 6.0f;
			speed[1] += ((float) rand() / (float) (RAND_MAX) - 0.5f) * 6.0f;
			speed[0] += ((float) rand() / (float) (RAND_MAX) - 0.5f) * 6.0f;
			mass = ((float) rand() / (float) (RAND_MAX));
			color = vec3((float) rand() / (float) (RAND_MAX),
					(float) rand() / (float) (RAND_MAX),
					(float) rand() / (float) (RAND_MAX));
			life = 0;
		}
	};
private:
	Particle Particles[MAX_PARTICLES];
	void update();
public:

	Firework();
	virtual ~Firework();
	virtual void init();
	virtual void draw();
};

#endif /* defined(__project__Firework__) */
