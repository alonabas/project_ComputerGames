//
//  Torch.cpp
//  project
//
//  Creates torch as cursor
//
//  Created by Alona Bas on 9/23/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Torch.h"

extern int _global_width;
extern int _global_high;

Torch* Torch::_torch = NULL;
Torch::Torch(): _globalPos(vec2(0.0f)), _mode(REG)
{
    _object = new MeshObject();
    _flame = new Flame();
}

Torch::~Torch()
{
    if(_object != NULL){
        delete _object;
        _object = NULL;
    }
    if(_flame != NULL){
        delete _flame;
        _flame = NULL;
    }
    if(_torch != NULL){
        delete _torch;
        _torch = NULL;
    }
    glDeleteProgram(_program);
}

void Torch::init()
{
    programManager::sharedInstance().createProgram("Torch",
                                                   SHADERS_DIR "TorchShader.vert", SHADERS_DIR "TorchShader.frag");
	_program = programManager::sharedInstance().programWithID("Torch");
    _LightPositionUniform[0] = glGetUniformLocation(_program, "light_position[0]");
    _LightPositionUniform[1] = glGetUniformLocation(_program, "light_position[1]");
    _LightPositionUniform[2] = glGetUniformLocation(_program, "light_position[2]");
    _LightColorUniform[0] = glGetUniformLocation(_program, "light_color[0]");
    _LightColorUniform[1] = glGetUniformLocation(_program, "light_color[1]");
    _LightColorUniform[2] = glGetUniformLocation(_program, "light_color[2]");
    
    Camera * camera = Camera::instance();
    mat4 sc = scale(mat4(1.0f), vec3(0.1f,0.05f,0.1f));
    mat4 translation = translate(mat4(1.0f), camera->getTarget());
    _object->init(_program,"obj_files/torch.obj");
    _object->setScale(sc);
    _object->setTranslation(translation);
    _flame->init();
    _flame->setTranslation(translation);
}
void Torch::update(int globalX, int globalY)
{
    
    float x = 2.0f * ((float)globalX - (float)_global_width/2.0f)/(float)_global_width;
	float y = 2.0f * ((float)_global_high/2.0f-(float)globalY)/(float)_global_high;
    _globalPos = vec2(x,y);
    vec3 pos(x,y,0.0f);
    mat4 translation = translate(mat4(1.0f),pos);
    _object->setTranslation(translation);
    _flame->setTranslation(translation);
}
void Torch::draw()
{
    if(_mode == REG){
        _flame->draw();
    }
    glUseProgram(_program);
    glUniform3fv(_LightPositionUniform[0], 1, value_ptr(LIGHT1_POSITION));
    glUniform3fv(_LightPositionUniform[1], 1, value_ptr(LIGHT2_POSITION));
    glUniform3fv(_LightPositionUniform[2], 1, value_ptr(Torch::instance()->getPosition()));
    glUniform3fv(_LightColorUniform[0], 1, value_ptr(LIGHT1_COLOR));
    glUniform3fv(_LightColorUniform[1], 1, value_ptr(LIGHT2_COLOR));
    glUniform3fv(_LightColorUniform[2], 1, value_ptr(LIGHT3_COLOR));

    _object->draw();
    glUseProgram(0);
}
void Torch::switchMode()
{
    if (_mode == LIGHT){
        _mode = REG;
    }
    else{
        _mode = LIGHT;
    }
}
vec3 Torch::getPosition(){
    vec3 pos = _globalPos[0] * Camera::instance()->getSide() + _globalPos[1] * Camera::instance()->getUp() + Camera::instance()->getTarget();
    return pos;
}
