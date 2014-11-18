//
//  Camera.cpp
//  project
//
//  Camera singleton class
//
//  Created by Alona Bas on 4/21/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Camera.h"

Camera* Camera::_camera = NULL;

Camera::Camera() :
		_up(UP_VECTOR), _side(ASIDE_VECTOR), _stright(DIR_VECTOR), _position_cam(
				0.0, 0.0, 0.0), _target_cam(0.0, 0.0, 1.0) {
	_rotation = quat_cast(mat4(1.0f));
	_target_cam = _position_cam + _stright * 0.5f;
}

Camera::~Camera() {
}
mat4 Camera::getVP() {

	_view = lookAt(_position_cam, _target_cam, _up);
	_projection = perspective(PERSPECTIVE_ANGLE, 1.0f, PERSPECTIVE_START,
			PERSPECTIVE_DISTANCE + 1.0f);
	return _projection * _view;
}

mat4 Camera::getV() {
	_view = lookAt(_position_cam, _target_cam, _up);
	return _view;
}
mat4 Camera::getP() {
	_projection = perspective(PERSPECTIVE_ANGLE, 1.0f, PERSPECTIVE_START,
			PERSPECTIVE_DISTANCE + 1.0f);
	return _projection;
}

void Camera::turnUp() {
	quat temp = angleAxis(degrees(ROTATION_STEP), _side);
	mat3 tempRotation = toMat3(temp);
	_stright = normalize(tempRotation * _stright);
	_up = normalize(tempRotation * _up);
	_rotation = temp * _rotation;
	_target_cam = _position_cam + _stright * 0.5f;
}
void Camera::turnDown() {
	quat temp = angleAxis(degrees(ROTATION_STEP), -_side);
	mat3 tempRotation = toMat3(temp);
	_stright = normalize(tempRotation * _stright);
	_up = normalize(tempRotation * _up);
	_rotation = temp * _rotation;
	_target_cam = _position_cam + _stright * 0.5f;
}

void Camera::reset() {
	_position_cam = vec3(0.0f);
	_up = UP_VECTOR;
	_side = ASIDE_VECTOR;
	_stright = DIR_VECTOR;
	_target_cam = _position_cam + _stright * 0.5f;
	_rotation = quat_cast(mat4(1.0f));
}
void Camera::turnLeft() {
	quat temp = angleAxis(degrees(ROTATION_STEP), _up);
	mat3 tempRotation = toMat3(temp);
	_stright = normalize(tempRotation * _stright);
	_side = normalize(tempRotation * _side);
	_rotation = temp * _rotation;
	_target_cam = _position_cam + _stright * 0.5f;
}

void Camera::turnRight() {
	quat temp = angleAxis(degrees(ROTATION_STEP), -_up);
	mat3 tempRotation = toMat3(temp);
	_stright = normalize(tempRotation * _stright);
	_side = normalize(tempRotation * _side);
	_rotation = temp * _rotation;
	_target_cam = _position_cam + _stright * 0.5f;
}

void Camera::moveRight() {
	if (_position_cam[0] < 6.0) {
		_position_cam += vec3(0.1, 0.0f, 0.0f);
		_target_cam = _position_cam + _stright * 0.5f;
	}
}
void Camera::moveLeft() {
	if (_position_cam[0] > -6.0) {
		_position_cam -= vec3(0.1, 0.0f, 0.0f);
		_target_cam = _position_cam + _stright * 0.5f;
	}
}
