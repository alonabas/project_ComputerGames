//
//  RefractingObject.cpp
//  project
//
//  Class that defines Refracting (glass) object
//
//  Created by Alona Bas on 9/15/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "RefractingObject.h"

RefractingObject::RefractingObject(): Object(),_textureBuffer(0)
{
    _mesh = new MeshObject();
}

RefractingObject::~RefractingObject()
{
    if (_textureBuffer != 0)
		glDeleteTextures(1, &_textureBuffer);
    if(_mesh != NULL){
        delete _mesh;
        _mesh = NULL;
    }
}

void RefractingObject::init(vec3 center)
{
    // get uniforms
    programManager::sharedInstance().createProgram("RefractedObject",
                                                   SHADERS_DIR "RefractedObject.vert", SHADERS_DIR "RefractedObject.frag");
	_program = programManager::sharedInstance().programWithID("RefractedObject");
	_textureUniform = glGetUniformLocation(_program, "textureData");
    
    glGenTextures(1, &_textureBuffer);
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer);
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
    _LightPositionUniform[0] = glGetUniformLocation(_program, "light_position[0]");
    _LightPositionUniform[1] = glGetUniformLocation(_program, "light_position[1]");
    _LightPositionUniform[2] = glGetUniformLocation(_program, "light_position[2]");
    _LightColorUniform[0] = glGetUniformLocation(_program, "light_color[0]");
    _LightColorUniform[1] = glGetUniformLocation(_program, "light_color[1]");
    _LightColorUniform[2] = glGetUniformLocation(_program, "light_color[2]");

    _mesh->init(_program, "obj_files/beetle.off");
    _modelTranslate = translate(mat4(1.0f), center);
    _modelRotate = rotate(mat4(1.0f), 60.0f, vec3(0.0f,1.0f,0.0f));
    _modelScale = scale(mat4(1.0f), vec3(0.15f));
    _mesh->setTranslation(_modelTranslate);
    _mesh->setRotation(_modelRotate);
    _mesh->setScale(_modelScale);
	glBindVertexArray(0);
    _bound._center = center;
    _bound._radius = _mesh->getRadius() * 0.15f;
    
}
void RefractingObject::draw()
{
    if(_found){
        _mesh->setTranslation(_modelTranslate);
    }
    glUseProgram(_program);
    // Bind texture
	glUniform1i(_textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer);
    glUniform3fv(_LightPositionUniform[0], 1, value_ptr(LIGHT1_POSITION));
    glUniform3fv(_LightPositionUniform[1], 1, value_ptr(LIGHT2_POSITION));
    glUniform3fv(_LightPositionUniform[2], 1, value_ptr(Torch::instance()->getPosition()));
    glUniform3fv(_LightColorUniform[0], 1, value_ptr(LIGHT1_COLOR));
    glUniform3fv(_LightColorUniform[1], 1, value_ptr(LIGHT2_COLOR));
    glUniform3fv(_LightColorUniform[2], 1, value_ptr(LIGHT3_COLOR));
    _mesh->draw();
	// Unbind the Vertex Array object
	glBindVertexArray(0);
    glUseProgram(0);    
}
