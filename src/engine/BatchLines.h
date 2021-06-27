#pragma once

#include <vector>
#include <array>

#include "engine/OpenglBufferObjects.h"
#include "engine/Shader.h"

#include <memory>

struct BatchLine {
	float x1, y1;
	float r1, g1, b1, a1;

	float x2, y2;
	float r2, g2, b2, a2;
};

// Used to draw a large number of lines simultaniously, each line has a color and can be gradiented from beginning to end
class BatchLines {
public:
	BatchLines();
	BatchLines(int numOfLines);

	~BatchLines();

	void addLine(float x, float y, float x2, float y2, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
	void removeLine(int i);
	BatchLine& getLine(int i); // use this to modify quads, NEVER create a pointer to a line

	void setLine(int i, float x, float y, float x2, float y2, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
	void setLinePos(int i, float x, float y, float width, float height);
	void setLineColor(int i, float r, float g, float b, float a = 1.0f);

	void setBeginLineColor(int i, float r, float g, float b, float a = 1.0f);
	void setEndLineColor(int i, float r, float g, float b, float a = 1.0f);

	void useShader(Shader& shader);

	void renderAll(float scale = 1.0f, std::array<float, 2> translation = { 0.0f, 0.0f });

	int size();

private:
	std::vector<BatchLine> vertices;
	IndexBuffer* ib;
	VertexBuffer* vb;
	VertexArray* va;
	void updateBuffers();

	static Shader* defaultShader;
	Shader* lineShader;

	bool buffersReady = false;
	bool buffersInitialized = false;
};