//
//  Instructions.h
//  project
//
//  Class that defines the messages at the left side of the screen
//
//  Created by Alona Bas on 9/22/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Instructions.h"
Instructions::Instructions():_mode(MAIN_MODE), _width(WINDOW_SIZE_LIST), _high(WINDOW_SIZE)
{
    _text[0] = new Text("Fonts/VideophreakFont.tga");
    _text[1] = new Text("Fonts/VideophreakFontRed.tga");
    _text[2] = new Text("Fonts/VideophreakFontGray.tga");
    _text[3] = new Text("Fonts/VideophreakFontStroke.tga");
    _text[4] = new Text("Fonts/VideophreakFontStrokeGray.tga");
    _text[5] = new Text("Fonts/NBP.tga");
    _sky = new Sky();
}

Instructions::~Instructions()
{
    if(_text[0] != NULL){
        delete _text[0];
        _text[0] = NULL;
    }
    if(_text[1] != NULL){
        delete _text[1];
        _text[1] = NULL;
    }
    if(_text[2] != NULL){
        delete _text[2];
        _text[2] = NULL;
    }
    if(_text[3] != NULL){
        delete _text[3];
        _text[3] = NULL;
    }
    if(_text[4] != NULL){
        delete _text[4];
        _text[4] = NULL;
    }
    if(_text[5] != NULL){
        delete _text[5];
        _text[5] = NULL;
    }
    if(_sky != NULL){
        delete _sky;
        _sky = NULL;
    }
}

