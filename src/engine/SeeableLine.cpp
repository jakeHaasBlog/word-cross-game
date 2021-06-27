#include "engine/SeeableLine.h"

SeeableLine::SeeableLine(float x1, float y1, float x2, float y2, Texture * tex)
{
	this->tex = tex;
	line.x1 = x1;
	line.y1 = y1;
	line.x2 = x2;
	line.y2 = y2;
}

SeeableLine::SeeableLine(float x1, float y1, float x2, float y2, float r, float g, float b)
{
	this->tex = nullptr;
	line.x1 = x1;
	line.y1 = y1;
	line.x2 = x2;
	line.y2 = y2;
	color = { r, g, b };
}

bool SeeableLine::seenBy(Geo::LineSeg & ray, float & dist, float & intersectedAt, float& length, std::array<float, 2>* pointOfIntersection)
{
	std::vector<std::array<float, 2>> poi;
	if (line.getIntersection(ray, &poi)) {

		if (pointOfIntersection)
			*pointOfIntersection = poi[0];

		float x = poi[0][0];
		float y = poi[0][1];
		dist = sqrt((x - ray.x1) * (x - ray.x1) + (y - ray.y1) * (y - ray.y1));

		float dx = line.x2 - line.x1;
		float dy = line.y2 - line.y1;

		if (abs(dx) > 0.0f) {
			intersectedAt = (poi[0][0] - line.x1) / dx;
		}
		else {
			intersectedAt = (poi[0][1] - line.y1) / dy;
		}

		length = sqrt(dx * dx + dy * dy);

		return true;
	}
	else {
		return false;
	}
}

void SeeableLine::renderPrimitive()
{
	line.render(color[0], color[1], color[2]);
}

void SeeableLine::renderPrimitive(std::array<float, 2> translation, float scale)
{
	Geo::LineSeg lineCopy = line;
	lineCopy.x1 *= scale;
	lineCopy.y1 *= scale;
	lineCopy.x2 *= scale;
	lineCopy.y2 *= scale;
	lineCopy.x1 += translation[0];
	lineCopy.y1 += translation[1];
	lineCopy.x2 += translation[0];
	lineCopy.y2 += translation[1];
	lineCopy.render(color[0], color[1], color[2]);
}

void SeeableLine::setX1(float x)
{
	line.x1 = x;
}

void SeeableLine::setY1(float y)
{
	line.y1 = y;
}

float SeeableLine::getX1()
{
	return line.x1;
}

float SeeableLine::getY1()
{
	return line.y1;
}

void SeeableLine::setX2(float x)
{
	line.x2 = x;
}

void SeeableLine::setY2(float y)
{
	line.y2 = y;
}

float SeeableLine::getX2()
{
	return line.x2;
}

float SeeableLine::getY2()
{
	return line.y2;
}
