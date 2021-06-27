#include "Prop.h"


Prop::Prop(float x, float y, float width, float height, Texture& tex, const std::string& typeName, float z) :
	x(x),
	y(y),
	width(width),
	height(height),
	texture(&tex),
	typeName(typeName),
	z(z)
{
}

Prop::Prop(float x, float y, float width, float height, std::array<float, 4>& color, const std::string& typeName, float z) :
	x(x),
	y(y),
	width(width),
	height(height),
	color(&color),
	typeName(typeName),
	z(z)
{
}

Prop::Prop(float x, float y, float width, float height, AnimatedSprite& animatedSprite, const std::string& typeName, float z) :
	x(x),
	y(y),
	width(width),
	height(height),
	animatedSprite(&animatedSprite),
	typeName(typeName),
	z(z)
{
}

void Prop::setTypeName(const std::string& typeName)
{
	this->typeName = typeName;
}

const std::string& Prop::getTypeName()
{
	return typeName;
}

void Prop::setWidth(float width)
{
	this->width = width;
}

float Prop::getWidth()
{
	return width;
}

void Prop::setHeight(float height)
{
	this->height = height;
}

float Prop::getHeight()
{
	return height;
}

void Prop::setX(float x)
{
	this->x = x;
}

float Prop::getX()
{
	return x;
}

void Prop::setY(float y)
{
	this->y = y;
}

float Prop::getY()
{
	return y;
}

void Prop::setZ(float z)
{
	this->z = z;
}

float Prop::getZ()
{
	return z;
}

void Prop::bindTexture(Texture& tex)
{
	texture = &tex;
}

Texture* Prop::getTexture()
{
	return texture;
}

void Prop::bindColor(std::array<float, 4>& color)
{
	this->color = &color;
}

std::array<float, 4>* Prop::getColor()
{
	return color;
}

void Prop::bindAnimatedSprite(AnimatedSprite& sprite)
{
	animatedSprite = &sprite;
}

AnimatedSprite* Prop::getAnimatedSprite()
{
	return animatedSprite;
}
