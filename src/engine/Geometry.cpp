
#include "Geometry.h"

#include <stdio.h>
#include "engine/Window.h"

#include "engine/ViewportManager.h"

namespace {
	bool getRectRectIntersection(const Geo::Rectangle& rect1, const Geo::Rectangle& rect2, std::vector<std::array<float, 2>>* poi = nullptr) {

		if (!poi) {
			if (rect1.x < rect2.x + rect2.width &&
				rect1.x + rect1.width > rect2.x &&
				rect1.y < rect2.y + rect2.height &&
				rect1.y + rect1.height > rect2.y)
			{
				return true;
			}
			return false;
		}
		else {
			bool intersecting = false;

			if (rect2.x <= rect1.x && rect1.x <= rect2.x + rect2.width) {
				if (rect1.y - rect1.height <= rect2.y && rect2.y <= rect1.y) {
					intersecting = true;
					poi->push_back({ rect1.x, rect2.y });
				}
				if (rect1.y - rect1.height <= rect2.y - rect2.height && rect2.y - rect2.height <= rect1.y) {
					intersecting = true;
					poi->push_back({ rect1.x, rect2.y - rect2.height});
				}
			}

			if (rect2.x <= rect1.x + rect1.width && rect1.x + rect1.width <= rect2.x + rect2.width) {
				if (rect1.y - rect1.height <= rect2.y && rect2.y <= rect1.y) {
					intersecting = true;
					poi->push_back({ rect1.x + rect1.width, rect2.y });
				}
				if (rect1.y - rect1.height <= rect2.y - rect2.height && rect2.y - rect2.height <= rect1.y) {
					intersecting = true;
					poi->push_back({ rect1.x + rect1.width, rect2.y - rect2.height });
				}
			}

			if (rect2.y - rect2.height <= rect1.y && rect1.y <= rect2.y) {
				if (rect1.x <= rect2.x && rect2.x <= rect1.x + rect1.width) {
					intersecting = true;
					poi->push_back({ rect2.x, rect1.y });
				}
				if (rect1.x <= rect2.x + rect2.width && rect2.x + rect2.width <= rect1.x + rect1.width) {
					intersecting = true;
					poi->push_back({ rect2.x + rect2.width, rect1.y });
				}
			}

			if (rect2.y - rect2.height <= rect1.y - rect1.height && rect1.y - rect1.height <= rect2.y) {
				if (rect1.x <= rect2.x && rect2.x <= rect1.x + rect1.width) {
					intersecting = true;
					poi->push_back({ rect2.x, rect1.y - rect1.height });
				}
				if (rect1.x <= rect2.x + rect2.width && rect2.x + rect2.width <= rect1.x + rect1.width) {
					intersecting = true;
					poi->push_back({ rect2.x + rect2.width, rect1.y - rect1.height });
				}
			}

			if (rect1.containsPoint(rect2.x, rect2.y)) intersecting = true;
			if (rect1.containsPoint(rect2.x + rect2.width, rect2.y)) intersecting = true;
			if (rect1.containsPoint(rect2.x + rect2.width, rect2.y - rect2.height)) intersecting = true;
			if (rect1.containsPoint(rect2.x, rect2.y - rect2.height)) intersecting = true;

			if (rect2.containsPoint(rect1.x, rect1.y)) intersecting = true;
			if (rect2.containsPoint(rect1.x + rect1.width, rect1.y)) intersecting = true;
			if (rect2.containsPoint(rect1.x + rect1.width, rect1.y - rect1.height)) intersecting = true;
			if (rect2.containsPoint(rect1.x, rect1.y - rect1.height)) intersecting = true;

			return intersecting;
		}
	}

