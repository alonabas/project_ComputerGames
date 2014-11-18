//
//  Background.cpp
//  project
//
//  Created by Alona Bas on 9/23/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Background.h"
#include "stb_image.h"

Background::Background() :
		_time(0), _vao(0), _vbo(0), _ebo(0), _vboUV(0), _textureBuffer { 0 }, _modelScale(
				1.0f), _modelRotate(1.0f), _modelTranslate(1.0f) {
}

Background::~Background() {
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);
	if (_vboUV != 0)
		glDeleteBuffers(1, &_vboUV);
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_textureBuffer != 0)
		glDeleteBuffers(4, _textureBuffer);
}

void Background::init() {
	vec2 uvs[BG_COUNT];
	vec3 vertice[BG_COUNT];
	_size = (BG_POINTS_HIGH - 1) * (BG_POINTS_WIDTH - 1) * 6;
	for (int x = 0; x < BG_POINTS_HIGH; x++) {
		for (int y = 0; y < BG_POINTS_WIDTH; y++) {
			vertice[BG_INDX(x, y)] = vec3(
					((float) x) * SCALE_FACTOR_X
							- (float) (SCALE_FACTOR_X / 2.0f),
					((float) y) * SCALE_FACTOR_Y
							- (float) (SCALE_FACTOR_Y / 2.0f), -10.0f);
			uvs[BG_INDX(x, y)] = vec2(1.0f - (float) x, 1.0f - (float) y);
		}
	}
	//generate indices
	int k = 0;
	GLuint indice[_size];
	for (int x = 0; x < BG_POINTS_HIGH - 1; x++) {
		for (int y = 0; y < BG_POINTS_WIDTH - 1; y++) {
			// look at square and devide to 2 triangles

			if ((x & 1 && y & 1) || !((x + y) & 1)) {
				indice[k] = BG_INDX(x, (y + 1));
				k++;
				indice[k] = BG_INDX(x, y);
				k++;
				indice[k] = BG_INDX((x + 1),y);
				k++;
				// second triangle
				indice[k] = BG_INDX(x, (y + 1));
				k++;
				indice[k] = BG_INDX((x + 1), (y + 1));
				k++;
				indice[k] = BG_INDX((x + 1),y);

			} else {
				indice[k] = BG_INDX(x, y);
				k++;
				indice[k] = BG_INDX((x + 1), (y + 1));
				k++;
				indice[k] = BG_INDX((x + 1),y);
				k++;
				// second triangle
				indice[k] = BG_INDX(x, (y + 1));
				k++;
				indice[k] = BG_INDX((x + 1), (y + 1));
				k++;
				indice[k] = BG_INDX(x, y);
			}
			k++;
		}
	}
	// get uniforms
	programManager::sharedInstance().createProgram("Background",
			SHADERS_DIR "Background.vert", SHADERS_DIR "Background.frag");
	_program = programManager::sharedInstance().programWithID("Background");
	_textureUniform[0] = glGetUniformLocation(_program, "textureData");
	_textureUniform[1] = glGetUniformLocation(_program, "textureDataDepth");
	_textureUniform[2] = glGetUniformLocation(_program, "textureDataSky");
	_textureUniform[3] = glGetUniformLocation(_program, "textureDataNormal");
	_Muniform = glGetUniformLocation(_program, "modelMat");
	_Vuniform = glGetUniformLocation(_program, "viewMat");
	_Puniform = glGetUniformLocation(_program, "projMat");
	_camUniform = glGetUniformLocation(_program, "cameraPosition");
	_cameraDirUniform = glGetUniformLocation(_program, "cameraDir");
	_timeUniform = glGetUniformLocation(_program, "time");
	_resolutionUniform = glGetUniformLocation(_program, "resolution");
	_LightPosUniform = glGetUniformLocation(_program, "lightPos");
	_LightColorUniform = glGetUniformLocation(_program, "lightColor");

	// atributes
	_attrib[0] = glGetAttribLocation(_program, "position");
	_attrib[1] = glGetAttribLocation(_program, "uv");

	int w, h, comp;
	unsigned char *data;
	glGenTextures(4, _textureBuffer);
	{
		data = stbi_load("skybox/bg.png", &w, &h, &comp, STBI_rgb_alpha);

		glBindTexture(GL_TEXTURE_2D, _textureBuffer[0]);

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
	}
	{
		data = stbi_load("skybox/bgDepth.png", &w, &h, &comp, STBI_rgb_alpha);
		glBindTexture(GL_TEXTURE_2D, _textureBuffer[1]);
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
	}
	{
		data = stbi_load("skybox/bgSkyAlpha.png", &w, &h, &comp,
				STBI_rgb_alpha);
		glBindTexture(GL_TEXTURE_2D, _textureBuffer[2]);
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
	}
	{
		data = stbi_load("skybox/bgNormal.png", &w, &h, &comp, STBI_rgb_alpha);
		glBindTexture(GL_TEXTURE_2D, _textureBuffer[3]);
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
	}

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	// Create and load vertex data into a Vertex Buffer Object:
	glGenBuffers(1, &_vbo);
	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), &(vertice[0]),
			GL_STATIC_DRAW);

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

	glGenBuffers(1, &_vboUV);
	glBindBuffer(GL_ARRAY_BUFFER, _vboUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), &(uvs[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(_attrib[1]);
	glVertexAttribPointer(_attrib[1], // attribute handle
			2,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Background::draw() {
	glUseProgram(_program);
	Camera * camera = Camera::instance();
	glUniformMatrix4fv(_Muniform, 1, GL_FALSE, value_ptr(_modelMat));
	glUniformMatrix4fv(_Vuniform, 1, GL_FALSE, value_ptr(camera->getV()));
	glUniformMatrix4fv(_Puniform, 1, GL_FALSE, value_ptr(camera->getP()));
	glUniform3fv(_camUniform, 1, value_ptr(camera->getEye()));
	glUniform3fv(_cameraDirUniform, 1, value_ptr(camera->getDir()));
	glUniform1f(_timeUniform, (_time++) / 120.0f);
	glUniform2fv(_resolutionUniform, 1, value_ptr(vec2(800.0f, 800.0f)));
	glUniform3fv(_LightPosUniform, 1,
			value_ptr(Torch::instance()->getPosition()));
	glUniform3fv(_LightColorUniform, 1, value_ptr(LIGHT3_COLOR));

	// Bind texture
	{
		glUniform1i(_textureUniform[0], 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureBuffer[0]);
	}
	{
		glUniform1i(_textureUniform[1], 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _textureBuffer[1]);
	}
	{
		glUniform1i(_textureUniform[2], 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _textureBuffer[2]);
	}
	{
		glUniform1i(_textureUniform[3], 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, _textureBuffer[3]);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(_vao);

	glDrawElements(GL_TRIANGLES, (int) _size, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	// Unbind the Vertex Array object
	glBindVertexArray(0);
	glUseProgram(0);
}
