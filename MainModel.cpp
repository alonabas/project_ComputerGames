//
//  Model.h
//  cg-projects
//
//  Main screen
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "MainModel.h"
extern int _global_width;
extern int _global_high;

MainModel::MainModel() : Model(), _curFound(-1){
    _camera = Camera::instance();
    _objects[0] = new Statue();
    _objects[1] = new RefractingObject();
    _objects[2] = new Mirror();
    _frame = Frame::instance();
    _sub = new SubScene();
    _skyBox = new SkyBox();
}

MainModel::~MainModel() {
	if (_skyBox != NULL){
		delete _skyBox;
        _skyBox = NULL;
    }
	if (_camera != NULL){
		delete _camera;
        _camera = NULL;
    }
	if (_objects[0] != NULL){
		delete _objects[0];
        _objects[0] = NULL;
    }
	if (_objects[1] != NULL){
		delete _objects[1];
        _objects[1] = NULL;
    }
	if (_objects[2] != NULL){
		delete _objects[2];
        _objects[2] = NULL;
    }
	if (_frame != NULL){
		delete _frame;
        _frame = NULL;
    }
	if (_sub != NULL){
		delete _sub;
        _sub = NULL;
    }
}

void MainModel::init() {
    _frame->init();
	_skyBox->init(_camera);
    _objects[0]->init(1.75f * vec3(0.479938, -0.1, 0.234065));
    _objects[1]->init(0.89f * vec3(0.184077, -0.229385, -0.955771));
    _objects[2]->init(0.51f * vec3(-0.703922, -0.234641, 0.670402));
    _sub->init(0.6f * vec3(-0.603133, -0.186026, -0.775645));
}

void MainModel::draw() {
    _camera = Camera::instance();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _skyBox->draw();
    for (Object* obj : _objects){
        if(obj->isFound() == false){
            obj->draw();
        }
    }
     _sub->draw();
    if (_status == ANIMATION){
        if(_animation_count < ANIMATION_TIME){
            _objects[_curFound]->animate(true,(TIME_UNIT));
        }
        else if(_animation_count < 1.5 * ANIMATION_TIME){
            _objects[_curFound]->animate(false,(TIME_UNIT));
            (Frame::instance())->draw();
        }
        else if(_animation_count < 1.5 * ANIMATION_TIME + 3){
            (Frame::instance())->draw();
        }
        else if(_animation_count < 2 * ANIMATION_TIME + 6){
            _objects[_curFound]->animate(true,(TIME_UNIT));
            (Frame::instance())->draw();
        }
        else if(_animation_count < 2 * ANIMATION_TIME + 12){
            (Frame::instance())->draw();
        }
        else{
            _animation_count = 0;
            _status = REGULAR;
            _curFound = -1;
            return;
        }
        _animation_count ++;
        _objects[_curFound]->draw();
    }
    glUseProgram(0);
}

bool MainModel::isHit(int x, int y)
{
    float my_x = ((float)x - (float)_global_width/2.0f)/(float)_global_width;
	float my_y = ((float)_global_high/2.0f-(float)y)/(float)_global_high;
    
    Camera * camera = Camera::instance();
    vec3 ray = my_x * _camera->getSide() + my_y * _camera->getUp() +camera->getTarget();
    for (int i = 0; i<NUMBER_OF_OBJECTS; i++){
        if((_objects[i]->isFound()) == false && _objects[i]->isInside(ray)){
            _curFound = i;
            _status = ANIMATION;
            return true;
        }
    }
    if(_sub->isInside(ray)){
        _curFound = 100;
        return true;
    }
    return false;
}
void MainModel::remove()
{
    if(_curFound != 100 && _curFound != -1){
        _objects[_curFound]->found();
    }
}
int MainModel::getFound()
{
    if(_curFound != 100){
        return MAP[_curFound];
    }
    return _curFound;
}