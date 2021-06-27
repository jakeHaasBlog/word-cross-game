#pragma once

#include "engine/SeeableEntity.h"

class SeeableCircle : public SeeableEntity {
public:

	SeeableCircle(float x, float y, float radius, Texture* tex = nullptr);
	SeeableCircle(float x, float y, float radius, float r, float g, float b);

	virtual bool seenBy(Geo::LineSeg & ray, float & dist, float & intersectedAt, float& intersectedAtReal, std::array<float, 2>* poi = nullptr) override;
	virtual void renderPrimitive() override;
	virtual void renderPrimitive(std::array<float, 2> translation, float scale) override;

	void setX(float x);
	void setY(float y);
	void setRadius(float radius);

	float getX();
	float getY();
	float getRadius();

private:
	Geo::Circle circle;

};