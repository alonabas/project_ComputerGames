//
//  Frame.h
//  project
//
//  Class that defines the frame for found items
//
//  Created by Alona Bas on 9/24/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Frame.h"
#include "stb_image.h"
#include "TGALoader.h"

Frame* Frame::_frame = NULL;

Frame::Frame() :
		_vao(0), _vbo(0), _ebo(0), _vboUV(0), _textureBuffer(0), _transform(
				mat4(1.0f)) {
}

Frame::~Frame() {
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);
	if (_vboUV != 0)
		glDeleteBuffers(1, &_vboUV);
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_textureBuffer != 0)
		glDeleteVertexArrays(1, &_textureBuffer);
	if (_frame != NULL) {
		delete _frame;
		_frame = NULL;
	}
}

void Frame::init() {
	vec3 vertice[FRAME_COUNT];
	vec2 uvs[FRAME_COUNT];
	_size = (FRAME_POINTS_HIGH - 1) * (FRAME_POINTS_WIDTH - 1) * 6;
	for (int x = 0; x < FRAME_POINTS_HIGH; x++) {
		for (int y = 0; y < FRAME_POINTS_WIDTH; y++) { // y=c
			vertice[FRAME_INDX(x,y)] = vec3(
					((float) x - 0.5f) * FRAME_SCALE_FACTOR_X,
					((float) y - 0.5f) * FRAME_SCALE_FACTOR_Y, -1.0f);
			uvs[FRAME_INDX(x,y)] = vec2((float) x, 1.0f - (float) y);
		}
	}
	//generate indices
	int k = 0;
	GLuint indice[_size];
	for (int x = 0; x < FRAME_POINTS_HIGH - 1; x++) {
		for (int y = 0; y < FRAME_POINTS_WIDTH - 1; y++) {
			// look at square and devide to 2 triangles

			if ((x & 1 && y & 1) || !((x + y) & 1)) {
				indice[k] = FRAME_INDX(x, (y + 1));
				k++;
				indice[k] = FRAME_INDX(x, y);
				k++;
				indice[k] = FRAME_INDX((x + 1), y);
				k++;
				// second triangle
				indice[k] = FRAME_INDX(x, (y + 1));
				k++;
				indice[k] = FRAME_INDX((x + 1), (y + 1));
				k++;
				indice[k] = FRAME_INDX((x + 1), y);

			} else {
				indice[k] = FRAME_INDX(x, y);
				k++;
				indice[k] = FRAME_INDX((x + 1), (y + 1));
				k++;
				indice[k] = FRAME_INDX((x + 1), y);
				k++;
				// second triangle
				indice[k] = FRAME_INDX(x, (y + 1));
				k++;
				indice[k] = FRAME_INDX((x + 1), (y + 1));
				k++;
				indice[k] = FRAME_INDX(x, y);
			}
			k++;
		}
	}
	// get uniforms
	programManager::sharedInstance().createProgram("Frame",
			SHADERS_DIR "Frame.vert", SHADERS_DIR "Frame.frag");
	_program = programManager::sharedInstance().programWithID("Frame");
	_textureUniform = glGetUniformLocation(_program, "textureData");
	_Muniform = glGetUniformLocation(_program, "modelMat");

	// atributes
	_attrib[0] = glGetAttribLocation(_program, "position");
	_attrib[1] = glGetAttribLocation(_program, "uv");

	TGALOADER::IMAGE* image = new TGALOADER::IMAGE();
	image->LoadTGA("frame.tga");

	glGenTextures(1, &_textureBuffer);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(),
			image->getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE,
			image->getDataForOpenGL());
	delete image;

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
}

void Frame::setTransform(mat4 transorm) {
	_transform = transorm;
}

void Frame::draw() {
	glUseProgram(_program);
	glUniformMatrix4fv(_Muniform, 1, GL_FALSE, value_ptr(_transform));
	// Bind texture
	glUniform1i(_textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, (int) _size, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	// Unbind the Vertex Array object
	glBindVertexArray(0);
	glUseProgram(0);
}

