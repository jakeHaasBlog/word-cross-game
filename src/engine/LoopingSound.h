#pragma once

#include "yse.hpp"
#include <string>
#include <array>

class LoopingSound {
public:
	// volume defaults to 0.3 and is paused
	LoopingSound(const std::string filepath);

	// clamped between 0 and 1
	void setVolume(float volume);

	// 0.5 = half original speed, 5 = 5 times original speed
	void setPlaybackSpeed(float speed);

	// flat sound, not affected by position
	void play2D();

	// 3d audio affected by position, should call setPosition() every frame if the observer or sound is moving
	void play3D();

	// sets the 3D position of the sound when the observer is at 0, 0, 0, facing towards 0, 0, 1
	void setPosition(float x, float y, float z);
	
	// calculates and sets the desired position of the sound based on the sound position relitive to a given observer position and direction
	void setPosition(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos);

	void pause();
	void resume();

	bool isPlaying();

	// time should be between 0 and 1, where 0 is the beginning and 1 the end
	void setTime(float time);

	// returns between 0 and 1, where 0 is the beginning and 1 the end
	float getCurrentTime();

	// returns the length of the loaded sound in seconds
	float getSoundLength();

	// between 0 and 1
	float getVolume();

	// 0.5 = half original speed, 5 = 5 times original speed
	float getPlaybackSpeed();

	const std::string& getFilepath();

private:
	const std::string filepath;
	bool isInitialized = false;
	float x, y, z;

	YSE::sound sound;

	const static int samplerate;

	void tryInitialize();

};

