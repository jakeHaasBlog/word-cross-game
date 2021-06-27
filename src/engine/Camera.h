#pragma once

#include <array>

#include "engine/SeeableEntity.h"
#include "engine/TexturedQuad.h"
#include "engine/Prop.h"

// namespace impl is used so end users know not to use its contents, in this case the stuct RayIntersectInfo is only a utility used in the private methods of this class
namespace impl {
	struct RayIntersectInfo {
		bool hasIntersection;
		std::array<float, 2> origin;
		std::array<float, 2> end; // the point of intersection
		float intersectDistance;
		float intersectedAt; // float between 0 and 1 that describes where along the SeeableEntity the ray intersected (this is for rendering textures)
		float intersectedAtReal; // float between 0 and the length of the SeeableEntity decribing where along its length the intersection occured

		SeeableEntity* entity; // pointer to the SeeableEntity that was intersected 
		Prop* prop;
	};
}

class Camera {
public:
	Camera(float x, float y, float direction, float fov, int rayCount);

	void renderPrimitiveRays(std::array<float, 2> translation, float scale);
	void renderPrimitiveRays(std::array<float, 2> translation, float scale, std::vector<SeeableEntity*>& seeableEntities, std::vector<Prop*>& props);

	void renderView(std::vector<SeeableEntity*>& seeableEntities, std::vector<Prop*>& props);

	void setX(float x);
	void setY(float y);
	void setDirection(float direction);

	float getX();
	float getY();
	float getDirection();

private:
	float x, y;
	float direction;
	float fov;
	int rayCount;

	// returns a struct of data describing the closest intersection of the given ray with the given SeeableEntities
	impl::RayIntersectInfo getClosestIntersection(Geo::LineSeg& ray, std::vector<SeeableEntity*>& seeableEntities);

	// renders an individual strip given required information about where to put the strip and information about the ray intersection it represents
	void renderRayStrip(impl::RayIntersectInfo intersectionInfo, float rayDirection, TexturedQuad& renderArea);


	std::vector<impl::RayIntersectInfo> getPropIntersectionsSorted(Geo::LineSeg& ray, std::vector<Prop*>& props);
	void renderProps(std::vector<impl::RayIntersectInfo>& sortedPropIntersections, impl::RayIntersectInfo& closestWallIntersection, float rayDirection, TexturedQuad& renderArea);

};