void Instructions::switchMode()
{
    _mode = (_mode + 1)%2;
}
void Instructions::init()
{
    _text[0]->init();
    _text[0]->resize(_width, _high);
	_text[1]->init();
    _text[1]->resize(_width, _high);
    _text[2]->init();
    _text[2]->resize(_width, _high);
    _text[3]->init();
    _text[3]->resize(_width, _high);
    _text[4]->init();
    _text[4]->resize(_width, _high);
    _text[5]->init();
    _text[5]->resize(_width, _high);

    _sky->init();
}
void Instructions::draw()
{
	_sky->draw();
    if (_mode == MAIN_MODE || isEndFlags()){
        _text[1]->draw(INSTRUCTION_MAIN1_1, _high - 30.0f, 30.0f);
        _text[1]->draw(INSTRUCTION_MAIN1_2, _high - 60.0f, 30.0f);
        _text[1]->draw(INSTRUCTION_MAIN1_3, _high - 90.0f, 30.0f);
        _text[0]->draw(INSTRUCTION_MAIN2, _high - 180.0f, 20.0f);
        
        
        if(_objects[0] == 1){
            _text[3]->draw(INSTRUCTION_MAIN_OBJECT0, _high - 210.0f, 20.0f);
        }
        else{
            _text[0]->draw(INSTRUCTION_MAIN_OBJECT0, _high - 210.0f, 20.0f);
        }
        
        if(_objects[1] == 1){
            _text[3]->draw(INSTRUCTION_MAIN_OBJECT1, _high - 240.0f, 20.0f);
        }
        else{
            _text[0]->draw(INSTRUCTION_MAIN_OBJECT1, _high - 240.0f, 20.0f);
        }
        
        if(_objects[2] == 1){
            _text[4]->draw(INSTRUCTION_MAIN_OBJECT2, _high - 280.0f, 20.0f);
        }
        else{
            _text[2]->draw(INSTRUCTION_MAIN_OBJECT2, _high - 280.0f, 20.0f);
        }
        
        if(_objects[3] == 1){
            _text[4]->draw(INSTRUCTION_MAIN_OBJECT3, _high - 310.0f, 20.0f);
        }
        else{
            _text[2]->draw(INSTRUCTION_MAIN_OBJECT3, _high - 310.0f, 20.0f);
        }

        if(_objects[4] == 1){
            _text[4]->draw(INSTRUCTION_MAIN_OBJECT4, _high - 340.0f, 20.0f);
        }
        else{
            _text[2]->draw(INSTRUCTION_MAIN_OBJECT4, _high - 340.0f, 20.0f);
        }

        
        if(_objects[5] == 1){
            _text[4]->draw(INSTRUCTION_MAIN_OBJECT5, _high - 370.0f, 20.0f);
        }
        else{
            _text[2]->draw(INSTRUCTION_MAIN_OBJECT5, _high - 370.0f, 20.0f);
        }

        
        if(_objects[6] == 1){
            _text[3]->draw(INSTRUCTION_MAIN_OBJECT6, _high - 410.0f, 20.0f);
        }
        else{
            _text[0]->draw(INSTRUCTION_MAIN_OBJECT6, _high - 410.0f, 20.0f);
        }

    }
    else{
        _text[1]->draw(INSTRUCTION_LIST1_1, _high - 30.0f, 30.0f);
        _text[1]->draw(INSTRUCTION_LIST1_2, _high - 60.0f, 30.0f);
        _text[1]->draw(INSTRUCTION_LIST1_3, _high - 90.0f, 30.0f);
        _text[1]->draw(INSTRUCTION_LIST1_4, _high - 130.0f, 30.0f);
        _text[1]->draw(INSTRUCTION_LIST1_5, _high - 160.0f, 30.0f);

        _text[0]->draw(INSTRUCTION_LIST2, _high - 250.0f, 20.0f);
        

        if(_objects[0] == 1){
            _text[4]->draw(INSTRUCTION_MAIN_OBJECT0, _high - 280.0f, 20.0f);
        }
        else{
            _text[2]->draw(INSTRUCTION_MAIN_OBJECT0, _high - 280.0f, 20.0f);
        }
        
        if(_objects[1] == 1){
            _text[4]->draw(INSTRUCTION_MAIN_OBJECT1, _high - 310.0f, 20.0f);
        }
        else{
            _text[2]->draw(INSTRUCTION_MAIN_OBJECT1, _high - 310.0f, 20.0f);
        }
        
        if(_objects[2] == 1){
            _text[3]->draw(INSTRUCTION_MAIN_OBJECT2, _high - 340.0f, 20.0f);
        }
        else{
            _text[0]->draw(INSTRUCTION_MAIN_OBJECT2, _high - 340.0f, 20.0f);
        }
        
        if(_objects[3] == 1){
            _text[3]->draw(INSTRUCTION_MAIN_OBJECT3, _high - 370.0f, 20.0f);
        }
        else{
            _text[0]->draw(INSTRUCTION_MAIN_OBJECT3, _high - 370.0f, 20.0f);
        }
        
        if(_objects[4] == 1){
            _text[3]->draw(INSTRUCTION_MAIN_OBJECT4, _high - 400.0f, 20.0f);
        }
        else{
            _text[0]->draw(INSTRUCTION_MAIN_OBJECT4, _high - 400.0f, 20.0f);
        }
        
        
        if(_objects[5] == 1){
            _text[3]->draw(INSTRUCTION_MAIN_OBJECT5, _high - 430.0f, 20.0f);
        }
        else{
            _text[0]->draw(INSTRUCTION_MAIN_OBJECT5, _high - 430.0f, 20.0f);
        }
        
        
        if(_objects[6] == 1){
            _text[4]->draw(INSTRUCTION_MAIN_OBJECT6, _high - 460.0f, 20.0f);
        }
        else{
            _text[2]->draw(INSTRUCTION_MAIN_OBJECT6, _high - 460.0f, 20.0f);
        }

    }
    _text[5]->draw(INSTRUCTION_NOTE1, _high - 600.0f, 18.0f);
    _text[5]->draw(INSTRUCTION_NOTE2, _high - 620.0f, 18.0f);
    _text[5]->draw(INSTRUCTION_NOTE3, _high - 640.0f, 18.0f);
    _text[5]->draw(INSTRUCTION_NOTE4, _high - 660.0f, 18.0f);
}
void Instructions::setFound(int objID)
{
    _objects[objID] = FOUND;
}
bool Instructions::isEndFlags()
{
    if ((_objects.to_ulong() & 60) == 60){
        return true;
    }
    return false;
}
bool Instructions::isWin()
{
    if((_objects.to_ulong() & 127) == 127){
        return true;
    }
    else{
        return false;
    }
}
