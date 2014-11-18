//
//  Text.cpp
//  project
//
//  Draw text on the screen
//
//  Created by Alona Bas on 4/26/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Text.h"

Text::Text(const char* texture) :
		_textureBuffer(0), _vao(0), _vbo { 0, 0 }, _texture(texture), _width(
				WINDOW_SIZE), _height(WINDOW_SIZE) {

}

Text::~Text() {
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(2, _vbo);
	if (_textureBuffer != 0)
		glDeleteTextures(1, &_textureBuffer);
    glDeleteProgram(_program);
}

void Text::init() {
	programManager::sharedInstance().createProgram("text",
			SHADERS_DIR "TextShader.vert", SHADERS_DIR "TextShader.frag");
	_program = programManager::sharedInstance().programWithID("text");

	_textureUniform = glGetUniformLocation(_program, "textureText");
	_resolutionUniform = glGetUniformLocation(_program, "resolution");
	_attrib[0] = glGetAttribLocation(_program, "position");
	_attrib[1] = glGetAttribLocation(_program, "texCoord");

	TGALOADER::IMAGE image;
	image.LoadTGA(_texture);

	glGenTextures(1, &_textureBuffer);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(),
			0, GL_BGRA, GL_UNSIGNED_BYTE, image.getDataForOpenGL());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(2, _vbo);

}

void Text::draw(const char* text, float posY, float size) {
	size_t length = strlen(text);
	float start = _width / 2.0;
	start -= length / 2 * (size-2.0f);
	// Fill buffers
	std::vector<vec2> vertices;
	std::vector<vec2> UVs;
	for (unsigned int i = 0; i < length; i++) {

		glm::vec2 vertex_up_left = glm::vec2(start + i * size - i*2.0, posY + size);
		glm::vec2 vertex_up_right = glm::vec2(start + i * size + size - i*2.0,
				posY + size);
		glm::vec2 vertex_down_right = glm::vec2(start + i * size + size - i*2.0, posY);
		glm::vec2 vertex_down_left = glm::vec2(start + i * size - i*2.0, posY);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, 1.0f - uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, 1.0f - uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f,
				1.0f - (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, 1.0f - (uv_y + 1.0f / 16.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2),
			&vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0],
			GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(_program);
	glBindVertexArray(_vao);
	// Bind texture
	glUniform1i(_textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);
	// Set our "myTextureSampler" sampler to user Texture Unit 0

	glUniform2fv(_resolutionUniform, 1, value_ptr(vec2(_width, _height)));
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(_attrib[0]);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(_attrib[1]);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, (int) vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

