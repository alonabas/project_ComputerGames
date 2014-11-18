//
//  FinalScene.cpp
//  project_temp
//
//  Created by Alona Bas on 9/27/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "FinalScene.h"
#include "stb_image.h"

extern int _global_width;
extern int _global_high;

FinalScene::FinalScene() :
		_vao(0), _vbo(0), _textureBuffer(0) {
	_winMessage = new Text("Fonts/VideophreakFontRed.tga");
	_firework = new Firework();
	_sky = new Sky();
//            _winSound = NULL;
            
}

FinalScene::~FinalScene() {
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_textureBuffer != 0)
		glDeleteBuffers(1, &_textureBuffer);
	if (_winMessage != NULL) {
		delete _winMessage;
		_winMessage = NULL;
	}
	if (_firework != NULL) {
		delete _firework;
		_firework = NULL;
	}
	if (_sky != NULL) {
		delete _sky;
		_sky = NULL;
	}
    if (_winSound != NULL){
        delete _winSound;
        _winSound = NULL;
    }
}

void FinalScene::init() {
	programManager::sharedInstance().createProgram("FinalScene",
			SHADERS_DIR "FinalSceneShader.vert",
			SHADERS_DIR "FinalSceneShader.geom",
			SHADERS_DIR "FinalSceneShader.frag");
	_program = programManager::sharedInstance().programWithID("FinalScene");
	// get uniforms
	_Muniform = glGetUniformLocation(_program, "modelMat");
	_timeUniform = glGetUniformLocation(_program, "time");
	_textureUniform = glGetUniformLocation(_program, "map");
	_LightPositionUniform[0] = glGetUniformLocation(_program,
			"light_position[0]");
	_LightPositionUniform[1] = glGetUniformLocation(_program,
			"light_position[1]");
	_LightPositionUniform[2] = glGetUniformLocation(_program,
			"light_position[2]");
	_LightColorUniform[0] = glGetUniformLocation(_program, "light_color[0]");
	_LightColorUniform[1] = glGetUniformLocation(_program, "light_color[1]");
	_LightColorUniform[2] = glGetUniformLocation(_program, "light_color[2]");
	_scaleUniform = glGetUniformLocation(_program, "scale");
	_translateUniform = glGetUniformLocation(_program, "translate");
	_attrib = glGetAttribLocation(_program, "position");

	int w, h, comp;
	unsigned char *data;
	data = stbi_load("final.png", &w, &h, &comp, STBI_rgb_alpha);

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

	vec3 points = vec3(0.0f, 0.0f, 0.0f);
	// Create and bind the object's Vertex Array Object:
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(_attrib);
	glVertexAttribPointer(_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	_winMessage->init();
	_winMessage->resize(_global_width, _global_high);
	_postProcessing.init(WINDOW_SIZE, WINDOW_SIZE);
	_firework->init();
	_sky->init();
    _winSound = Sound::readWave("Sound/win.wav");
}

void FinalScene::draw() {

    _winSound->play();
	_postProcessing.setup();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _sky->draw();
	_firework->draw();
	_postProcessing.render();
//    glutSwapBuffers();
	glUseProgram(_program);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Also try using GL_FILL and GL_POINT
	//
	glUniform1i(_textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);
	//
	glUniform3fv(_LightPositionUniform[0], 1, value_ptr(LIGHT1_POSITION));
	glUniform3fv(_LightPositionUniform[1], 1, value_ptr(LIGHT2_POSITION));
	glUniform3fv(_LightPositionUniform[2], 1,
			value_ptr(Torch::instance()->getPosition()));
	glUniform3fv(_LightColorUniform[0], 1, value_ptr(LIGHT1_COLOR));
	glUniform3fv(_LightColorUniform[1], 1, value_ptr(LIGHT2_COLOR));
	glUniform3fv(_LightColorUniform[2], 1, value_ptr(LIGHT3_COLOR));
	glUniformMatrix4fv(_scaleUniform, 1, GL_FALSE,
			value_ptr(scale(mat4(1.0f), vec3(0.25f))));
	glUniformMatrix4fv(_translateUniform, 1, GL_FALSE,
			value_ptr(translate(mat4(1.0f), vec3(0.0f, 0.5f, 0.0f))));
	//
	glBindVertexArray(_vao);
	glDrawArraysInstanced(GL_POINTS, 0, 1, 1);
	glDisable(GL_BLEND);


	glUseProgram(0);
	_winMessage->draw("Congratulations! You Win!", _global_high - 50.0f, 30.0f);
}
