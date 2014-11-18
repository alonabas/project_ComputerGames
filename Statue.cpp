//
//  Statue.cpp
//  project
//
//  Create statue
//
//  Created by Alona Bas on 9/18/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Statue.h"
#include "stb_image.h"

Statue::Statue() : Object()
{
    _mesh = new MeshObject();
}
Statue::~Statue()
{
    if(_mesh != NULL){
        delete _mesh;
        _mesh = NULL;
    }
}
void Statue::init(vec3 center)
{
    
    programManager::sharedInstance().createProgram("statue",
                                                   SHADERS_DIR "StatueShader.vert", SHADERS_DIR "StatueShader.frag");
	_program = programManager::sharedInstance().programWithID("statue");
    _LightPositionUniform[0] = glGetUniformLocation(_program, "light_position[0]");
    _LightPositionUniform[1] = glGetUniformLocation(_program, "light_position[1]");
    _LightPositionUniform[2] = glGetUniformLocation(_program, "light_position[2]");
    _LightColorUniform[0] = glGetUniformLocation(_program, "light_color[0]");
    _LightColorUniform[1] = glGetUniformLocation(_program, "light_color[1]");
    _LightColorUniform[2] = glGetUniformLocation(_program, "light_color[2]");

    _mesh->init(_program,"obj_files/Lion.obj");
    
    _bound._radius = _mesh->getRadius();
    
    _modelScale = glm::scale(mat4(1.0f), vec3(0.08f));
    _mesh->setScale(_modelScale);
    
    _modelTranslate = translate(mat4(1.0f), center);
    _mesh->setTranslation(_modelTranslate);
    
    _modelRotate = rotate(mat4(1.0f),-90.0f, vec3(1.0f,0.0f,0.0f));
    _mesh->setRotation(_modelRotate);
    
    _bound._center = center;
    _bound._radius = _bound._radius * 0.08f;
}

void Statue::draw()
{
    if(_found){
        _mesh->setTranslation(_modelTranslate);
    }
    glUseProgram(_program);
    glUniform3fv(_LightPositionUniform[0], 1, value_ptr(LIGHT1_POSITION));
    glUniform3fv(_LightPositionUniform[1], 1, value_ptr(LIGHT2_POSITION));
    glUniform3fv(_LightPositionUniform[2], 1, value_ptr(Torch::instance()->getPosition()));
    glUniform3fv(_LightColorUniform[0], 1, value_ptr(LIGHT1_COLOR));
    glUniform3fv(_LightColorUniform[1], 1, value_ptr(LIGHT2_COLOR));
    glUniform3fv(_LightColorUniform[2], 1, value_ptr(LIGHT3_COLOR));

    _mesh->draw();
    glUseProgram(0);
}