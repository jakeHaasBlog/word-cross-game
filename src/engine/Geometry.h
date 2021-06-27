#pragma once

#include "engine/OpenglBufferObjects.h"
#include "engine/Shader.h"

#include <array>
#include <vector>

namespace Geo {

	class Rectangle;
	class Circle;
	class LineSeg;

	class GeoObject {
	protected:
		virtual VertexBuffer& getVB() = 0;
		virtual IndexBuffer& getIB() = 0;
		virtual VertexArray& getVA() = 0;
		void render(Shader& shader);
		void render(Shader& shader, VertexBuffer& vb, IndexBuffer& ib, VertexArray& va, int inputType);
	};

	class Rectangle : protected GeoObject {
	public:
		Rectangle(float x, float y, float width, float height);
		Rectangle();

	protected:
		VertexBuffer& getVB() override;
		IndexBuffer& getIB() override;
		VertexArray& getVA() override;

		static Rectangle& getStencyl();

	public:
		float x, y, width, height;
		void renderFilled(float r, float g, float b, float a = 1.0f);
		void renderOutline(float r, float g, float b, float a = 1.0f);

		static void fillRect(float x, float y, float width, float height, float r, float g, float b, float a = 1.0f);
		static void outlineRect(float x, float y, float width, float height, float r, float g, float b, float a = 1.0f);

		bool getIntersection(const Geo::LineSeg& line, std::vector<std::array<float, 2>>* poi = nullptr) const;
		bool getIntersection(const Geo::Rectangle& rect, std::vector<std::array<float, 2>>* poi = nullptr) const;
		bool getIntersection(const Geo::Circle& circle, std::vector<std::array<float, 2>>* poi = nullptr) const;

		bool containsPoint(float x, float y) const;
	};

	class Circle : protected GeoObject {
	public:
		Circle(float x, float y, float radius);
		Circle();
	protected:
		VertexBuffer& getVB() override;
		IndexBuffer& getIB() override;
		VertexArray& getVA() override;

		static Circle& getStencyl();

		const static unsigned int lowResCircleVertexCount;
		static VertexBuffer& getLowResCircleVB();
		static IndexBuffer& getLowResCircleIB();
		static VertexArray& getLowResCircleVA();

		const static unsigned int medResCircleVertexCount;
		static VertexBuffer& getMedResCircleVB();
		static IndexBuffer& getMedResCircleIB();
		static VertexArray& getMedResCircleVA();

		const static unsigned int highResCircleVertexCount;
		static VertexBuffer& getHighResCircleVB();
		static IndexBuffer& getHighResCircleIB();
		static VertexArray& getHighResCircleVA();

	public:
		float x, y, radius;
		void renderFilled(float r, float g, float b, float a = 1.0f);
		void renderOutline(float r, float g, float b, float a = 1.0f);

		static void fillCircle(float x, float y, float radius, float r, float g, float b, float a = 1.0f);
		static void outlineCircle(float x, float y, float radius, float r, float g, float b, float a = 1.0f);

		bool getIntersection(const Geo::Circle& circle, std::vector<std::array<float, 2>>* poi = nullptr);
		bool getIntersection(const Geo::Rectangle& rect, std::vector<std::array<float, 2>>* poi = nullptr);
		bool getIntersection(const Geo::LineSeg& line, std::vector<std::array<float, 2>>* poi = nullptr);

		bool containsPoint(float _x, float _y) const;
	};

	class LineSeg : protected GeoObject {
	public:
		LineSeg(float x1, float y1, float x2, float y2);
		LineSeg();
	protected:
		virtual VertexBuffer & getVB() override;
		virtual IndexBuffer & getIB() override;
		virtual VertexArray & getVA() override;
	public:
		float x1, y1;
		float x2, y2;
		void render(float r, float g, float b, float a = 1.0f);
		static void renderLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a = 1.0f);

		bool getIntersection(const Geo::Circle& circle, std::vector<std::array<float, 2>>* poi = nullptr);
		bool getIntersection(const Geo::LineSeg& line, std::vector<std::array<float, 2>>* poi = nullptr);
		bool getIntersection(const Geo::Rectangle& rect, std::vector<std::array<float, 2>>* poi = nullptr);

		float getLength();
	};
}