#pragma once

#include "engine/SeeableEntity.h"

class SeeableRectangle : public SeeableEntity {
public:
	SeeableRectangle(float x, float y, float width, float height, Texture* tex = nullptr);
	SeeableRectangle(float x, float y, float width, float height, float r, float g, float b);

	bool seenBy(Geo::LineSeg& ray, float& dist, float& intersectedAt, float& intersectedAtReal, std::array<float, 2>* poi = nullptr) override;

	virtual void renderPrimitive() override;
	virtual void renderPrimitive(std::array<float, 2> translation, float scale) override;

	void setX(float x);
	void setY(float y);
	void setWidth(float width);
	void setHeight(float height);

	float getX();
	float getY();
	float getWidth();
	float getHeight();

private:
	Geo::Rectangle rect;

};