	bool getLineCircleIntersection(const Geo::LineSeg& line, const Geo::Circle& circle, std::vector<std::array<float, 2>>* poi) {
		
		float r = circle.radius;

		float a = -(line.y2 - line.y1);
		float b = line.x2 - line.x1;
		float c = -a * (line.x1 - circle.x) - b * (line.y1 - circle.y);

		float epsilon = 0.000001f;

		bool intersecting = false;

		float x0 = -a * c / (a*a + b * b), y0 = -b * c / (a*a + b * b);
		if (c*c > r*r*(a*a + b * b) + epsilon)
			return false;
		else if (abs(c*c - r * r*(a*a + b * b)) < epsilon) {
			if ((fmin(line.x1, line.x2) - epsilon <= x0 + circle.x && x0 + circle.x <= fmax(line.x1, line.x2) + epsilon))
				if ((fmin(line.y1, line.y2) - epsilon <= y0 + circle.y && y0 + circle.y <= fmax(line.y1, line.y2) + epsilon)) {
					if (poi) poi->push_back({ x0 + circle.x, y0 + circle.y });
					intersecting = true;
				}
		}
		else {
			float d = r * r - c * c / (a*a + b * b);
			float mult = sqrt(d / (a*a + b * b));
			float ax, ay, bx, by;
			ax = x0 + b * mult;
			bx = x0 - b * mult;
			ay = y0 - a * mult;
			by = y0 + a * mult;

			if ((fmin(line.x1, line.x2) - epsilon <= ax + circle.x && ax + circle.x <= fmax(line.x1, line.x2) + epsilon))
				if ((fmin(line.y1, line.y2) - epsilon <= ay + circle.y && ay + circle.y <= fmax(line.y1, line.y2) + epsilon)) {
					if (poi) poi->push_back({ ax + circle.x, ay + circle.y });
					intersecting = true;
				}
			if ((fmin(line.x1, line.x2) - epsilon <= bx + circle.x && bx + circle.x <= fmax(line.x1, line.x2) + epsilon))
				if ((fmin(line.y1, line.y2) - epsilon <= by + circle.y && by + circle.y <= fmax(line.y1, line.y2) + epsilon)) {
					if (poi) poi->push_back({ bx + circle.x, by + circle.y });
					intersecting = true;
				}
		}

		if (circle.containsPoint(line.x1, line.y1) || circle.containsPoint(line.x2, line.y2))
			intersecting = true;

		return intersecting;

	}

	bool getRectCircleIntersection(const Geo::Rectangle& rect, const Geo::Circle& circle, std::vector<std::array<float, 2>>* poi) {
		if (!poi) {
			float deltaX = circle.x - fmax(rect.x, fmin(circle.x, rect.x + rect.width));
			float deltaY = circle.y - fmin(rect.y, fmax(circle.y, rect.y - rect.height));
			return (deltaX * deltaX + deltaY * deltaY) < (circle.radius * circle.radius) || rect.containsPoint(circle.x, circle.y);
		}
		else {
			bool intersected = false;
			Geo::LineSeg line2 = { rect.x, rect.y, rect.x + rect.width, rect.y };
			if (getLineCircleIntersection(line2, circle, poi)) {
				intersected = true;
			}

			line2 = { rect.x, rect.y - rect.height, rect.x + rect.width, rect.y - rect.height };
			if (getLineCircleIntersection(line2, circle, poi)) {
				intersected = true;
			}

			line2 = { rect.x, rect.y, rect.x, rect.y - rect.height };
			if (getLineCircleIntersection(line2, circle, poi)) {
				intersected = true;
			}
			
			line2 = { rect.x + rect.width, rect.y, rect.x + rect.width, rect.y - rect.height };
			if (getLineCircleIntersection(line2, circle, poi)) {
				intersected = true;
			}

			if (rect.containsPoint(circle.x, circle.y))
				intersected = true;

			return intersected;
		}
	}

	bool getLineLineIntersection(const Geo::LineSeg& line1, const Geo::LineSeg& line2, std::vector<std::array<float, 2>>* poi) {
		
		float p[2] = { line1.x1, line1.y1 };
		float t;
		float r[2] = { line1.x2 - line1.x1, line1.y2 - line1.y1 };
		// p + tr

		float q[2] = { line2.x1, line2.y1 };
		float u;
		float s[2] = { line2.x2 - line2.x1, line2.y2 - line2.y1 };
		// q + us

		float rXs = r[0] * s[1] - r[1] * s[0];
		float qMinusP[2] = { q[0] - p[0], q[1] - p[1] };

		float qMinisPCrossS = qMinusP[0] * s[1] - qMinusP[1] * s[0];
		float qMinusPCrossR = qMinusP[0] * r[1] - qMinusP[1] * r[0];

		t = qMinisPCrossS / (r[0] * s[1] - s[0] * r[1]);
		u = qMinusPCrossR / rXs;

		if (rXs != 0.0f && 0.0f <= t && t <= 1.0f && 0 <= u && u <= 1.0f) {
			if (poi) {
				poi->push_back({ p[0] + t * r[0], p[1] + t * r[1] });
			}
			return true;
		}
		else {
			return false;
		}

	}

