//
//  ModelFlags.cpp
//  project
//
//  Class that defines flag scene
//
//  Created by Alona Bas on 9/13/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "ModelFlags.h"

extern int _global_width;
extern int _global_high;

ModelFlags::ModelFlags() : Model(), _curFound(-1)
{
    for(int i = 0; i< NUMBER_FLAGS; i++){
        _flags[i] = new Flag(_textures[i],_textures[i]);
    }
    _table = new Table();
    _background = new Background();
}
ModelFlags::~ModelFlags()
{
    for(int i = 0; i< NUMBER_FLAGS; i++){
        if (_flags[i] != NULL){
            delete _flags[i];
            _flags[i] = NULL;
        }
    }
    if(_table != NULL){
        delete _table;
        _table = NULL;
    }
    if(_background != NULL){
        delete _background;
        _background = NULL;
    }
}
void ModelFlags::init()
{
    _camera = Camera::instance();
    float x = -6.0f;
    float y = -0.2f;
    float z = -3.0f;
   
    for(int i = 0; i< NUMBER_FLAGS; i++){
        _flags[i]->init(vec3(x,y,z));
        x += 2.0f;
        if(i%2 == 0){
            z = -3.5f;
            y = -0.3f;
        }
        else{
            z = -3.0f;
            y = -0.2f;
        }
    }
    _table->init();
    _background->init();
    
}
void ModelFlags::draw()
{
    _background->draw();
    for(int i = 0; i< NUMBER_FLAGS; i++){
        if(_flags[i]->isFound() == false){
            _flags[i]->draw();
        }
    }
    _table->draw();
    if (_status == ANIMATION){
        if(_animation_count < ANIMATION_TIME){
            _flags[HIDDEN_FLAGS[_curFound]]->animate(true,TIME_UNIT);
        }
        else if(_animation_count < 1.5 * ANIMATION_TIME){
            _flags[HIDDEN_FLAGS[_curFound]]->animate(false, TIME_UNIT);
            (Frame::instance())->draw();
        }
        else if(_animation_count < 1.5 * ANIMATION_TIME + 3){
            (Frame::instance())->draw();
        }
        else if(_animation_count < 2 * ANIMATION_TIME + 6){
            _flags[HIDDEN_FLAGS[_curFound]]->animate(true, TIME_UNIT);
            (Frame::instance())->draw();
        }
        else if(_animation_count < 2 * ANIMATION_TIME + 12){
            (Frame::instance())->draw();
        }
        else{
            _animation_count = 0;
            _status = REGULAR;
            HIDDEN_FLAGS[_curFound] = -1;
            _curFound = -1;
            return;

        }
        _animation_count ++;
        _flags[HIDDEN_FLAGS[_curFound]]->draw();
    }
}

bool ModelFlags::isHit(int x, int y)
{
    float my_x = ((float)x - (float)_global_width/2.0f)/(float)_global_width;
	float my_y = ((float)_global_high/2.0f-(float)y)/(float)_global_high;
    Camera * camera = Camera::instance();
    vec3 ray = my_x * _camera->getSide() + my_y * _camera->getUp() +(camera->getTarget() - camera->getEye());
    for (int i = 0; i<NUMBER_HIDDEN_FLAGS; i++){
        if (HIDDEN_FLAGS[i] != -1 && _flags[HIDDEN_FLAGS[i]]->isInside(ray)){
            _curFound = i;
            _status = ANIMATION;
            return true;
        }
    }
    return false;
}

void ModelFlags::remove()
{
    _flags[HIDDEN_FLAGS[_curFound]]->found();
}

int ModelFlags::getFound()
{
    return _curFound + FLAG1;
}