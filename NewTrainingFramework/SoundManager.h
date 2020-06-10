#pragma once
#include "SoundResources.h"
#include "Fmod/fmod_studio.hpp"
#include "Fmod/fmod.hpp"


class SoundManager
{
public:
	SoundResources* sr;
	GLuint soundId;
	FMOD::Sound* sound;

	SoundManager();
	~SoundManager();
};

