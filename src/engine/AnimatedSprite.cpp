#include "AnimatedSprite.h"
#include "engine/TextureManager.h"
#include <iostream>

AnimatedSprite::AnimatedSprite(const std::string & filepathToTexture)
{
	texture = TextureManager::getTexture(filepathToTexture);
}

AnimatedSprite::AnimatedSprite(Texture * texture)
{
	this->texture = texture;
}

void AnimatedSprite::addAnimation(const std::string & animationName)
{
	animations[animationName] = std::vector<std::array<float, 5>>();
}

void AnimatedSprite::bindAnimation(const std::string & animationName)
{
	currentAnimation = animationName;
}



void AnimatedSprite::setFrames(int rowCount, int colCount, int frameCount, float frameDir, float frameX, float frameY)
{
	int frameVal = 0;
	for (int c = 0; c < colCount; c++) {
		for (int r = 0; r < rowCount; r++) {

			if (frameVal < frameCount) {
				std::cout << frameX * r << " " << frameY * c << " " << frameX << " " << frameY << std::endl;
				addFrame(frameDir, frameX * r, frameY * c, frameX, frameY);
				frameVal++;
			}
			else {
				std::cout << "no frame added" << std::endl;
			}
			
		}
	}




}

void AnimatedSprite::addFrame(float durration, int sampleX, int sampleY, int sampleWidth, int sampleHeight)
{
	animations[currentAnimation].push_back({ (float)durration, (float)sampleX, (float)sampleY, (float)sampleWidth, (float)sampleHeight });
}

int AnimatedSprite::getFrameCount()
{
	return animations[currentAnimation].size();
}

void AnimatedSprite::setScalesuniformly(bool value)
{

}

void AnimatedSprite::setX(float x)
{
	this->x = x;
}

void AnimatedSprite::setY(float y)
{
	this->y = y;
}

void AnimatedSprite::setWidth(float width)
{
	scalesByWidth = true;
	this->width;
}

void AnimatedSprite::setHeight(float height)
{
	scalesByWidth = false;
	this->height = height;
}

void AnimatedSprite::setScaleOrigin(int originPosition)
{
	scaleOrigin = originPosition;
}

float AnimatedSprite::getAnimationLength()
{
	float time = 0.0f;

	for (int i = 0; i < animations[currentAnimation].size(); i++) {
		time += animations[currentAnimation][i][0];
	}

	return time;
}

void AnimatedSprite::setFlipsHorozontal(bool value)
{
	flipHorozontal = value;
}

void AnimatedSprite::setFlipsVertical(bool value)
{
	flipVertical = value;
}

float AnimatedSprite::getX()
{
	return x;
}

float AnimatedSprite::getY()
{
	return y;
}

float AnimatedSprite::getWidth()
{
	return width;
}

float AnimatedSprite::getHeight()
{
	return height;
}

Texture* AnimatedSprite::getTexture()
{
	return texture;
}

void AnimatedSprite::setTexture(Texture& texture)
{
	this->texture = &texture;
}

