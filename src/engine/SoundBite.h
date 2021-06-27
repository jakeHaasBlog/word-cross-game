#pragma once

#include <string>
#include "yse.hpp"

#include <array>
#include <list>
#include <map>

#define MAX_SIMULTANIOUS_SOUNDS 30

class SoundBite {
public:
	// volume defaults to 0.3 and is paused
	SoundBite(const std::string& filepath);
	~SoundBite();

	// between 0 and 1
	void setVolume(float volume);

	// 0.5 = half speed, 5 = 5 times speed
	void setPlaybackSpeed(float speed);

	// 2D sounds are not affected by position
	void set2D(bool is2D);

	// sets relitive position of the sound with respect to your monitor facing position
	void setPosition(float x, float y, float z);

	// sets the relitive position of the sound with respect to an observer's position and facing direction
	void setPosition(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos);

	// plays the whole given audio file
	void playAll();

	// parts must be added via the addPart method
	void playPart(int i);

	// parts must be added via the addPart method
	void playPart(const std::string& partName);

	// parts must be added via the addPart method
	void playRandomPart();

	// will stop playing at the end of the interval
	void playInterval(int startInMilliseconds, int endInMilliseconds);

	// parts are stored in an array and can be played using playPart
	void addPart(int startInMilliseconds, int endInMilliseconds, const std::string& partName = "");

	// returns the length of the sound in milliseconds
	float getTotalLength();

	const std::string& getFilepath();

public:
	// this function is automatically called once per frame, DONT CALL IT AGAIN!!
	static void update();

private:
	bool isInitialized = false;
	const std::string filepath;

	float volume = 1.0f;
	float playbackSpeed = 1.0f;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	bool is3D = false;

	const static int samplerate;
	float fileLength;

	void makeSound(float begin, float end);

	std::vector<std::array<float, 2>> partIntervals;
	std::map<std::string, int> partNames;

	static int currentSoundIndex;
	static std::array<YSE::sound*, MAX_SIMULTANIOUS_SOUNDS> sounds;
	static std::array<std::pair<bool, float>, MAX_SIMULTANIOUS_SOUNDS> soundInfo;

};