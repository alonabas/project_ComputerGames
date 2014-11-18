//
//  Mirror.h
//  project
//
//  Class that dreates and displayes mirror
//
//  Created by Alona Bas on 9/15/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Mirror.h"
#include "stb_image.h"

Mirror::Mirror(): Object(),_time(0), _vao(0), _vbo(0), _ebo(0), _vboNormal(0), _vboUV(0),_textureBuffer{0}
{
    _frame = new MeshObject();
}

Mirror::~Mirror()
{
    if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);
	if (_vboNormal != 0)
		glDeleteBuffers(1, &_vboNormal);
    if (_vboUV != 0)
		glDeleteBuffers(1, &_vboUV);
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
    if (_textureBuffer != 0)
		glDeleteVertexArrays(2, _textureBuffer);
}

void Mirror::init(vec3 center)
{
    vec3 vertice[MIRROR_COUNT];
    vec3 normals[MIRROR_COUNT];

    vec2 uvs[MIRROR_COUNT];
    _size = (MIRROR_POINTS_HIGH - 1)*(MIRROR_POINTS_WIDTH - 1) * 6;
    for (int x = 0; x < MIRROR_POINTS_HIGH; x++){
        for(int y=0; y < MIRROR_POINTS_WIDTH; y++){ // y=c
            vertice[MIRROR_INDX(x,y)] = vec3(((float)x - 0.52f)*MIRROR_SCALE_FACTOR_X,(1.2f - (float)y)*MIRROR_SCALE_FACTOR_Y,0.0f);
            normals[MIRROR_INDX(x,y)] = vec3(0.0f,0.0f,-1.0f);
            uvs[MIRROR_INDX(x,y)] = vec2((float)x,1.0f-(float)y);
        }
    }
    //generate indices
    int k=0;
    GLuint indice[_size];
    for (int x = 0; x < MIRROR_POINTS_HIGH - 1; x++){
        for (int y = 0; y < MIRROR_POINTS_WIDTH - 1; y++) {
            // look at square and devide to 2 triangles
            
            if((x&1 && y&1) || !((x+y)&1)){
                indice[k] = MIRROR_INDX(x,(y + 1));
                k++;
                indice[k] = MIRROR_INDX(x,y);
                k++;
                indice[k] = MIRROR_INDX((x + 1),y);
                k++;
                // second triangle
                indice[k] = MIRROR_INDX(x,(y + 1));
                k++;
                indice[k] = MIRROR_INDX((x + 1),(y + 1));
                k++;
                indice[k] = MIRROR_INDX((x + 1),y);
                
            }
            else{
                indice[k] = MIRROR_INDX(x,y);
                k++;
                indice[k] = MIRROR_INDX((x + 1),(y + 1));
                k++;
                indice[k] = MIRROR_INDX((x + 1),y);
                k++;
                // second triangle
                indice[k] = MIRROR_INDX(x,(y + 1));
                k++;
                indice[k] = MIRROR_INDX((x + 1),(y + 1));
                k++;
                indice[k] = MIRROR_INDX(x,y);
            }
            k++;
        }
    }
    // get uniforms
    programManager::sharedInstance().createProgram("Mirror",
                                                   SHADERS_DIR "MirrorShader.vert", SHADERS_DIR "MirrorShader.frag");
	_program = programManager::sharedInstance().programWithID("Mirror");
	_textureUniform[0] = glGetUniformLocation(_program, "textureData");
    _textureUniform[1] = glGetUniformLocation(_program, "textureGhost");
	_MnormalUniform = glGetUniformLocation(_program, "normalMat");
    _Muniform = glGetUniformLocation(_program, "modelMat");
    _Vuniform = glGetUniformLocation(_program, "viewMat");
    _Puniform = glGetUniformLocation(_program, "projMat");
    _typeUniform = glGetUniformLocation(_program, "type");
    _LightPositionUniform[0] = glGetUniformLocation(_program, "light_position[0]");
    _LightPositionUniform[1] = glGetUniformLocation(_program, "light_position[1]");
    _LightPositionUniform[2] = glGetUniformLocation(_program, "light_position[2]");
    _LightColorUniform[0] = glGetUniformLocation(_program, "light_color[0]");
    _LightColorUniform[1] = glGetUniformLocation(_program, "light_color[1]");
    _LightColorUniform[2] = glGetUniformLocation(_program, "light_color[2]");
    _timeUniform = glGetUniformLocation(_program, "time");

	// atributes
	_attrib[0] = glGetAttribLocation(_program, "position");
	_attrib[1] = glGetAttribLocation(_program, "normal");
    _attrib[2] = glGetAttribLocation(_program, "uv");
    
    glGenTextures(2, _textureBuffer);
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

    int w, h, comp;
    unsigned char *data;
    data = stbi_load("ghost.png", &w, &h, &comp, STBI_rgb_alpha);
    
	glBindTexture(GL_TEXTURE_2D, _textureBuffer[1]);
    
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
    
	glGenBuffers(1, &_vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, _vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals),
                 &(normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(_attrib[1]);
	glVertexAttribPointer(_attrib[1], // attribute handle
                          3,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE, 0, 0);
    
    glGenBuffers(1, &_vboUV);
	glBindBuffer(GL_ARRAY_BUFFER, _vboUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs),
                 &(uvs[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(_attrib[2]);
	glVertexAttribPointer(_attrib[2], // attribute handle
                          2,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE, 0, 0);
    
    _frame->init(_program, "obj_files/mirror_frame.off");
    
    
    _modelTranslate = translate(_modelTranslate, center);
    
    _frame->setTranslation(_modelTranslate);
    
    mat4 rot = rotate(mat4(1.0f), -180.0f, vec3(0.0f,1.0f,0.0f));
    _frame->setRotation(rot);
    
    mat4 sc = scale(mat4(1.0f), vec3(0.075f,0.08f,0.08f));
    _frame->setScale(sc);
   
    _bound._center = center;
    _bound._radius = _frame->getRadius() * 0.08f;
    
	glBindVertexArray(0);
    
}
void Mirror::draw()
{
	_time++;
	if (_time == 50) {
		_time = -50;
	}
    if(_found){
        _frame->setTranslation(_modelTranslate);
    }
    Camera* camera = Camera::instance();
    mat4 modelMat = _modelRotate * _modelTranslate * _modelScale * _moveToCenter;
	glm::mat4x4 model_vew_mat = camera->getV() * modelMat;
	// inverse transpose for normals
	glm::mat4 normalMat = glm::transpose(glm::inverse(model_vew_mat));
    glUseProgram(_program);
    glDisable(GL_DEPTH_TEST);
    glUniform1i(_typeUniform, 1);
    _frame->draw();
    glUniform1i(_typeUniform, 0);
	glUniformMatrix4fv(_MnormalUniform, 1, GL_FALSE, value_ptr(normalMat));
    glUniformMatrix4fv(_Muniform, 1, GL_FALSE, value_ptr(modelMat));
    glUniformMatrix4fv(_Vuniform, 1, GL_FALSE, value_ptr(camera->getV()));
    glUniformMatrix4fv(_Puniform, 1, GL_FALSE, value_ptr(camera->getP()));
    glUniform3fv(_LightPositionUniform[0], 1, value_ptr(LIGHT1_POSITION));
    glUniform3fv(_LightPositionUniform[1], 1, value_ptr(LIGHT2_POSITION));
    glUniform3fv(_LightPositionUniform[2], 1, value_ptr(Torch::instance()->getPosition()));
    glUniform3fv(_LightColorUniform[0], 1, value_ptr(LIGHT1_COLOR));
    glUniform3fv(_LightColorUniform[1], 1, value_ptr(LIGHT2_COLOR));
    glUniform3fv(_LightColorUniform[2], 1, value_ptr(LIGHT3_COLOR));
    glUniform1f(_timeUniform, (float)abs(_time));
    // Bind texture
	glUniform1i(_textureUniform[0], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureBuffer[0]);
    
    glUniform1i(_textureUniform[1], 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _textureBuffer[1]);
    
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	glBindVertexArray(_vao);
    
	glDrawElements(GL_TRIANGLES, (int) _size, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	// Unbind the Vertex Array object
	glBindVertexArray(0);
    glUseProgram(0);
    
}