std::array<float, 4> AnimatedSprite::getSampleBoundsAtTime(float timeInMilliseconds)
{
	timeInMilliseconds = fmod(timeInMilliseconds, getAnimationLength());

	float timeLeft = timeInMilliseconds;
	int i;
	for (i = 0; i < animations[currentAnimation].size(); i++) {
		timeLeft -= animations[currentAnimation][i][0];
		if (timeLeft <= 0) {
			break;
		}
	}

	std::array<float, 5>& currentFrame = animations[currentAnimation][i];
	float currentFrameX = currentFrame[1];
	float currentFrameY = currentFrame[2];
	float currentFrameWidth = currentFrame[3];
	float currentFrameHeight = currentFrame[4];

	float renderWidth, renderHeight;
	if (scalesUniformly) {
		if (scalesByWidth) {
			renderWidth = width;
			renderHeight = (currentFrameHeight / currentFrameWidth) * width;
		}
		else {
			renderHeight = height;
			renderWidth = (currentFrameWidth / currentFrameHeight) * height;
		}
	}
	else {
		renderWidth = width;
		renderHeight = height;
	}

	float renderX, renderY;

	// 0 = topLeft,  1 = topRight,   2 = bottomLeft,   3 = bottomRight,   4 = Middle
	switch (scaleOrigin) {
	case 0:
		renderX = x;
		renderY = y - renderHeight;
		break;
	case 1:
		renderX = x - renderWidth;
		renderY = y - renderHeight;
		break;
	case 2:
		renderX = x;
		renderY = y;
		break;
	case 3:
		renderX = x - renderWidth;
		renderY = y;
		break;
	case 4:
		renderX = x - renderWidth / 2;
		renderY = y - renderHeight / 2;
		break;
	default:
		__debugbreak(); // this is a problem
		break;
	}


	int sampleX = (int)currentFrame[1], sampleY = (int)currentFrame[2], sampleWidth = (int)currentFrame[3], sampleHeight = (int)currentFrame[4];
	if (flipHorozontal) {
		sampleX += sampleWidth;
		sampleWidth *= -1;
	}
	if (flipVertical) {
		sampleY += sampleHeight;
		sampleHeight *= -1;
	}

	return { (float)sampleX, (float)sampleY, (float)sampleWidth, (float)sampleHeight };
}



void AnimatedSprite::render(float timeInMilliseconds)
{
	timeInMilliseconds = fmod(timeInMilliseconds, getAnimationLength());

	float timeLeft = timeInMilliseconds;
	int i;
	for (i = 0; i < animations[currentAnimation].size(); i++) {
		timeLeft -= animations[currentAnimation][i][0];
		if (timeLeft <= 0) {
			break;
		}
	}

	std::array<float, 5>& currentFrame = animations[currentAnimation][i];
	float currentFrameX = currentFrame[1];
	float currentFrameY = currentFrame[2];
	float currentFrameWidth = currentFrame[3];
	float currentFrameHeight = currentFrame[4];

	float renderWidth, renderHeight;
	if (scalesUniformly) {
		if (scalesByWidth) {
			renderWidth = width;
			renderHeight = (currentFrameHeight / currentFrameWidth) * width;
		}
		else {
			renderHeight = height;
			renderWidth = (currentFrameWidth / currentFrameHeight) * height;
		}
	}
	else {
		renderWidth = width;
		renderHeight = height;
	}

	float renderX, renderY;

	// 0 = topLeft,  1 = topRight,   2 = bottomLeft,   3 = bottomRight,   4 = Middle
	switch (scaleOrigin) {
	case 0:
		renderX = x;
		renderY = y - renderHeight;
		break;
	case 1:
		renderX = x - renderWidth;
		renderY = y - renderHeight;
		break;
	case 2:
		renderX = x;
		renderY = y;
		break;
	case 3:
		renderX = x - renderWidth;
		renderY = y;
		break;
	case 4:
		renderX = x - renderWidth / 2;
		renderY = y - renderHeight / 2;
		break;
	default:
		__debugbreak(); // this is a problem
		break;
	}

	static TexturedQuad quad;
	quad.setX(renderX);
	quad.setY(renderY);
	quad.setWidth(renderWidth);
	quad.setHeight(renderHeight);

	int sampleX = (int)currentFrame[1], sampleY = (int)currentFrame[2], sampleWidth = (int)currentFrame[3], sampleHeight = (int)currentFrame[4];
	if (flipHorozontal) {
		sampleX += sampleWidth;
		sampleWidth *= -1;
	}
	if (flipVertical) {
		sampleY += sampleHeight;
		sampleHeight *= -1;
	}

	quad.setTexture(*texture);
	quad.setTextureSampleArea(sampleX, sampleY, sampleWidth, sampleHeight);

	quad.render();
}