	bool getLineRectIntersection(const Geo::Rectangle& rect, const Geo::LineSeg& line, std::vector<std::array<float, 2>>* poi) {

		if (!poi) {
			if (rect.containsPoint(line.x1, line.y1)) return true;
			if (rect.containsPoint(line.x1, line.y1)) return true;

			Geo::LineSeg line2 = { rect.x, rect.y, rect.x + rect.width, rect.y };
			if (getLineLineIntersection(line, line2, poi)) {
				return true;
			} 

			line2.x2 = rect.x;
			line2.y2 = rect.y - rect.height;
			if (getLineLineIntersection(line, line2, poi)) {
				return true;
			}

			line2.x1 = rect.x + rect.width;
			line2.y1 = rect.y - rect.height;
			if (getLineLineIntersection(line, line2, poi)) {
				return true;
			}

			line2.x1 = rect.x + rect.width;
			line2.y1 = rect.y;
			if (getLineLineIntersection(line, line2, poi)) {
				return true;
			}

			return false;
		}
		else {

			bool intersection = false;

			if (rect.containsPoint(line.x1, line.y1)) intersection = true;
			if (rect.containsPoint(line.x1, line.y1)) intersection = true;

			Geo::LineSeg line2 = { rect.x, rect.y, rect.x + rect.width, rect.y };
			if (getLineLineIntersection(line, line2, poi)) {
				intersection = true;
			}

			line2.x2 = rect.x;
			line2.y2 = rect.y - rect.height;
			if (getLineLineIntersection(line, line2, poi)) {
				intersection = true;
			}

			line2.x1 = rect.x + rect.width;
			line2.y1 = rect.y - rect.height;
			if (getLineLineIntersection(line, line2, poi)) {
				intersection = true;
			}

			line2.x1 = rect.x + rect.width;
			line2.y1 = rect.y;
			line2.x2 = rect.x + rect.width;
			line2.y2 = rect.y - rect.height;
			if (getLineLineIntersection(line, line2, poi)) {
				intersection = true;
			}

			return intersection;
		}

	}

}


void Geo::GeoObject::render(Shader & shader, VertexBuffer & vb, IndexBuffer & ib, VertexArray & va, int inputType)
{
	shader.bind();

	va.bind();
	ib.bind();
	glDrawElements(inputType, ib.getCount(), GL_UNSIGNED_INT, nullptr);
	va.unbind();
	ib.unbind();
}

void Geo::GeoObject::render(Shader& shader)
{
	render(shader, getVB(), getIB(), getVA(), GL_TRIANGLES);
}


Geo::Rectangle::Rectangle(float _x, float _y, float _width, float _height)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;
}

Geo::Rectangle::Rectangle()
{
	x = 0.0f;
	y = 0.0f;
	width = 1.0f;
	height = 1.0f;
}

