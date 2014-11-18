//
//  Flag.cpp
//  project_temp
//
//  Created by Alona Bas on 9/15/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Flag.h"

Flag::Flag(const char* programName, const char* texture) :
		Object(), _vao(0), _vbo(0), _ebo(0), _vboNormal(0), _vboUV(0), _textureBuffer(
				0), _found(false) {
	_programName = programName;
	_textureImage = texture;
	_stand = new MeshObject();
}

Flag::~Flag() {
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);
	if (_vboNormal != 0)
		glDeleteBuffers(1, &_vboNormal);
	if (_vboUV != 0)
		glDeleteBuffers(1, &_vboUV);
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_textureBuffer != 0)
		glDeleteVertexArrays(1, &_textureBuffer);
}

void Flag::init(vec3 position) {
	vec2 uvs[COUNT];
	_size = (FLAG_POINTS_HIGH - 1) * (FLAG_POINTS_WIDTH - 1) * 6;
	for (int x = 0; x < FLAG_POINTS_HIGH; x++) {
		for (int y = 0; y < FLAG_POINTS_WIDTH; y++) { // y=c
			_vertice[FLAG_INDX(x,y)] = vec3(((float) x) * SCALE_FACTOR,
					((float) y) * SCALE_FACTOR, 0.0f);
			_normals[FLAG_INDX(x, y)] = vec3(0.0f, 0.0f, -1.0f);
			_velocities[FLAG_INDX(x, y)] = vec3(0.0f, 0.0f, 0.0f);
			uvs[FLAG_INDX(x, y)] = vec2((float) x / (float) FLAG_POINTS_HIGH,
					1.0f - (float) y / (float) FLAG_POINTS_WIDTH);
		}
	}
	//generate indices
	int k = 0;
	GLuint indice[_size];
	for (int x = 0; x < FLAG_POINTS_HIGH - 1; x++) {
		for (int y = 0; y < FLAG_POINTS_WIDTH - 1; y++) {
			// look at square and devide to 2 triangles

			if ((x & 1 && y & 1) || !((x + y) & 1)) {
				indice[k] = FLAG_INDX(x,(y + 1));
				k++;
				indice[k] = FLAG_INDX(x,y);
				k++;
				indice[k] = FLAG_INDX((x + 1),y);
				k++;
				// second triangle
				indice[k] = FLAG_INDX(x,(y + 1));
				k++;
				indice[k] = FLAG_INDX((x + 1),(y + 1));
				k++;
				indice[k] = FLAG_INDX((x + 1),y);

			} else {
				indice[k] = FLAG_INDX(x,y);
				k++;
				indice[k] = FLAG_INDX((x + 1),(y + 1));
				k++;
				indice[k] = FLAG_INDX((x + 1),y);
				k++;
				// second triangle
				indice[k] = FLAG_INDX(x,(y + 1));
				k++;
				indice[k] = FLAG_INDX((x + 1),(y + 1));
				k++;
				indice[k] = FLAG_INDX(x,y);
			}
			k++;
		}
	}
	// get uniforms
	programManager::sharedInstance().createProgram(_programName,
			SHADERS_DIR "FlagShader.vert", SHADERS_DIR "FlagShader.frag");
	_program = programManager::sharedInstance().programWithID(_programName);
	_textureUniform = glGetUniformLocation(_program, "textureData");
	_MnormalUniform = glGetUniformLocation(_program, "normalMat");
	_Muniform = glGetUniformLocation(_program, "modelMat");
	_Vuniform = glGetUniformLocation(_program, "viewMat");
	_Puniform = glGetUniformLocation(_program, "projMat");
	_typeUniform = glGetUniformLocation(_program, "type");
	_LightPositionUniform[0] = glGetUniformLocation(_program,
			"light_position[0]");
	_LightPositionUniform[1] = glGetUniformLocation(_program,
			"light_position[1]");
	_LightPositionUniform[2] = glGetUniformLocation(_program,
			"light_position[2]");
	_LightColorUniform[0] = glGetUniformLocation(_program, "light_color[0]");
	_LightColorUniform[1] = glGetUniformLocation(_program, "light_color[1]");
	_LightColorUniform[2] = glGetUniformLocation(_program, "light_color[2]");

	// atributes
	_attrib[0] = glGetAttribLocation(_program, "position");
	_attrib[1] = glGetAttribLocation(_program, "normal");
	_attrib[2] = glGetAttribLocation(_program, "uv");

	int w, h, comp;
	unsigned char *data;
	data = stbi_load(_textureImage, &w, &h, &comp, STBI_rgb_alpha);

	glGenTextures(1, &_textureBuffer);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (comp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data);
	else if (comp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	// Create and load vertex data into a Vertex Buffer Object:
	glGenBuffers(1, &_vbo);
	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertice), &(_vertice[0]),
			GL_DYNAMIC_DRAW);

	// Create and load face (elements) data into an Element Buffer Object:
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), &(indice[0]),
			GL_STATIC_DRAW);
	// Obtain attribute handles:

	glEnableVertexAttribArray(_attrib[0]);
	glVertexAttribPointer(_attrib[0], // attribute handle
			3,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, 0, 0);

	glGenBuffers(1, &_vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, _vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_normals), &(_normals[0]),
			GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(_attrib[1]);
	glVertexAttribPointer(_attrib[1], // attribute handle
			3,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, 0, 0);

	glGenBuffers(1, &_vboUV);
	glBindBuffer(GL_ARRAY_BUFFER, _vboUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), &(uvs[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(_attrib[2]);
	glVertexAttribPointer(_attrib[2], // attribute handle
			2,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, 0, 0);

	_modelTranslate = translate(mat4(1.0f), position);
	glBindVertexArray(0);
	_moveToCenter = mat4(1.0f);
	_stand->init(_program, "obj_files/cone.off");

	mat4 sc = scale(mat4(1.0f), vec3(0.02f, 1.0f, 0.02f));
	_stand->setScale(sc);

	mat4 tr = translate(_modelTranslate, vec3(0.0f, -0.4f, 0.0f));
	_stand->setTranslation(tr);

	mat4 rot = rotate(mat4(1.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
	_stand->setRotation(rot);
	_bound._center = position;
	_bound._radius = fmax(FLAG_POINTS_HIGH, FLAG_POINTS_WIDTH) * SCALE_FACTOR
			* 0.75f;
}
vec3 Flag::calculateStep(int curIdx, int nexIdx, float initStep) {
//    mat4 modelMat = _modelRotate * _modelTranslate * _modelScale * _moveToCenter;
	vec3 force(0.0f);
	vec3 len = vec3(
			_modelMat * vec4(_vertice[nexIdx], 1.0f)
					- _modelMat * vec4(_vertice[curIdx], 1.0f));
	if (length(len) < EPS) {
		return force;
	}
	force = KS * ((length(len) - initStep) * normalize(len));
	if (length(force) > 10.0f) {
		return vec3(0.0f);
	}
	return force;

//    vec3 dist =  _vertice[nexIdx] - _vertice[curIdx];
//    float len = length(dist);
//    if (len < EPS) return vec3(0.0f);
//    vec3 force = KS * (dist * ((len - initStep)/len));
//    if (length(force) > 10.0f) return vec3(0.0f);
//    return force;

}

void Flag::update() {
	vec3 force_spring, force_dump, force, wind;
	vec3 temp;
	float temp1;
	vec3 acceleration[COUNT];
	for (int x = 0; x < FLAG_POINTS_HIGH; x++) {
		for (int y = 0; y < FLAG_POINTS_WIDTH; y++) {
			if (x > 0) {
				// gravity
				acceleration[FLAG_INDX(x,y)] = GRAVITY;

				// spring dumpers
				if (x != 0) {
					acceleration[FLAG_INDX(x,y)] += calculateStep(
							FLAG_INDX(x,y), FLAG_INDX((x - 1),y), SD);
				}
				if (x != FLAG_POINTS_HIGH - 1) {
					acceleration[FLAG_INDX(x,y)] += calculateStep(
							FLAG_INDX(x,y), FLAG_INDX((x + 1),y), SD);
				}
				if (y != 0) {
					acceleration[FLAG_INDX(x,y)] += calculateStep(
							FLAG_INDX(x,y), FLAG_INDX(x,(y - 1)), SD);
				}
				if (y != FLAG_POINTS_WIDTH - 1) {
					acceleration[FLAG_INDX(x,y)] += calculateStep(
							FLAG_INDX(x,y), FLAG_INDX(x,(y + 1)), SD);
				}
				if (x != 0 && y != 0) {
					acceleration[FLAG_INDX(x,y)] += calculateStep(
							FLAG_INDX(x,y), FLAG_INDX((x - 1),(y - 1)), SDD);
				}
				if (x != 0 && y != FLAG_POINTS_WIDTH - 1) {
					acceleration[FLAG_INDX(x,y)] += calculateStep(
							FLAG_INDX(x,y), FLAG_INDX((x - 1),(y + 1)), SDD);
				}
				if (x != FLAG_POINTS_HIGH - 1 && y != FLAG_POINTS_WIDTH - 1) {
					acceleration[FLAG_INDX(x,y)] += calculateStep(
							FLAG_INDX(x,y), FLAG_INDX((x + 1),(y + 1)), SDD);
				}
				if (x != FLAG_POINTS_HIGH - 1 && y != 0) {
					acceleration[FLAG_INDX(x,y)] += calculateStep(
							FLAG_INDX(x,y), FLAG_INDX((x + 1),(y - 1)), SDD);
				}
				// aerodynamic forces
				wind = WIND_POSITION1
						- vec3(
								_modelMat
										* vec4(_vertice[FLAG_INDX(x,y)], 1.0f));
				temp1 = fabs(dot(normalize(wind), _normals[FLAG_INDX(x,y)]));
				acceleration[FLAG_INDX(x,y)] += temp1 * TIME_UNIT
						* normalize(wind);
				wind = WIND_POSITION2
						- vec3(
								_modelMat
										* vec4(_vertice[FLAG_INDX(x,y)], 1.0f));
				temp1 = fabs(dot(normalize(wind), _normals[FLAG_INDX(x,y)]));
				acceleration[FLAG_INDX(x,y)] += temp1 * TIME_UNIT
						* normalize(wind);
			} else {
				acceleration[FLAG_INDX(x,y)] = vec3(0.0f);
			}
		}
	}
	for (int x = 0; x < FLAG_POINTS_HIGH; x++) {
		for (int y = 0; y < FLAG_POINTS_WIDTH; y++) {
			_velocities[FLAG_INDX(x,y)] += acceleration[FLAG_INDX(x,y)]
					* TIME_UNIT;
			_velocities[FLAG_INDX(x,y)] = _velocities[FLAG_INDX(x,y)] * KD;
			_vertice[FLAG_INDX(x,y)] += _velocities[FLAG_INDX(x,y)] * TIME_UNIT;
		}
	}
	vec3 u, v;
	vec3 posTemp1, posTemp2;
	for (int x = 0; x < FLAG_POINTS_HIGH; x++) {
		for (int y = 0; y < FLAG_POINTS_WIDTH; y++) {
			if (x > 0) {
				posTemp1 = _vertice[FLAG_INDX((x - 1),y)];
			} else {
				posTemp1 = _vertice[FLAG_INDX(x,y)];
			}
			if (x < FLAG_POINTS_HIGH - 1) {
				posTemp2 = _vertice[FLAG_INDX((x + 1),y)];
			} else {
				posTemp2 = _vertice[FLAG_INDX(x,y)];
			}
			u = posTemp2 - posTemp1;

			if (y > 0) {
				posTemp1 = _vertice[FLAG_INDX(x,(y - 1))];
			} else {
				posTemp1 = _vertice[FLAG_INDX(x,y)];
			}
			if (y < FLAG_POINTS_WIDTH - 1) {
				posTemp2 = _vertice[FLAG_INDX(x,(y + 1))];
			} else {
				posTemp2 = _vertice[FLAG_INDX(x,y)];
			}
			v = posTemp2 - posTemp1;
			_normals[FLAG_INDX(x,y)] = normalize(cross(u, v));
		}
	}
}

void Flag::draw() {
	// update data
	if (_found) {
		mat4 tr = translate(_modelTranslate, vec3(0.0f, -0.4f, 0.0f));
		_stand->setTranslation(tr);
	}
	_modelMat = _modelRotate * _modelTranslate * _modelScale * _moveToCenter;
	update();
	Camera* camera = Camera::instance();
	glm::mat4x4 model_vew_mat = camera->getV() * _modelMat;
	// inverse transpose for normals
	glm::mat4 normalMat = glm::transpose(glm::inverse(model_vew_mat));
	glUseProgram(_program);
	glUniformMatrix4fv(_MnormalUniform, 1, GL_FALSE, value_ptr(normalMat));
	glUniformMatrix4fv(_Muniform, 1, GL_FALSE, value_ptr(_modelMat));
	glUniformMatrix4fv(_Vuniform, 1, GL_FALSE, value_ptr(camera->getV()));
	glUniformMatrix4fv(_Puniform, 1, GL_FALSE, value_ptr(camera->getP()));
	glUniform3fv(_LightPositionUniform[0], 1, value_ptr(LIGHT1_POSITION));
	glUniform3fv(_LightPositionUniform[1], 1, value_ptr(LIGHT2_POSITION));
	glUniform3fv(_LightPositionUniform[2], 1,
			value_ptr(Torch::instance()->getPosition()));
	glUniform3fv(_LightColorUniform[0], 1, value_ptr(LIGHT1_COLOR));
	glUniform3fv(_LightColorUniform[1], 1, value_ptr(LIGHT2_COLOR));
	glUniform3fv(_LightColorUniform[2], 1, value_ptr(LIGHT3_COLOR));

	glUniform1i(_typeUniform, 0);
	// Bind texture
	glUniform1i(_textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertice), &(_vertice[0]),
			GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(_attrib[0]);
	glVertexAttribPointer(_attrib[0], // attribute handle
			3,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_normals), &(_normals[0]),
			GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(_attrib[1]);
	glVertexAttribPointer(_attrib[1], // attribute handle
			3,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, 0, 0);

	glDrawElements(GL_TRIANGLES, (int) _size, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	glUniform1i(_typeUniform, 1);
	_stand->draw();
	// Unbind the Vertex Array object
	glBindVertexArray(0);
	glUseProgram(0);
}
