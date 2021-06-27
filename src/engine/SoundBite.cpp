#include "engine/SoundBite.h"

int SoundBite::currentSoundIndex = 0;
std::array<YSE::sound*, MAX_SIMULTANIOUS_SOUNDS> SoundBite::sounds = std::array<YSE::sound*, MAX_SIMULTANIOUS_SOUNDS>();
std::array<std::pair<bool, float>, MAX_SIMULTANIOUS_SOUNDS> SoundBite::soundInfo;

const int SoundBite::samplerate = 52080;

SoundBite::SoundBite(const std::string & filepath) :
	filepath(filepath)
{
}

SoundBite::~SoundBite()
{
}

void SoundBite::setVolume(float volume)
{
	this->volume = volume;
}

void SoundBite::setPlaybackSpeed(float speed)
{
	this->playbackSpeed = speed;
}

void SoundBite::set2D(bool is2D)
{
	is3D = !is2D;
}

void SoundBite::setPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void SoundBite::setPosition(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos)
{
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
}

void SoundBite::playAll()
{
	makeSound(0, getTotalLength());
}

void SoundBite::playPart(int i)
{
	makeSound(partIntervals[i][0], partIntervals[i][1]);
}

void SoundBite::playPart(const std::string & partName) {
	playPart(partNames[partName]);
}

void SoundBite::playRandomPart()
{
	int r = rand();
	makeSound(partIntervals[r % partIntervals.size()][0], partIntervals[r % partIntervals.size()][1]);
}

void SoundBite::playInterval(int startInMilliseconds, int endInMilliseconds)
{
	makeSound(startInMilliseconds, endInMilliseconds);
}

void SoundBite::addPart(int startInMilliseconds, int endInMilliseconds, const std::string & partName)
{
	if (partName.length() > 0)
		partNames[partName] = partIntervals.size();

	partIntervals.push_back({ (float)startInMilliseconds, (float)endInMilliseconds });
}

float SoundBite::getTotalLength()
{
	if (fileLength == 0) {
		YSE::sound tmpSound;
		tmpSound.create(filepath.c_str());

		// yse initializes on seperate thread so have to wait for it
		while (fileLength == 0) {
			fileLength = (float)tmpSound.getLength() / (float)samplerate;
			YSE::system().update();
		} 

	}

	return fileLength * 1000.0f;
}

const std::string & SoundBite::getFilepath()
{
	return filepath;
}

void SoundBite::update()
{

	for (int i = 0; i < sounds.size(); i++) {

		if (sounds[i]) {
			if (sounds[i]->isPlaying()) {
				soundInfo[i].first = true;
			}

			if (soundInfo[i].first && (!sounds[i]->isPlaying() || sounds[i]->getTime() >= soundInfo[i].second)) {
				sounds[i]->stop();
			}
		}

	}

}

void SoundBite::makeSound(float begin, float end)
{
	begin /= 1000.0f;
	end /= 1000.0f;
	begin *= (float)samplerate;
	end *= (float)samplerate;

	if (sounds[currentSoundIndex]) {
		delete sounds[currentSoundIndex];
	}
	sounds[currentSoundIndex] = new YSE::sound();
	sounds[currentSoundIndex]->create(filepath.c_str());
	soundInfo[currentSoundIndex] = { false, end };

	sounds[currentSoundIndex]->setTime(begin);

	if (is3D) {
		sounds[currentSoundIndex]->set2D(false);
		sounds[currentSoundIndex]->setPosition({ x, y, z });
	}
	else {
		sounds[currentSoundIndex]->set2D(true);
	}

	sounds[currentSoundIndex]->setVolume(volume);
	sounds[currentSoundIndex]->setSpeed(playbackSpeed);

	sounds[currentSoundIndex]->play();

	currentSoundIndex++;
	if (currentSoundIndex >= sounds.size()) {
		currentSoundIndex = 0;
	}
}
