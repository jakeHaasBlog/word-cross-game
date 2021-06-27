#pragma once

#include "engine/SeeableEntity.h"

class SeeableLine : public SeeableEntity {
public:
	SeeableLine(float x1, float y1, float x2, float y2, Texture* tex = nullptr);
	SeeableLine(float x1, float y1, float x2, float y2, float r, float g, float b);

	bool seenBy(Geo::LineSeg& ray, float& dist, float& intersectedAt, float& length, std::array<float, 2>* poi = nullptr) override;

	virtual void renderPrimitive() override;
	virtual void renderPrimitive(std::array<float, 2> translation, float scale) override;

	void setX1(float x);
	void setY1(float y);
	float getX1();
	float getY1();

	void setX2(float x);
	void setY2(float y);
	float getX2();
	float getY2();

private:
	Geo::LineSeg line;

};