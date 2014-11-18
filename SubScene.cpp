//
//  SubScene.h
//  project
//
//  Image of flags scene in main scene
//  Created by Alona Bas on 9/15/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "stb_image.h"
#include "SubScene.h"

SubScene::SubScene(): Object(), _vao(0), _vbo(0), _ebo(0), _vboUV(0),_textureBuffer(0)
{
}

SubScene::~SubScene()
{
    if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);
    if (_vboUV != 0)
		glDeleteBuffers(1, &_vboUV);
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
    if (_textureBuffer != 0)
		glDeleteTextures(1, &_textureBuffer);
    glDeleteProgram(_program);
}

void SubScene::init(vec3 position)
{
    vec3 vertice[SUB_COUNT];
    vec2 uvs[SUB_COUNT];
    _size = (SUB_POINTS_HIGH - 1) * (SUB_POINTS_WIDTH - 1) * 6;
    for (int x = 0; x < SUB_POINTS_HIGH; x++){
        for(int y=0; y < SUB_POINTS_WIDTH; y++){ // y=c
            vertice[SUB_INDX(x,y)] = vec3(((float)x) * SUBSCENE_SCALE_FACTOR_X, ((float)y) * SUBSCENE_SCALE_FACTOR_Y, 0.0f);
            uvs[SUB_INDX(x,y)] = vec2((float)x, 1.0f - (float)y);
        }
    }
    //generate indices
    int k=0;
    GLuint indice[_size];
    for (int x = 0; x<SUB_POINTS_HIGH-1;x++){
        for (int y = 0; y < SUB_POINTS_WIDTH-1; y++) {
            // look at square and devide to 2 triangles
            
            if((x&1 && y&1) || !((x+y)&1)){
                indice[k] = SUB_INDX(x, (y + 1));
                k++;
                indice[k] = SUB_INDX(x,y);
                k++;
                indice[k] = SUB_INDX((x + 1),y);
                k++;
                // second triangle
                indice[k] = SUB_INDX(x, (y + 1));
                k++;
                indice[k] = SUB_INDX((x + 1), (y + 1));
                k++;
                indice[k] = SUB_INDX((x + 1),y);
                
            }
            else{
                indice[k] = SUB_INDX(x,y);
                k++;
                indice[k] = SUB_INDX((x + 1), (y + 1));
                k++;
                indice[k] = SUB_INDX((x + 1),y);
                k++;
                // second triangle
                indice[k] = SUB_INDX(x, (y + 1));
                k++;
                indice[k] = SUB_INDX((x + 1), (y + 1));
                k++;
                indice[k] = SUB_INDX(x,y);
            }
            k++;
        }
    }
    // get uniforms
    programManager::sharedInstance().createProgram("SubScene",
                                                   SHADERS_DIR "SubScene.vert", SHADERS_DIR "SubScene.frag");
	_program = programManager::sharedInstance().programWithID("SubScene");
	_textureUniform = glGetUniformLocation(_program, "textureData");
    _Muniform = glGetUniformLocation(_program, "modelMat");
    _Vuniform = glGetUniformLocation(_program, "viewMat");
    _Puniform = glGetUniformLocation(_program, "projMat");
    _LightPositionUniform = glGetUniformLocation(_program, "light_position");
    _LightColorUniform = glGetUniformLocation(_program, "light_color");

	// atributes
	_attrib[0] = glGetAttribLocation(_program, "position");
    _attrib[1] = glGetAttribLocation(_program, "uv");
    
    int w, h, comp;
    unsigned char *data;
    data = stbi_load("sub_scene.png", &w, &h, &comp, STBI_rgb_alpha);
    
	glGenTextures(1, &_textureBuffer);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if(comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if(comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	// Create and load vertex data into a Vertex Buffer Object:
	glGenBuffers(1, &_vbo);
	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice),
                 &(vertice[0]), GL_STATIC_DRAW);
    
	// Create and load face (elements) data into an Element Buffer Object:
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice),
                 &(indice[0]), GL_STATIC_DRAW);
	// Obtain attribute handles:
    
	glEnableVertexAttribArray(_attrib[0]);
	glVertexAttribPointer(_attrib[0], // attribute handle
                          3,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE, 0, 0);
    
    glGenBuffers(1, &_vboUV);
	glBindBuffer(GL_ARRAY_BUFFER, _vboUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs),
                 &(uvs[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(_attrib[1]);
	glVertexAttribPointer(_attrib[1], // attribute handle
                          2,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE, 0, 0);
    
    _bound._center = position;
    _bound._radius = SUBSCENE_SCALE_FACTOR_X;
    _modelTranslate = translate(mat4(1.0f), position);
	glBindVertexArray(0);
    _moveToCenter = mat4(1.0f);
}


void SubScene::draw()
{
    // update data
    _modelMat = _modelRotate * _modelTranslate * _modelScale * _moveToCenter;
    Camera* camera = Camera::instance();
    glUseProgram(_program);
    glUniformMatrix4fv(_Muniform, 1, GL_FALSE, value_ptr(_modelMat));
    glUniformMatrix4fv(_Vuniform, 1, GL_FALSE, value_ptr(camera->getV()));
    glUniformMatrix4fv(_Puniform, 1, GL_FALSE, value_ptr(camera->getP()));
    glUniform3fv(_LightPositionUniform, 1, value_ptr(Torch::instance()->getPosition()));
    glUniform3fv(_LightColorUniform, 1, value_ptr(LIGHT3_COLOR));

    // Bind texture
	glUniform1i(_textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer);
    
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(_vao);
    
	glDrawElements(GL_TRIANGLES, (int) _size, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	// Unbind the Vertex Array object
	glBindVertexArray(0);
    glUseProgram(0);
}

