#include "ProgressBar.h"

ProgressBar::ProgressBar(int min, int max, float x, float y, float width, float height) :
	x(x),
	y(y),
	width(width),
	height(height),
	min(min),
	max(max)
{
}

float ProgressBar::getCurrent()
{
	return current;
}

void ProgressBar::setCurrent(int x)
{
	current = x;
}

void ProgressBar::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void ProgressBar::setSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

float ProgressBar::getX()
{
	return x;
}

float ProgressBar::getY()
{
	return y;
}

float ProgressBar::getWidth()
{
	return width;
}

float ProgressBar::getHeight()
{
	return height;
}

int ProgressBar::getMin()
{
	return min;
}

int ProgressBar::getMax()
{
	return max;
}

void ProgressBar::setMin(int min)
{
	this->min = min;
}

void ProgressBar::setMax(int max)
{
	this->max = max;
}

void ProgressBar::render()
{
	Geo::Rectangle::fillRect(x, y, width, height, 0.5f, 0.5f, 0.5f);
	float border = 0.01f;

	float maxLength = width - 0.01f * 2.0f;
	float currentLength = maxLength * ((float)(current - min) / (max - min));

	if (current > max) {
		Geo::Rectangle::fillRect(x + border, y + border, maxLength, height - border * 2, 1.0f, 0.0f, 0.0f);
	} 
	else if (current > min) {
		Geo::Rectangle::fillRect(x + border, y + border, currentLength, height - border * 2, 1.0f, 0.0f, 0.0f);
	}
}
