//
//  Model.h
//  cg-projects
//
//  Main screen
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __project__MainModel__
#define __project__MainModel__

#include "Camera.h"
#include "SkyBox.h"
#include "Text.h"
#include "PPBObject.h"
#include "Sound.h"
#include "Defs.h"
#include "Mirror.h"
#include "Statue.h"
#include "RefractingObject.h"
#include "SubScene.h"
#include "Model.h"
#include "Frame.h"

#define NUMBER_OF_OBJECTS 3

class MainModel : public Model {
    const int MAP[NUMBER_OF_OBJECTS] = {0,1,6};
	Camera* _camera;
	SkyBox* _skyBox;
    Frame* _frame;
    Object* _objects[NUMBER_OF_OBJECTS];
    SubScene* _sub;
    int _curFound;
public:
	MainModel();
	virtual ~MainModel();
	void init();
	void draw();
    bool isHit(int x, int y);
    void remove();
    int getFound();
};

#endif /* defined(__project__Model__) */
