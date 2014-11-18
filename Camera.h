//
//  Camera.cpp
//  project
//
//  Camera singleton class
//
//  Created by Alona Bas on 4/21/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Camera__
#define __project__Camera__

#include "ShaderIO.h"

class Camera {
	vec3 _up, _side, _stright; // basis 3D
	vec3 _position_cam, _target_cam;
	Camera(); // private ctor
	static Camera* _camera;
	quat _rotation;
	mat4 _view;
	mat4 _projection;
public:
	static Camera* instance() {  // Singleton
		if (_camera == NULL) {
			_camera = new Camera();
		}
		return _camera;
	}
	virtual ~Camera(); // Destructor
	void reset();
	void turnUp();
	void turnDown();
	void turnLeft();
	void turnRight();
	void moveRight();
	void moveLeft();
	mat4 getVP(); // Get matrix, View and projection
	mat4 getV(); // Get matrix View
	mat4 getP(); // Get matrix projection
	vec3 getEye() {
		return _position_cam;
	}
	vec3 getUp() {
		return _up;
	}
	vec3 getDir() {
		return normalize(_position_cam - _target_cam);
	}
	vec3 getTarget() {
		return _target_cam;
	}
	vec3 getSide() {
		return normalize(cross(_up, normalize(_position_cam - _target_cam)));
	}
};

#endif /* defined(__project__Camera__) */
