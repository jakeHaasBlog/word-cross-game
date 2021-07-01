#include "Button.h"

#include "engine/BitmapText.h"

Button::Button(float x, float y, float width, float height, const std::function<void()>& function, std::string prompt) :
	x(x),
	y(y),
	width(width),
	height(height),
	buttonFunction(function),
	buttonText(prompt),
	bg_color({ 0.5f, 0.5f, 0.5f, 1.0f }),
	fg_color({ 1.0f, 1.0f, 1.0f, 1.0f })
{
}

void Button::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Button::setSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

float Button::getX()
{
	return x;
}

float Button::getY()
{
	return y;
}

float Button::getWidth()
{
	return width;
}

float Button::getHeight()
{
	return height;
}

void Button::setTextXOffset(float x)
{
	buttonTextXOffset = x;
}

float Button::getXOffset()
{
	return buttonTextXOffset;
}

void Button::setTextYOffset(float y)
{
	buttonTextYOffset = y;
}

void Button::setPrompt(const std::string& prompt)
{
	buttonText = prompt;
}

void Button::setTextHeight(float height)
{
	buttonTextSize = height;
}

float Button::getYOffset()
{
	buttonTextYOffset;
}

std::string Button::getPrompt()
{
	return buttonText;
}

float Button::getTextHeight()
{
	return buttonTextSize;
}

void Button::setPressFunction(const std::function<void()>& function)
{
	this->buttonFunction = function;
}

void Button::setForegroundColor(float r, float g, float b, float a)
{
	fg_color = { r, g, b, a };
}

void Button::setBackgroundColor(float r, float g, float b, float a)
{
	bg_color = { r, g, b, a };
}

void Button::press()
{
	buttonFunction();
}

void Button::mouseListener(float x, float y, int button, int action, int mods)
{
	Geo::Rectangle rect = Geo::Rectangle(this->x, this->y + height, width, height);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (rect.containsPoint(x, y)) {
			press();
		}

		isDown = false;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (rect.containsPoint(x, y)) {
			isDown = true;
		}
	}

}

void Button::render()
{
	auto preFGcolor = fg_color;
	auto preBGcolor = bg_color;

	if (isDown) {
		for (float& c : fg_color) c /= 2;
		for (float& c : bg_color) c /= 2;
	}

	float borderWidth = 0.01f;
	Geo::Rectangle::fillRect(x, y, width, height, fg_color[0], fg_color[1], fg_color[2], fg_color[3]);
	Geo::Rectangle::fillRect(x + borderWidth, y + borderWidth, width - borderWidth * 2, height - borderWidth * 2, bg_color[0], bg_color[1], bg_color[2], bg_color[3]);

	if (buttonText.length() > 0) {
		static BitmapText text;
		text.setFont(FontManager::getFont("assets/fonts/arial"));
		text.setText(buttonText);
		text.setColor(fg_color[0], fg_color[1], fg_color[2], fg_color[3]);
		text.setCharHeight(buttonTextSize);
		text.setPosition({ x + buttonTextXOffset, y + buttonTextYOffset });
		text.render();
	}

	fg_color = preFGcolor;
	bg_color = preBGcolor;
	
}
