//
//  Sound.cpp
//  project
//
//  Load and play sound
//  Created by Alona Bas on 4/29/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "Sound.h"

#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096

Sound::Sound(int format, int sampleRate, void* data, int size) :
		_data(data), _size(size), _format(format), _sampleRate(sampleRate) {
	alGenBuffers(1, &_bufferID);
	alBufferData(_bufferID, _format, _data, _size, _sampleRate);
	alGenSources(1, &_sourceID);
	alSourcei(_sourceID, AL_BUFFER, _bufferID);

}
Sound::~Sound() {
	if (_bufferID != 0)
		alDeleteBuffers(1, &_bufferID);
	if (_sourceID != 0)
		alDeleteSources(1, &_sourceID);
}
void Sound::play() {
	int state;
	alGetSourcei(_sourceID, AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING) {
		alSourcePlay(_sourceID);
	}
}

void Sound::stop() {
	int state;
	alGetSourcei(_sourceID, AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING) {
		alSourceStop(_sourceID);
	}
}

Sound* Sound::readWave(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) {
//        fclose(fp);
		std::cout << filename << ": Failed to open file" << std::endl;
//        return NULL;
	}
	char* chunkID = new char[4];
	fread(chunkID, 4, sizeof(char), fp);
	if (strcmp(chunkID, "RIFF")) {
//        delete [] chunkID;
//        fclose(fp);
		std::cout << filename << ": Error in format: not RIFF, " << chunkID
				<< std::endl;
//        return NULL;
	}

	fseek(fp, 8, SEEK_SET);

	char* format = new char[4];
	fread(format, 4, sizeof(char), fp);
	if (strcmp(format, "WAVE")) {
//        delete [] chunkID;
//        delete [] format;
//        fclose(fp);
		std::cout << filename << ": Illegal format: not WAVE, " << format
				<< std::endl;
//        return NULL;
	}

	char* subChunk1Id = new char[4];
	fread(subChunk1Id, 4, sizeof(char), fp);
	if (strcmp(subChunk1Id, "fmt ")) {
//        delete [] chunkID;
//        delete [] format;
//        delete [] subChunk1Id;
//        fclose(fp);
		std::cout << filename << ": Error in format: not fmt , " << subChunk1Id
				<< std::endl;
//        return NULL;
	}

	unsigned int subChunk1Size;
	fread(&subChunk1Size, 1, sizeof(unsigned int), fp);

	unsigned int subChunk2Position = (unsigned int) ftell(fp) + subChunk1Size;

	unsigned short audioFormat;
	fread(&audioFormat, 1, sizeof(unsigned short), fp);
	if (audioFormat != 1) {
//        delete [] chunkID;
//        delete [] format;
//        delete [] subChunk1Id;
//        fclose(fp);
		std::cout << filename << ": Error in format: not PCM , " << audioFormat
				<< std::endl;
//        return NULL;
	}

	unsigned short numOfChannels;
	fread(&numOfChannels, 1, sizeof(unsigned short), fp);

	unsigned int sampleRate;
	fread(&sampleRate, 1, sizeof(unsigned int), fp);

	fseek(fp, 34, SEEK_SET);

	unsigned short bitsPerSample;
	fread(&bitsPerSample, 1, sizeof(unsigned short), fp);

	int alFormat;
	if (numOfChannels == 1 && bitsPerSample == 8)
		alFormat = AL_FORMAT_MONO8;
	else if (numOfChannels == 1 && bitsPerSample == 16)
		alFormat = AL_FORMAT_MONO16;
	else if (numOfChannels == 2 && bitsPerSample == 8)
		alFormat = AL_FORMAT_STEREO8;
	else if (numOfChannels == 2 && bitsPerSample == 16)
		alFormat = AL_FORMAT_STEREO16;
	else {
		alFormat = AL_FORMAT_MONO8;
//        delete [] chunkID;
//        delete [] format;
//        delete [] subChunk1Id;
//        fclose(fp);
		std::cout << filename << ": Unknown format" << std::endl;
//        return NULL;
	}

	fseek(fp, subChunk2Position, SEEK_SET);

	char* subChunk2Id = new char[4];
	fread(subChunk2Id, 4, sizeof(char), fp);
	if (strcmp(subChunk2Id, "data")) {
//        delete [] chunkID;
//        delete [] format;
//        delete [] subChunk1Id;
//        delete [] subChunk2Id;
//        fclose(fp);
		std::cout << filename << ": Error in format" << subChunk2Id
				<< std::endl;
//        return NULL;
	}

	unsigned int subChunk2Size;
	fread(&subChunk2Size, 1, sizeof(unsigned int), fp);

	unsigned char* data = new unsigned char[subChunk2Size];
	fread(data, subChunk2Size, sizeof(unsigned char), fp);

	delete[] chunkID;
	delete[] format;
	delete[] subChunk1Id;
	delete[] subChunk2Id;
	fclose(fp);

	return new Sound(alFormat, sampleRate, data, subChunk2Size);
}

