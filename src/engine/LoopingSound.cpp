#include "engine/LoopingSound.h"

const int LoopingSound::samplerate = 52080;

LoopingSound::LoopingSound(const std::string filepath) :
	filepath(filepath)
{
}

void LoopingSound::setVolume(float volume)
{
	tryInitialize();
	sound.setVolume(volume);
}

void LoopingSound::setPlaybackSpeed(float speed)
{
	tryInitialize();
	sound.setSpeed(speed);
}

void LoopingSound::play2D()
{
	tryInitialize();

	sound.set2D(true);
	sound.setTime(0);
	sound.play();
}

void LoopingSound::setPosition(float x, float y, float z)
{
	tryInitialize();

	this->x = x;
	this->y = y;
	this->z = z;

	sound.setPosition({ x, y, z });
}

void LoopingSound::setPosition(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos)
{
	tryInitialize();

	observerFacingDirection += 3.14159f / 2.0f;

	float observerFacingDirectionX = cos(observerFacingDirection);
	float observerFacingDirectionY = sin(observerFacingDirection);

	std::array<float, 2> vecToSound = {
		soundPos[0] - observerPos[0],
		soundPos[1] - observerPos[1],
	};

	float dotProduct = vecToSound[0] * observerFacingDirectionX + vecToSound[1] * observerFacingDirectionY;
	float magA = sqrt(pow(vecToSound[0], 2) + pow(vecToSound[1], 2));
	float magB = sqrt(pow(observerFacingDirectionX, 2) + pow(observerFacingDirectionY, 2));
	float angle = acos(dotProduct / (magA * magB));


	x = -cos(angle) * magA;
	y = 0.0f;
	z = -sin(angle) * magA;

	sound.setPosition({ x, y, z });
}

void LoopingSound::play3D()
{
	tryInitialize();

	sound.set2D(false);
	sound.setPosition({ x, y, z });
	sound.setTime(0);
	sound.play();
}

void LoopingSound::pause()
{
	tryInitialize();
	sound.pause();
}

void LoopingSound::resume()
{
	tryInitialize();
	sound.play();
}

void LoopingSound::setTime(float time)
{
	tryInitialize();
	sound.setTime(time * getSoundLength());
}

bool LoopingSound::isPlaying()
{
	tryInitialize();
	return sound.isPlaying();
}

float LoopingSound::getCurrentTime()
{
	tryInitialize();
	return sound.getTime() / sound.getLength();
}

float LoopingSound::getSoundLength()
{
	tryInitialize();

	return ((float)sound.getLength() / samplerate) * 1000.0f;
}

float LoopingSound::getVolume()
{
	tryInitialize();
	return sound.getVolume();
}

float LoopingSound::getPlaybackSpeed()
{
	tryInitialize();
	return sound.getSpeed();
}

const std::string & LoopingSound::getFilepath()
{
	return filepath;
}

void LoopingSound::tryInitialize()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}
}
