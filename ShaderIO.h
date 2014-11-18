//
//  ShaderIO.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ShaderIO__
#define __ShaderIO__

#include "Defs.h"

class programManager {
	ProgramMap _programs;

public:
	static programManager& sharedInstance();

public:
	GLuint createProgram(std::string program_id,
			const char* vertex_shader_filename,
			const char* geometry_shader_filename,
			const char* fragment_shader_filename);
	GLuint createProgram(std::string program_id,
			const char* vertex_shader_filename,
			const char* fragment_shader_filename);
public:
	GLuint programWithID(std::string program_id) const;
};

#endif /* defined(__ShaderIO__) */
