//
//  Table.cpp
//  project
//
//  Draws table in flags scene
//
//  Created by Alona Bas on 9/18/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Table.h"

Table::Table()
{
    _obj = new MeshObject();
}

Table::~Table()
{
    if(_obj != NULL){
        delete _obj;
        _obj = NULL;
    }
}

void Table::init()
{
    programManager::sharedInstance().createProgram("table",
                                                   SHADERS_DIR "TableShader.vert", SHADERS_DIR "TableShader.frag");
	_program = programManager::sharedInstance().programWithID("table");
    _LightPositionUniform[0] = glGetUniformLocation(_program, "light_position[0]");
    _LightPositionUniform[1] = glGetUniformLocation(_program, "light_position[1]");
    _LightPositionUniform[2] = glGetUniformLocation(_program, "light_position[2]");
    _LightColorUniform[0] = glGetUniformLocation(_program, "light_color[0]");
    _LightColorUniform[1] = glGetUniformLocation(_program, "light_color[1]");
    _LightColorUniform[2] = glGetUniformLocation(_program, "light_color[2]");

    _obj->init(_program,"obj_files/table.off");
    mat4 scale = glm::scale(mat4(1.0f), vec3(5.0f,1.75f,1.0f));
    _obj->setScale(scale);
    mat4 translating = translate(mat4(1.0f), vec3(0.0f,-2.0f,-3.0f));
    _obj->setTranslation(translating);
}

void Table::draw()
{
    glUseProgram(_program);
    glUniform3fv(_LightPositionUniform[0], 1, value_ptr(LIGHT1_POSITION));
    glUniform3fv(_LightPositionUniform[1], 1, value_ptr(LIGHT2_POSITION));
    glUniform3fv(_LightPositionUniform[2], 1, value_ptr(Torch::instance()->getPosition()));
    glUniform3fv(_LightColorUniform[0], 1, value_ptr(LIGHT1_COLOR));
    glUniform3fv(_LightColorUniform[1], 1, value_ptr(LIGHT2_COLOR));
    glUniform3fv(_LightColorUniform[2], 1, value_ptr(LIGHT3_COLOR));
    _obj->draw();
    glUseProgram(0);
}