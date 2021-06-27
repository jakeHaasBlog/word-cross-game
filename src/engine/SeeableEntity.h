#pragma once

#include "engine/Geometry.h"
#include "engine/Texture.h"

class SeeableEntity {
public:

	virtual bool seenBy(Geo::LineSeg& ray, float& dist, float& intersectedAt, float& length, std::array<float, 2>* poi = nullptr) = 0;

	virtual void renderPrimitive() = 0;
	virtual void renderPrimitive(std::array<float, 2> translation, float scale) = 0;

	Texture* getTexture();
	float getR();
	float getG();
	float getB();

	void setR(float r);
	void setG(float g);
	void setB(float b);

	void setColor(float r, float g, float b);

protected:
	std::array<float, 3> color = { 1, 1, 1 };
	Texture* tex = nullptr;

};