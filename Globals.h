//
//  Globals.h
//  project
//
//  Created by Alona Bas on 9/22/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef project_Globals_h
#define project_Globals_h

#define STATUE 0
#define BEETLE 1
#define FLAG1 2
#define FLAG2 3
#define FLAG3 4
#define FLAG4 5
#define MIRROR 6
#include "Defs.h"

static const vec3 EYE_POSITION(0.0f,0.0f,0.0f);
static const vec3 LIGHT1_POSITION(100.0f,100.0f,100.0f);
static const vec3 LIGHT2_POSITION(-100.0f,100.0f,-100.0f);

static const vec3 LIGHT1_COLOR(1.0, 0.9, 0.7);
static const vec3 LIGHT2_COLOR(0.7, 1.0, 0.9);
static const vec3 LIGHT3_COLOR(1.0,0.5,0.5);

static const vec3 GRAVITY(0.0, -0.015, 0.0005);

static const vec3 WIND_POSITION1(2.0f, 0.0f, 10.0f);
static const vec3 WIND_POSITION2(2.0f, 0.0f, -10.0f);

#endif
