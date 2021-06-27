#include "engine/SeeableRectangle.h"

#include "engine/SeeableLine.h"

SeeableRectangle::SeeableRectangle(float x, float y, float width, float height, Texture * tex)
{
	this->tex = tex;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
}

SeeableRectangle::SeeableRectangle(float x, float y, float width, float height, float r, float g, float b)
{
	this->tex = nullptr;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	color = { r, g, b };
}


bool SeeableRectangle::seenBy(Geo::LineSeg & ray, float & dist, float & intersectedAt, float& intersectedAtReal, std::array<float, 2>* pointOfIntersection)
{
	Geo::LineSeg l1(rect.x              , rect.y               , rect.x + rect.width , rect.y              );
	Geo::LineSeg l2(rect.x + rect.width , rect.y               , rect.x + rect.width , rect.y + rect.height);
	Geo::LineSeg l3(rect.x + rect.width , rect.y + rect.height , rect.x              , rect.y + rect.height);
	Geo::LineSeg l4(rect.x              , rect.y + rect.height , rect.x              , rect.y              );
	std::vector<std::array<float, 2>> poi = {};

	Geo::LineSeg* closestLine = nullptr;
	std::array<float, 2> closestPOI;
	float closestDistSQ = 0.0f;
	bool isIntersecting = false;

	if (l1.getIntersection(ray, &poi)) {
		float distSQ = pow(ray.x1 - poi[poi.size() - 1][0], 2) + pow(ray.y1 - poi[poi.size() - 1][1], 2);
		if (distSQ < closestDistSQ || !isIntersecting) {
			closestDistSQ = distSQ;
			closestPOI = poi[poi.size() - 1];
			closestLine = &l1;
		}
		isIntersecting = true;
	}
	if (l2.getIntersection(ray, &poi)) {
		float distSQ = pow(ray.x1 - poi[poi.size() - 1][0], 2) + pow(ray.y1 - poi[poi.size() - 1][1], 2);
		if (distSQ < closestDistSQ || !isIntersecting) {
			closestDistSQ = distSQ;
			closestPOI = poi[poi.size() - 1];
			closestLine = &l2;
		}
		isIntersecting = true;
	}
	if (l3.getIntersection(ray, &poi)) {
		float distSQ = pow(ray.x1 - poi[poi.size() - 1][0], 2) + pow(ray.y1 - poi[poi.size() - 1][1], 2);
		if (distSQ < closestDistSQ || !isIntersecting) {
			closestDistSQ = distSQ;
			closestPOI = poi[poi.size() - 1];
			closestLine = &l3;
		}
		isIntersecting = true;
	}
	if (l4.getIntersection(ray, &poi)) {
		float distSQ = pow(ray.x1 - poi[poi.size() - 1][0], 2) + pow(ray.y1 - poi[poi.size() - 1][1], 2);
		if (distSQ < closestDistSQ || !isIntersecting) {
			closestDistSQ = distSQ;
			closestPOI = poi[poi.size() - 1];
			closestLine = &l4;
		}
		isIntersecting = true;
	}

	if (isIntersecting) {

		//if (pointOfIntersection)
		//	*pointOfIntersection = closestPOI;
		//
		//dist = sqrt(closestDistSQ);
		//
		//float dx = closestLine->x2 - closestLine->x1;
		//float dy = closestLine->y2 - closestLine->y1;
		//
		//if (abs(dx) > 0.0f) intersectedAt = (closestPOI[0] - closestLine->x1) / dx;
		//else intersectedAt = (closestPOI[1] - closestLine->y1) / dy;
		//
		//intersectedAtReal = intersectedAt * sqrt(pow(dx, 2) + pow(dy, 2));

		SeeableLine line = SeeableLine(closestLine->x1, closestLine->y1, closestLine->x2, closestLine->y2, tex);
		line.seenBy(ray, dist, intersectedAt, intersectedAtReal, pointOfIntersection);

		return true;
	}
	else {
		return false;
	}
}

void SeeableRectangle::renderPrimitive()
{
	rect.renderOutline(color[0], color[1], color[2]);
}

void SeeableRectangle::renderPrimitive(std::array<float, 2> translation, float scale)
{
	Geo::Rectangle rectCopy = rect;
	rectCopy.width *= scale;
	rectCopy.height *= scale;
	rectCopy.x += translation[0];
	rectCopy.y += translation[1];
	rectCopy.renderOutline(color[0], color[1], color[2]);
}

void SeeableRectangle::setX(float x)
{
	rect.x = x;
}

void SeeableRectangle::setY(float y)
{
	rect.y = y;
}

void SeeableRectangle::setWidth(float width)
{
	rect.width = width;
}

void SeeableRectangle::setHeight(float height)
{
	rect.height = height;
}

float SeeableRectangle::getX()
{
	return rect.x;
}

float SeeableRectangle::getY()
{
	return rect.y;
}

float SeeableRectangle::getWidth()
{
	return rect.width;
}

float SeeableRectangle::getHeight()
{
	return rect.height;
}