VertexBuffer & Geo::Rectangle::getVB()
{
	static float vertices[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	static VertexBuffer vb = VertexBuffer(vertices, 8 * sizeof(float));

	return vb;
}

IndexBuffer & Geo::Rectangle::getIB()
{
	static unsigned int indices[6] = {
		2, 0, 1,
		0, 2, 3
	};

	static IndexBuffer ib = IndexBuffer(indices, 6 * sizeof(unsigned int));

	return ib;
}

VertexArray & Geo::Rectangle::getVA()
{
	static VertexArray va = VertexArray("ff", getVB(), getIB());
	return va;
}

Geo::Rectangle& Geo::Rectangle::getStencyl()
{
	static Rectangle rect;
	return rect;
}

void Geo::Rectangle::renderFilled(float r, float g, float b, float a)
{
	static std::string vertexShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"\n"
		"uniform vec2 u_stretch;\n"
		"uniform vec2 u_translation;\n"
		"uniform float u_aspectRatio;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4((position[0] / u_aspectRatio) * u_stretch[0] + (u_translation[0] / u_aspectRatio), position[1] * u_stretch[1] + u_translation[1], 0, 1);\n"
		"	gl_Position[2] = 0;\n"
		"};\n";

	static std::string fragmentShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"uniform vec4 u_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = u_color;\n"
		"};\n";

	static Shader fillShader(vertexShaderString, fragmentShaderString);

	fillShader.setUniform1f("u_aspectRatio", ViewportManager::getCurrentAspectRatio());
	fillShader.setUniform2f("u_stretch", width, height);
	fillShader.setUniform2f("u_translation", x, y);
	fillShader.setUniform4f("u_color", r, g, b, a);

	render(fillShader);
}

void Geo::Rectangle::renderOutline(float r, float g, float b, float a)
{
	static std::string vertexShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"\n"
		"uniform vec2 u_stretch;\n"
		"uniform vec2 u_translation;\n"
		"uniform float u_aspectRatio;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4((position[0] / u_aspectRatio) * u_stretch[0] + (u_translation[0] / u_aspectRatio), position[1] * u_stretch[1] + u_translation[1], 0, 1);\n"
		"	gl_Position[2] = 0;\n"
		"};\n";

	static std::string fragmentShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"uniform vec4 u_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = u_color;\n"
		"};\n";

	static Shader lineShader(vertexShaderString, fragmentShaderString);

	static unsigned int lineIndices[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,
	};
	static IndexBuffer lineIB = IndexBuffer(lineIndices, 2 * 4 * sizeof(unsigned int));

	lineShader.setUniform1f("u_aspectRatio", ViewportManager::getCurrentAspectRatio());
	lineShader.setUniform2f("u_stretch", width, height);
	lineShader.setUniform2f("u_translation", x, y);
	lineShader.setUniform4f("u_color", r, g, b, a);

	render(lineShader, getVB(), lineIB, getVA(), GL_LINES);
}

void Geo::Rectangle::fillRect(float x, float y, float width, float height, float r, float g, float b, float a)
{
	Rectangle& rect = getStencyl();
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	rect.renderFilled(r, g, b, a);
}

void Geo::Rectangle::outlineRect(float x, float y, float width, float height, float r, float g, float b, float a)
{
	Rectangle& rect = getStencyl();
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	rect.renderOutline(r, g, b, a);
}

bool Geo::Rectangle::getIntersection(const Geo::LineSeg & line, std::vector<std::array<float, 2>>* poi) const
{
	return getLineRectIntersection(*this, line, poi);
}

bool Geo::Rectangle::getIntersection(const Geo::Rectangle & rect, std::vector<std::array<float, 2>>* poi) const
{
	return getRectRectIntersection(*this, rect, poi);
}

bool Geo::Rectangle::getIntersection(const Geo::Circle & circle, std::vector<std::array<float, 2>>* poi) const
{
	return getRectCircleIntersection(*this, circle, poi);
}

bool Geo::Rectangle::containsPoint(float _x, float _y) const
{
	if (x <= _x && _x <= x + width) {
		if (y - height <= _y && _y <= y) {
			return true;
		}
	}
	return false;
}




const unsigned int Geo::Circle::lowResCircleVertexCount = 20;
const unsigned int Geo::Circle::medResCircleVertexCount = 50;
const unsigned int Geo::Circle::highResCircleVertexCount = 120;

Geo::Circle::Circle(float _x, float _y, float _radius)
{
	x = _x;
	y = _y;
	radius = _radius;
}

Geo::Circle::Circle()
{
	x = 0.0f;
	y = 0.0f;
	radius = 0.5f;
}

VertexBuffer & Geo::Circle::getVB()
{
	static float vertices[8] = {
		-0.5f, 0.5f,
		0.5f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};

	static VertexBuffer vb = VertexBuffer(vertices, 8 * sizeof(float));

	return vb;
}

IndexBuffer & Geo::Circle::getIB()
{
	static unsigned int indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	static IndexBuffer ib = IndexBuffer(indices, 6 * sizeof(unsigned int));

	return ib;
}

VertexArray & Geo::Circle::getVA()
{
	static VertexArray va = VertexArray("ff", getVB(), getIB());
	return va;
}

Geo::Circle& Geo::Circle::getStencyl()
{
	static Circle circle;
	return circle;
}

VertexBuffer& Geo::Circle::getLowResCircleVB()
{
	static float vertices[lowResCircleVertexCount * 2];
	static bool calculated = false;

	if (!calculated) {
		for (int i = 0; i < lowResCircleVertexCount; i++) {
			vertices[i * 2 + 0] = cos(((float)i / lowResCircleVertexCount) * 3.14159f * 2.0f) / 2;
			vertices[i * 2 + 1] = sin(((float)i / lowResCircleVertexCount) * 3.14159f * 2.0f) / 2;
		}
		calculated = true;
	}

	static VertexBuffer vb = VertexBuffer(vertices, 2 * lowResCircleVertexCount * sizeof(float));
	return vb;
}

IndexBuffer& Geo::Circle::getLowResCircleIB()
{
	static unsigned int indices[lowResCircleVertexCount * 2];

	static bool calculated = false;
	if (!calculated) {

		for (unsigned int i = 0; i < lowResCircleVertexCount; i++) {
			indices[i * 2 + 0] = i;
			indices[i * 2 + 1] = i + 1;
		}
		indices[lowResCircleVertexCount * 2 - 1] = 0;

		calculated = true;
	}

	static IndexBuffer ib = IndexBuffer(indices, lowResCircleVertexCount * 2 * sizeof(unsigned int));
	return ib;
}

VertexArray& Geo::Circle::getLowResCircleVA() {
	static VertexArray va = VertexArray("ff", getLowResCircleVB(), getLowResCircleIB());
	return va;
}


VertexBuffer& Geo::Circle::getMedResCircleVB()
{
	static float vertices[medResCircleVertexCount * 2];
	static bool calculated = false;

	if (!calculated) {
		for (int i = 0; i < medResCircleVertexCount; i++) {
			vertices[i * 2 + 0] = cos(((float)i / medResCircleVertexCount) * 3.14159f * 2.0f) / 2;
			vertices[i * 2 + 1] = sin(((float)i / medResCircleVertexCount) * 3.14159f * 2.0f) / 2;
		}
		calculated = true;
	}

	static VertexBuffer vb = VertexBuffer(vertices, 2 * medResCircleVertexCount * sizeof(float));
	return vb;
}

IndexBuffer& Geo::Circle::getMedResCircleIB()
{
	static unsigned int indices[medResCircleVertexCount * 2];
	
	static bool calculated = false;
	if (!calculated) {

		for (unsigned int i = 0; i < medResCircleVertexCount; i++) {
			indices[i * 2 + 0] = i;
			indices[i * 2 + 1] = i + 1;
		}
		indices[medResCircleVertexCount * 2 - 1] = 0;

		calculated = true;
	}

	static IndexBuffer ib = IndexBuffer(indices, medResCircleVertexCount * 2 * sizeof(unsigned int));
	return ib;
}

VertexArray& Geo::Circle::getMedResCircleVA() {
	static VertexArray va = VertexArray("ff", getMedResCircleVB(), getMedResCircleIB());
	return va;
}

VertexBuffer& Geo::Circle::getHighResCircleVB()
{
	static float vertices[highResCircleVertexCount * 2];
	static bool calculated = false;

	if (!calculated) {
		for (int i = 0; i < highResCircleVertexCount; i++) {
			vertices[i * 2 + 0] = cos(((float)i / highResCircleVertexCount) * 3.14159f * 2.0f) / 2;
			vertices[i * 2 + 1] = sin(((float)i / highResCircleVertexCount) * 3.14159f * 2.0f) / 2;
		}
		calculated = true;
	}

	static VertexBuffer vb = VertexBuffer(vertices, 2 * highResCircleVertexCount * sizeof(float));
	return vb;
}

IndexBuffer& Geo::Circle::getHighResCircleIB()
{
	static unsigned int indices[highResCircleVertexCount * 2];

	static bool calculated = false;
	if (!calculated) {

		for (unsigned int i = 0; i < highResCircleVertexCount; i++) {
			indices[i * 2 + 0] = i;
			indices[i * 2 + 1] = i + 1;
		}
		indices[highResCircleVertexCount * 2 - 1] = 0;

		calculated = true;
	}

	static IndexBuffer ib = IndexBuffer(indices, highResCircleVertexCount * 2 * sizeof(unsigned int));
	return ib;
}

VertexArray& Geo::Circle::getHighResCircleVA() {
	static VertexArray va = VertexArray("ff", getHighResCircleVB(), getHighResCircleIB());
	return va;
}



void Geo::Circle::renderFilled(float r, float g, float b, float a)
{
	static std::string vertexShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"\n"
		"out vec2 v_utPos;\n"
		"\n"
		"uniform vec2 u_stretch;\n"
		"uniform vec2 u_translation;\n"
		"uniform float u_aspectRatio;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	v_utPos[0] = position[0];\n"
		"	v_utPos[1] = position[1];\n"
		"	gl_Position = vec4((position[0] / u_aspectRatio) * u_stretch[0] + (u_translation[0] / u_aspectRatio), position[1] * u_stretch[1] + u_translation[1], 0, 1);\n"
		"	gl_Position[2] = 0;\n"
		"};\n";

	static std::string fragmentShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"uniform vec4 u_color;\n"
		"\n"
		"in vec2 v_utPos;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	float a = (v_utPos[0]);\n"
		"	float b = (v_utPos[1]);\n"
		"	float distSq = a * a + b * b;\n"
		"\n"
		"	if (distSq > 0.25)\n"
		"		discard;\n"
		"\n"
		"	color = u_color;\n"
		"}\n";

	static Shader fillShader(vertexShaderString, fragmentShaderString);

	fillShader.setUniform1f("u_aspectRatio", ViewportManager::getCurrentAspectRatio());
	fillShader.setUniform2f("u_stretch", radius * 2, radius * 2);
	fillShader.setUniform2f("u_translation", x, y);
	fillShader.setUniform4f("u_color", r, g, b, a);

	render(fillShader);
}

void Geo::Circle::renderOutline(float r, float g, float b, float a)
{
	static std::string vertexShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"\n"
		"uniform vec2 u_translation;\n"
		"uniform vec2 u_stretch;\n"
		"uniform float u_aspectRatio;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4((position[0] / u_aspectRatio) * u_stretch[0] + (u_translation[0] / u_aspectRatio), position[1] * u_stretch[1] + u_translation[1], 0, 1);\n"
		"	gl_Position[2] = 0;\n"
		"};\n";

	static std::string fragmentShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"uniform vec4 u_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = u_color;\n"
		"};\n";

	static Shader shader(vertexShaderString, fragmentShaderString);

	shader.setUniform1f("u_aspectRatio", ViewportManager::getCurrentAspectRatio());
	shader.setUniform2f("u_stretch", radius * 2, radius * 2);
	shader.setUniform2f("u_translation", x, y);
	shader.setUniform4f("u_color", r, g, b, a);

	if (radius > 0.42)
		render(shader, getHighResCircleVB(), getHighResCircleIB(), getHighResCircleVA(), GL_LINES);

	else if (radius > 0.05f)
		render(shader, getMedResCircleVB(), getMedResCircleIB(), getMedResCircleVA(), GL_LINES);
	
	else 
		render(shader, getLowResCircleVB(), getLowResCircleIB(), getLowResCircleVA(), GL_LINES);
}

void Geo::Circle::fillCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	Circle& circle = getStencyl();
	circle.x = x;
	circle.y = y;
	circle.radius = radius;
	circle.renderFilled(r, g, b, a);
}

