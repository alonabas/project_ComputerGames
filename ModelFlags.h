//
//  ModelFlags.h
//  project
//
//  Class that defines flag scene
//
//  Created by Alona Bas on 9/13/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__ModelFlags__
#define __project__ModelFlags__

#include <iostream>
#include "Defs.h"
#include "Camera.h"
#include "Table.h"
#include "Flag.h"
#include "Background.h"
#include "Model.h"
#include "Frame.h"
#include "Globals.h"

#define NUMBER_FLAGS 7
#define NUMBER_HIDDEN_FLAGS 4

class ModelFlags : public Model{
    int HIDDEN_FLAGS[NUMBER_HIDDEN_FLAGS] = {1,3,6,0};
    const char* _textures[NUMBER_FLAGS] = {(char*)"Cambodia.png", (char*)"South_Africa.png", (char*)"Uruguay.png", (char*)"malta.png", (char*)"Papua_New_Guinea.png",(char*)"luxembourg.png",(char*)"Venezuela.png"};
    Flag* _flag;
    int _curFound;
    Flag* _flags[NUMBER_FLAGS];
    Table* _table;
	Camera* _camera;
    Background * _background;
public:
    ModelFlags();
	virtual ~ModelFlags();
	void init();
	void draw();
    bool isHit(int x, int y);
    void remove();
    int getFound();
};

#endif /* defined(__project__ModelFlags__) */
