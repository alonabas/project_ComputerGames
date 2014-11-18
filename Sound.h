//
//  Sound.h
//  project
//
//  Load and play sound
//  Created by Alona Bas on 4/29/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef __ex3__Sound__
#define __ex3__Sound__

#include "Defs.h"

class Sound {
	void* _data;
	unsigned int _bufferID;
	unsigned int _sourceID;
	int _size;
	int _format;
	int _sampleRate;
public:
	Sound(int format, int sampleRate, void* data, int size);
	virtual ~Sound();
	static Sound* readWave(const char* filename);
	void play();
	void stop();
};

#endif /* defined(__project__Sound__) */