void Geo::Circle::outlineCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	Circle& circle = getStencyl();
	circle.x = x;
	circle.y = y;
	circle.radius = radius;
	circle.renderOutline(r, g, b, a);
}

bool Geo::Circle::getIntersection(const Geo::Circle & circle, std::vector<std::array<float, 2>>* poi)
{
	float dSquared = pow(circle.x - x, 2) + pow(circle.y - y, 2);

	bool intersecting = dSquared <= pow(radius + circle.radius, 2);
	if (!poi || !intersecting) {
		return intersecting;
	}
	else {
		float d = sqrt(dSquared);
		float b = (-pow(circle.radius, 2) + pow(radius, 2) - dSquared) / (-2 * d);
		float c = sqrt(pow(circle.radius, 2) - pow(b, 2));

		float EBunit[2] = { ((x - circle.x) / d), ((y - circle.y) / d) };
		float EB[2] = { EBunit[0] * b, EBunit[1] * b };
		float B[2] = { circle.x + EB[0], circle.y + EB[1] };

		float BD[2] = { -EBunit[1] * c, EBunit[0] * c };
		poi->push_back({ B[0] + BD[0], B[1] + BD[1] });
		poi->push_back({ B[0] - BD[0], B[1] - BD[1] });

		return true;
	}
	return false;
}

