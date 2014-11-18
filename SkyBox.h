//
//  SkyBox.h
//  project
//
//  Creates skybox
//
//  Created by Alona Bas on 4/24/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__SkyBox__
#define __project__SkyBox__

#include "bimage.h"
#include "ShaderIO.h"
#include "Camera.h"
#include "bimage.h"
#include "Globals.h"
#include "Torch.h"

class SkyBox {
	const char* _textureFiles[NUMBER_SKYBOX_TEXTURES] = {
			(char*) "skybox/a_stitch_Left.bmp",
			(char*) "skybox/a_stitch_Right.bmp",
			(char*) "skybox/a_stitch_Bottom.bmp",
			(char*) "skybox/a_stitch_Top.bmp",
			(char*) "skybox/a_stitch_Back.bmp",
			(char*) "skybox/a_stitch_Front.bmp" };
    const char* _textureDepthFiles[NUMBER_SKYBOX_TEXTURES] = {
        (char*) "skybox/a_Depth_stitch_Left.bmp",
        (char*) "skybox/a_Depth_stitch_Right.bmp",
        (char*) "skybox/a_Depth_stitch_Bottom.bmp",
        (char*) "skybox/a_Depth_stitch_Top.bmp",
        (char*) "skybox/a_Depth_stitch_Back.bmp",
        (char*) "skybox/a_Depth_stitch_Front.bmp" };
    const char* _textureSkyFiles[NUMBER_SKYBOX_TEXTURES] = {
        (char*) "skybox/a_SkyAlpha_stitch_Left.bmp",
        (char*) "skybox/a_SkyAlpha_stitch_Right.bmp",
        (char*) "skybox/a_SkyAlpha_stitch_Bottom.bmp",
        (char*) "skybox/a_SkyAlpha_stitch_Top.bmp",
        (char*) "skybox/a_SkyAlpha_stitch_Back.bmp",
        (char*) "skybox/a_SkyAlpha_stitch_Front.bmp" };
    const char* _textureNormalFiles[NUMBER_SKYBOX_TEXTURES] = {
        (char*) "skybox/a_normal_stitch_Left.bmp",
        (char*) "skybox/a_normal_stitch_Right.bmp",
        (char*) "skybox/a_normal_stitch_Bottom.bmp",
        (char*) "skybox/a_normal_stitch_Top.bmp",
        (char*) "skybox/a_normal_stitch_Back.bmp",
        (char*) "skybox/a_normal_stitch_Front.bmp" };
	GLuint _program;
	GLuint _WVPuniform, _textureUniform[4], _camUniform, _cameraSideUniform, _cameraUpUniform, _cameraDirUniform, _timeUniform,_resolutionUniform,_LightPosUniform, _LightColorUniform;
    GLuint _attrib;
	GLuint _textureBuffer[4], _vao, _vbo;
	Camera* _camera;
    int _time;
public:
	SkyBox();
	void init(Camera* camera);
	void draw();
	virtual ~SkyBox();
};

#endif /* defined(__project__SkyBox__) */
