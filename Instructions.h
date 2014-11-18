//
//  Instructions.h
//  project
//
//  Class that defines the messages at the left side of the screen
//
//  Created by Alona Bas on 9/22/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __project__Instructions__
#define __project__Instructions__

#include <iostream>
#include <bitset>
#include "Text.h"
#include "Globals.h"
#include "Sky.h"

#define MAIN_MODE 0
#define SUB_MODE 1

#define FOUND 1
#define NOT_FOUND 0
#define NUMBER_HIDDEN_OBJECTS 6

class Instructions{
	const char* INSTRUCTION_MAIN1_1 = "Use WASD";
	const char* INSTRUCTION_MAIN1_2 = "to rotate";
	const char* INSTRUCTION_MAIN1_3 = "the camera.";
	const char* INSTRUCTION_MAIN2 = "Find objects:";
	const char* INSTRUCTION_LIST1_1 = "Use SD";
	const char* INSTRUCTION_LIST1_2 = "to move";
	const char* INSTRUCTION_LIST1_3 = "the camera.";
	const char* INSTRUCTION_LIST1_4 = "Press B";
	const char* INSTRUCTION_LIST1_5 = "to return";
	const char* INSTRUCTION_LIST2 = "Find objects:";

	const char* INSTRUCTION_MAIN_OBJECT0 = "Gold Statue";
	const char* INSTRUCTION_MAIN_OBJECT1 = "Glass Beetle";
	const char* INSTRUCTION_MAIN_OBJECT2 = "RSA Flag";
	const char* INSTRUCTION_MAIN_OBJECT3 = "Malta Flag";
	const char* INSTRUCTION_MAIN_OBJECT4 = "Venezuella Flag";
	const char* INSTRUCTION_MAIN_OBJECT5 = "Cambodia Flag";
	const char* INSTRUCTION_MAIN_OBJECT6 = "Mirror";
    const char* INSTRUCTION_NOTE1 = "To continue in";
    const char* INSTRUCTION_NOTE2 = "lite mode press m";
    const char* INSTRUCTION_NOTE3 = "or choose lite mode";
    const char* INSTRUCTION_NOTE4 = "from the menu";

	int _mode;
	int _width, _high;
	Text* _text[6];
	Sky* _sky;
	std::bitset<NUMBER_HIDDEN_OBJECTS> _objects;
public:
	Instructions();
	virtual ~Instructions();
	void init();
	void switchMode();
	void draw();
	void setFound(int objID);
	void resize(int width, int high){_width = width; _high = high;}
	bool isEndFlags();
	bool isWin();


};
#endif /* defined(__project__Instructions__) */
