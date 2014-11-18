//
//  Defs.h
//  project
//
//  General defenitions and includes
//
//  Created by Alona Bas on 3/3/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef project_Defs_h
#define project_Defs_h

// defs
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#else
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <AL/al.h>
#include <AL/alc.h>
#endif

// basic
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <map>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <ctime>

// GLM and OpenMesh headers
#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
#include <OpenMesh/Core/IO/reader/OBJReader.hh>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/random.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

using namespace glm;
using namespace std;

#define	WINDOW_SIZE         (800) // initial size of the window               //
#define	WINDOW_POS_X        (100) // initial X position of the window         //
#define	WINDOW_POS_Y        (100) // initial Y position of the window         //
#define	WINDOW_SIZE_LIST   (300)

#define PERSPECTIVE_ANGLE 90.0f
#define PERSPECTIVE_DISTANCE 80.0f
#define PERSPECTIVE_START 0.1f

#define SHADERS_DIR "shaders/"

#define TIME_UNIT 0.08f
#define FPS 120
#define DEGREE_IN_PI 180.0f

#define ROTATION_STEP 0.02f

#define NUMBER_SKYBOX_TEXTURES 6

static const double EPS = 1e-6;   // Epsilon (value closest to zero) //
static const glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);
static const glm::vec3 ASIDE_VECTOR(1.0f, 0.0f, 0.0f);
static const glm::vec3 DIR_VECTOR(0.0f, 0.0f, -1.0f);

struct face_indices_t {
	GLuint a, b, c;
};

typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

typedef std::map<std::string, GLuint> ProgramMap;

#endif
