#pragma once

#include "engine/Texture.h"
#include "engine/AnimatedSprite.h"
#include <string>

/* A prop is a front-facing texture that represents any visible non-wall object
	- They can be an image, solid color, or animation
	- They have an x, y, and z coordinate
	- They have a width and height
	- Must be grouped with other props then passed into the renderProps method of a Camera to be rendered
*/
class Prop {
public:

	// constructor for texture variant
	Prop(float x, float y, float width, float height, Texture& tex, const std::string& typeName = "default", float z = 0.0f);

	// constructor for solid color variant
	Prop(float x, float y, float width, float height, std::array<float, 4>& color, const std::string& typeName = "default", float z = 0.0f);

	// constructor for animated variant
	Prop(float x, float y, float width, float height, AnimatedSprite& animatedSprite, const std::string& typeName = "default", float z = 0.0f);

	// optional: descripe what this thing is, for debugging or referencing i.e. if (getTypeName() == "powerup") { collect(); }
	void setTypeName(const std::string& typeName);
	const std::string& getTypeName();

	void setWidth(float width);
	float getWidth();

	// walls have height 1.0f
	void setHeight(float height);
	float getHeight();
		
	void setX(float x);
	float getX();

	void setY(float y);
	float getY();

	// 0.0f is floor level, 1.0f is wall height
	void setZ(float z);
	float getZ();

	// will return nullptr if not a texture variant
	Texture* getTexture();
	void bindTexture(Texture& tex);

	// will return nullptr if not a texture variant
	std::array<float, 4>* getColor();
	void bindColor(std::array<float, 4>& color);

	// will return nullptr if not an animated variant
	AnimatedSprite* getAnimatedSprite();
	void bindAnimatedSprite(AnimatedSprite& sprite);

private:
	std::string typeName;
	float width, height;
	float x, y, z;

	Texture* texture = nullptr;
	std::array<float, 4>* color = nullptr;
	AnimatedSprite* animatedSprite = nullptr;
};