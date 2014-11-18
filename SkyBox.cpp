//
//  SkyBox.h
//  project
//
//  Creates skybox
//
//  Created by Alona Bas on 4/24/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "SkyBox.h"
extern int _global_width;
extern int _global_high;

SkyBox::SkyBox() :
    _textureBuffer{0}, _vao(0), _vbo(0), _time(0) {
}

SkyBox::~SkyBox() {
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_textureBuffer != 0)
		glDeleteTextures(4, _textureBuffer);
    glDeleteProgram(_program);
}

void SkyBox::init(Camera* camera) {
	_camera = Camera::instance();
	programManager::sharedInstance().createProgram("skybox",
			SHADERS_DIR "SkyBoxShader.vert", SHADERS_DIR "SkyBoxShader.frag");
	_program = programManager::sharedInstance().programWithID("skybox");

	_WVPuniform = glGetUniformLocation(_program, "WVP");
	_textureUniform[0] = glGetUniformLocation(_program, "cubemapTexture");
    _textureUniform[1] = glGetUniformLocation(_program, "cubemapDepthTexture");
    _textureUniform[2] = glGetUniformLocation(_program, "cubemapSkyTexture");
    _textureUniform[3] = glGetUniformLocation(_program, "cubemapNormalTexture");
	_camUniform = glGetUniformLocation(_program, "cameraPosition");
    _cameraSideUniform = glGetUniformLocation(_program, "cameraSide");
    _cameraUpUniform = glGetUniformLocation(_program, "cameraUp");
    _cameraDirUniform = glGetUniformLocation(_program, "cameraDir");
    _timeUniform = glGetUniformLocation(_program, "time");
	_resolutionUniform = glGetUniformLocation(_program, "resolution");
    _LightPosUniform = glGetUniformLocation(_program, "lightPos");
    _LightColorUniform = glGetUniformLocation(_program, "lightColor");
    
	_attrib = glGetAttribLocation(_program, "position");

	glGenTextures(4, _textureBuffer);
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[0]);
        BImage image1(_textureFiles[0]);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, // level
                     GL_RGBA, // Internal representation
                     image1.width(), image1.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image1.getImageData()); // Pixel data
        image1.clear();
        BImage image2(_textureFiles[1]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, // level
                     GL_RGBA, // Internal representation
                     image2.width(), image2.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image2.getImageData()); // Pixel data
        image2.clear();
        BImage image3(_textureFiles[2]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, // level
                     GL_RGBA, // Internal representation
                     image3.width(), image3.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image3.getImageData()); // Pixel data
        image3.clear();
        BImage image4(_textureFiles[3]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, // level
                     GL_RGBA, // Internal representation
                     image4.width(), image4.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image4.getImageData()); // Pixel data
        image4.clear();
        BImage image5(_textureFiles[4]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, // level
                     GL_RGBA, // Internal representation
                     image5.width(), image5.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image5.getImageData()); // Pixel data
        image5.clear();
        BImage image6(_textureFiles[5]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, // level
                     GL_RGBA, // Internal representation
                     image6.width(), image6.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image6.getImageData()); // Pixel data
        image6.clear();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[1]);
        BImage image1(_textureDepthFiles[0]);
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, // level
                     GL_RGBA, // Internal representation
                     image1.width(), image1.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image1.getImageData()); // Pixel data
        image1.clear();
        BImage image2(_textureDepthFiles[1]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, // level
                     GL_RGBA, // Internal representation
                     image2.width(), image2.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image2.getImageData()); // Pixel data
        image2.clear();
        BImage image3(_textureDepthFiles[2]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, // level
                     GL_RGBA, // Internal representation
                     image3.width(), image3.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image3.getImageData()); // Pixel data
        image3.clear();
        BImage image4(_textureDepthFiles[3]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, // level
                     GL_RGBA, // Internal representation
                     image4.width(), image4.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image4.getImageData()); // Pixel data
        image4.clear();
        BImage image5(_textureDepthFiles[4]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, // level
                     GL_RGBA, // Internal representation
                     image5.width(), image5.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image5.getImageData()); // Pixel data
        image5.clear();
        BImage image6(_textureDepthFiles[5]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, // level
                     GL_RGBA, // Internal representation
                     image6.width(), image6.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image6.getImageData()); // Pixel data
        image6.clear();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[2]);
        BImage image1(_textureSkyFiles[0]);
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, // level
                     GL_RGBA, // Internal representation
                     image1.width(), image1.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image1.getImageData()); // Pixel data
        image1.clear();
        BImage image2(_textureSkyFiles[1]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, // level
                     GL_RGBA, // Internal representation
                     image2.width(), image2.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image2.getImageData()); // Pixel data
        image2.clear();
        BImage image3(_textureSkyFiles[2]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, // level
                     GL_RGBA, // Internal representation
                     image3.width(), image3.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image3.getImageData()); // Pixel data
        image3.clear();
        BImage image4(_textureSkyFiles[3]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, // level
                     GL_RGBA, // Internal representation
                     image4.width(), image4.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image4.getImageData()); // Pixel data
        image4.clear();
        BImage image5(_textureSkyFiles[4]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, // level
                     GL_RGBA, // Internal representation
                     image5.width(), image5.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image5.getImageData()); // Pixel data
        image5.clear();
        BImage image6(_textureSkyFiles[5]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, // level
                     GL_RGBA, // Internal representation
                     image6.width(), image6.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image6.getImageData()); // Pixel data
        image6.clear();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[3]);
        BImage image1(_textureNormalFiles[0]);
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, // level
                     GL_RGBA, // Internal representation
                     image1.width(), image1.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image1.getImageData()); // Pixel data
        image1.clear();
        BImage image2(_textureNormalFiles[1]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, // level
                     GL_RGBA, // Internal representation
                     image2.width(), image2.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image2.getImageData()); // Pixel data
        image2.clear();
        BImage image3(_textureNormalFiles[2]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, // level
                     GL_RGBA, // Internal representation
                     image3.width(), image3.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image3.getImageData()); // Pixel data
        image3.clear();
        BImage image4(_textureNormalFiles[3]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, // level
                     GL_RGBA, // Internal representation
                     image4.width(), image4.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image4.getImageData()); // Pixel data
        image4.clear();
        BImage image5(_textureNormalFiles[4]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, // level
                     GL_RGBA, // Internal representation
                     image5.width(), image5.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image5.getImageData()); // Pixel data
        image5.clear();
        BImage image6(_textureNormalFiles[5]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, // level
                     GL_RGBA, // Internal representation
                     image6.width(), image6.height(), // Texture size
                     0, // must be 0
                     GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                     image6.getImageData()); // Pixel data
        image6.clear();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
	float points[] = {
			// x, y, z, x, y, z, x, y, z, x, y, z
			1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
			1.0f, // +X

			-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
			-1.0f, // -X

			-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, // +Y

			-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
			-1.0f, // -Y

			1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
			1.0f, // +Z

			-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f // -Z
			};

	// Create and bind the object's Vertex Array Object:
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points,
			GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(_attrib);
	glVertexAttribPointer(_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void SkyBox::draw() {
    _camera = Camera::instance();
	glUseProgram(_program);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Also try using GL_FILL and GL_POINT

	glUniform1i(_textureUniform[0], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[0]);
    
    glUniform1i(_textureUniform[1], 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[1]);
    
    glUniform1i(_textureUniform[2], 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[2]);
    
    glUniform1i(_textureUniform[3], 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[3]);
    
    glUniformMatrix4fv(_WVPuniform, 1, GL_FALSE, value_ptr(_camera->getVP()));
	glUniform3fv(_camUniform, 1, value_ptr(_camera->getEye()));
    glUniform3fv(_cameraSideUniform, 1, value_ptr(_camera->getSide()));
	glUniform3fv(_cameraUpUniform, 1, value_ptr(_camera->getUp()));
    glUniform3fv(_cameraDirUniform, 1, value_ptr(_camera->getDir()));
    glUniform1f(_timeUniform, (_time++) / 120.0f);
    glUniform2fv(_resolutionUniform, 1, value_ptr(vec2(800.0f, 800.0f)));
    glUniform3fv(_LightPosUniform, 1, value_ptr(Torch::instance()->getPosition()));
    glUniform3fv(_LightColorUniform, 1, value_ptr(LIGHT3_COLOR));
    
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}
