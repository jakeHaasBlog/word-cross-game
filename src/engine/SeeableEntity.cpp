#include "engine/SeeableEntity.h"

Texture * SeeableEntity::getTexture() {
	return tex;
}

float SeeableEntity::getR() {
	return color[0];
}

float SeeableEntity::getG() {
	return color[1];
}

float SeeableEntity::getB() {
	return color[2];
}

void SeeableEntity::setR(float r) {
	color[0] = r;
}

void SeeableEntity::setG(float g) {
	color[1] = g;
}

void SeeableEntity::setB(float b) {
	color[2] = b;
}

void SeeableEntity::setColor(float r, float g, float b) {
	color = { r, g, b };
}
