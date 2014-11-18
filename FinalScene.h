//
//  FinalScene.h
//  project
//
//  Created by Alona Bas on 9/27/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__FinalScene__
#define __project__FinalScene__

#include <iostream>
#include "Model.h"
#include "Defs.h"
#include "ShaderIO.h"
#include "Globals.h"
#include "Torch.h"
#include "Text.h"
#include "PPBObject.h"
#include "Firework.h"
#include "Sky.h"
#include "Sound.h"

class FinalScene {
	GLuint _program;
	GLuint _Muniform, _timeUniform, _textureUniform;
	GLuint _vao, _vbo, _textureBuffer;
	GLint _LightPositionUniform[3], _LightColorUniform[3], _scaleUniform,
			_translateUniform;
	GLuint _attrib;
	Text* _winMessage;
	PPBObject _postProcessing;
	Firework * _firework;
	Sky * _sky;
    Sound *_winSound;
    
public:
	FinalScene();
	void init();
	void draw();
	virtual ~FinalScene();
};

#endif /* defined(__project__FinalScene__) */