bool Geo::Circle::getIntersection(const Geo::Rectangle & rect, std::vector<std::array<float, 2>>* poi)
{
	return getRectCircleIntersection(rect, *this, poi);
}

bool Geo::Circle::getIntersection(const Geo::LineSeg & line, std::vector<std::array<float, 2>>* poi)
{
	return getLineCircleIntersection(line, *this, poi);
}

bool Geo::Circle::containsPoint(float _x, float _y) const
{
	return pow(_x - x, 2) + pow(_y - y, 2) <= radius * radius;
}

Geo::LineSeg::LineSeg(float _x1, float _y1, float _x2, float _y2)
{
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
}

Geo::LineSeg::LineSeg()
{
}

VertexBuffer & Geo::LineSeg::getVB()
{
	static float vertices[4] = {
		0.0f, 0.0f,
		1.0f, 1.0f
	};

	static VertexBuffer vb = VertexBuffer(vertices, 4 * sizeof(float));
	return vb;
}

IndexBuffer & Geo::LineSeg::getIB()
{
	static unsigned int indices[2] = {
		0, 1
	};

	static IndexBuffer ib = IndexBuffer(indices, 2 * sizeof(unsigned int));
	return ib;
}

VertexArray & Geo::LineSeg::getVA()
{
	static VertexArray va = VertexArray("ff", getVB(), getIB());
	return va;
}

