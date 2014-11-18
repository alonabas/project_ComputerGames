//
//  Sky.cpp
//  project
//
//  Sky animation
//
//  Created by Alona Bas on 9/28/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Sky.h"
#include "stb_image.h"

Sky::Sky(): _time(0), _vao(0), _vbo(0), _ebo(0), _vboUV(0), _modelMat(mat4(1.0f))
{
}

Sky::~Sky()
{
    if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);
    if (_vboUV != 0)
		glDeleteBuffers(1, &_vboUV);
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
}

void Sky::init()
{
    vec2 uvs[SKY_COUNT];
    vec3 vertice[SKY_COUNT];
    _size = (SKY_POINTS_HIGH-1)*(SKY_POINTS_WIDTH-1) * 6;
    for (int x = 0; x < SKY_POINTS_HIGH; x++){
        for(int y=0; y < SKY_POINTS_WIDTH; y++){
            vertice[SKY_INDX(x, y)] = vec3(((float)x) * SKY_SCALE_FACTOR_X - (float)(SKY_SCALE_FACTOR_X / 2.0f), ((float)y) * SKY_SCALE_FACTOR_Y - (float)(SKY_SCALE_FACTOR_Y / 2.0f), 0.0f);
            uvs[SKY_INDX(x, y)] = vec2(1.0f - (float)x,1.0f - (float)y);
        }
    }
    //generate indices
    int k=0;
    GLuint indice[_size];
    for (int x = 0; x < SKY_POINTS_HIGH - 1; x++){
        for (int y = 0; y < SKY_POINTS_WIDTH - 1; y++) {
            // look at square and devide to 2 triangles
            
            if((x&1 && y&1) || !((x+y)&1)){
                indice[k] = SKY_INDX(x, (y + 1));
                k++;
                indice[k] = SKY_INDX(x, y);
                k++;
                indice[k] = SKY_INDX((x + 1),y);
                k++;
                // second triangle
                indice[k] = SKY_INDX(x, (y + 1));
                k++;
                indice[k] = SKY_INDX((x + 1), (y + 1));
                k++;
                indice[k] = SKY_INDX((x + 1),y);
                
            }
            else{
                indice[k] = SKY_INDX(x, y);
                k++;
                indice[k] = SKY_INDX((x + 1), (y + 1));
                k++;
                indice[k] = SKY_INDX((x + 1),y);
                k++;
                // second triangle
                indice[k] = SKY_INDX(x, (y + 1));
                k++;
                indice[k] = SKY_INDX((x + 1), (y + 1));
                k++;
                indice[k] = SKY_INDX(x, y);
            }
            k++;
        }
    }
    // get uniforms
    programManager::sharedInstance().createProgram("Sky",
                                                   SHADERS_DIR "SkyShader.vert", SHADERS_DIR "SkyShader.frag");
	_program = programManager::sharedInstance().programWithID("Sky");
    _timeUniform = glGetUniformLocation(_program, "time");
	_resolutionUniform = glGetUniformLocation(_program, "resolution");
	_Muniform = glGetUniformLocation(_program, "modelMat");
	_Vuniform = glGetUniformLocation(_program, "viewMat");
	_Puniform = glGetUniformLocation(_program, "projMat");

	// atributes
	_attrib[0] = glGetAttribLocation(_program, "position");
    _attrib[1] = glGetAttribLocation(_program, "uv");
    
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
    
	glBindVertexArray(0);
}


void Sky::draw()
{
	Camera* camera = Camera::instance();
    glUseProgram(_program);
    glUniform1f(_timeUniform, (_time++) / 120.0f);
    glUniform2fv(_resolutionUniform, 1, value_ptr(vec2(800.0f, 800.0f)));
	glUniformMatrix4fv(_Muniform, 1, GL_FALSE, value_ptr(_modelMat));
	glUniformMatrix4fv(_Vuniform, 1, GL_FALSE, value_ptr(camera->getV()));
	glUniformMatrix4fv(_Puniform, 1, GL_FALSE, value_ptr(camera->getP()));

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
