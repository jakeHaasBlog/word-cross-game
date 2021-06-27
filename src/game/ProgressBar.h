#pragma once

#include "engine/Geometry.h"
#include <array>

class ProgressBar {
public:
	ProgressBar(int min, int max, float x, float y, float width, float height);

	float getCurrent();
	void setCurrent(int x);

	void setPosition(float x, float y);
	void setSize(float width, float height);
	
	float getX();
	float getY();
	float getWidth();
	float getHeight();

	int getMin();
	int getMax();
	void setMin(int min);
	void setMax(int max);

	void render();

private:
	int current = 0;
	int min = 0;
	int max = 100;

	float x, y, width, height;

};