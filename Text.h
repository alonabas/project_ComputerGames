//
//  Text.h
//  project
//
//  Draw text on the screen
//
//  Created by Alona Bas on 4/26/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Text__
#define __project__Text__

#include "TGALoader.h"
#include "bimage.h"
#include "ShaderIO.h"
#include "Defs.h"

class Text {
	GLuint _textureBuffer, _vao, _vbo[2], _attrib[2];
	GLuint _textureUniform, _resolutionUniform; // uniforms
	GLuint _program;
	const char* _texture;
	float _width, _height;
public:
	Text(const char* texture);
	virtual ~Text();
	void init();
	void draw(const char* text, float posY, float size);
	void resize(float width, float height) {
		_width = width;
		_height = height;
	}
};

#endif /* defined(__project__Text__) */
