#pragma once

#include "TexturedQuad.h"
#include "Texture.h"
#include <vector>
#include <array>
#include <map>

/* There should only be one instance of every animated sprite, just render that one instance multiple times and alter its position and scale */
class AnimatedSprite {
public:
	
	AnimatedSprite(const std::string& filepathToTexture);
	AnimatedSprite(Texture* texture);

	void addAnimation(const std::string& animationName);
	void bindAnimation(const std::string& animationName);
	
	// this should only be used for initializaton
	void setFrames(int rowCount, int colCount, int frameCount, float frameDir, float frameX, float frameY); // add frames in bulk
	void addFrame(float durration, int sampleX, int sampleY, int sampleWidth, int sampleHeight);
	int getFrameCount();

	// if this is set true then setWidth and setHeight will not stretch the frames awkwardly
	void setScalesuniformly(bool value);

	// sets the x of the bottom left corner of every frame
	void setX(float x);

	// sets the y of the bottom left corner of every frame
	void setY(float y);

	// this will be the final height of every frame, if this sprite scales uniformly then it will affect the width of every frame differently
	void setWidth(float width);

	// this will be the final height of every frame, if this sprite scales uniformly then it will affect the height of every frame differently
	void setHeight(float height);

	// origin position can be: 0 = topLeft,  1 = topRight,   2 = bottomLeft,   3 = bottomRight,   4 = Middle
	void setScaleOrigin(int originPosition);

	// returns length of bound animation in milliseconds
	float getAnimationLength();

	// so if you have a running right animation, it will now appear to run left
	void setFlipsHorozontal(bool value);

	// so if you have a crawling up animation, it will now appear to crawl down
	void setFlipsVertical(bool value);

	float getX();
	float getY();
	float getWidth();
	float getHeight();

	Texture* getTexture();
	void setTexture(Texture& texture);

	std::array<float, 4> getSampleBoundsAtTime(float timeInMilliseconds);

	void render(float timeInMilliseconds);

private:
	Texture* texture;
	std::string currentAnimation;

	bool flipHorozontal = false;
	bool flipVertical = false;

	// map to animations. Animations are vectors of frames. Frames are arrays of floats { showingTime, sampleX, sampleY, sampleWidth, sampleHeight }
	std::map<std::string, std::vector<std::array<float, 5>>> animations;

	float x = 0.0f, y = 0.0f, width = 1.0f, height = 1.0f;
	bool scalesUniformly = true, scalesByWidth = false;
	int scaleOrigin = 2;

};