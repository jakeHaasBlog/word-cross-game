#pragma once

#include <functional>
#include "engine/Geometry.h"

#include <array>

class Button {
public:
	Button(float x, float y, float width, float height, const std::function<void()>& function, std::string prompt = "");

	void setPosition(float x, float y);
	float getX();
	float getY();

	void setSize(float width, float height);
	float getWidth();
	float getHeight();

	void setTextXOffset(float x);
	float getXOffset();

	void setTextYOffset(float y);
	float getYOffset();

	void setPressFunction(const std::function<void()>& function);

	void press();
	void mouseListener(float x, float y, int button, int action, int mods);

	void render();


private:
	float x = 0.0f, y = 0.0f, width = 0.4f, height = 0.2f;

	std::function<void()> buttonFunction;

	std::array<float, 4> bg_color;
	std::array<float, 4> fg_color;

	std::string buttonText = "";
	float buttonTextSize = 0.03f;
	float buttonTextXOffset = 0.0f;
	float buttonTextYOffset = 0.0f;

};