void Geo::LineSeg::render(float r, float g, float b, float a)
{
	// circle wireframe shader
	static std::string vertexShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"\n"
		"uniform vec2 u_translation;\n"
		"uniform vec2 u_stretch;\n"
		"uniform float u_aspectRatio;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4((position[0] / u_aspectRatio) * u_stretch[0] + (u_translation[0] / u_aspectRatio), position[1] * u_stretch[1] + u_translation[1], 0, 1);\n"
		"	gl_Position[2] = 0;\n"
		"};\n";

	static std::string fragmentShaderString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"uniform vec3 u_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(u_color, 1);\n"
		"};\n";

	static Shader shader(vertexShaderString, fragmentShaderString);

	shader.setUniform1f("u_aspectRatio", ViewportManager::getCurrentAspectRatio());
	shader.setUniform3f("u_color", r, g, b);
	shader.setUniform2f("u_translation", x1, y1);
	shader.setUniform2f("u_stretch", x2 - x1, y2 - y1);

	Geo::GeoObject::render(shader, getVB(), getIB(), getVA(), GL_LINES);

}

void Geo::LineSeg::renderLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a)
{
	static LineSeg line;
	line.x1 = x1;
	line.x2 = x2;
	line.y1 = y1;
	line.y2 = y2;
	line.render(r, g, b, a);
}

bool Geo::LineSeg::getIntersection(const Geo::Circle & circle, std::vector<std::array<float, 2>>* poi)
{
	return getLineCircleIntersection(*this, circle, poi);
}

bool Geo::LineSeg::getIntersection(const Geo::LineSeg & line, std::vector<std::array<float, 2>>* poi)
{
	return getLineLineIntersection(*this, line, poi);
}

bool Geo::LineSeg::getIntersection(const Geo::Rectangle & rect, std::vector<std::array<float, 2>>* poi)
{
	return getLineRectIntersection(rect, *this, poi);
}

float Geo::LineSeg::getLength()
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
