#include "engine/BatchLines.h"

#include "engine/Window.h"

#include "engine/ViewportManager.h"

Shader* BatchLines::defaultShader = nullptr;

BatchLines::BatchLines()
{
}

BatchLines::BatchLines(int numOfLines) :
	vertices(numOfLines)
{
}

BatchLines::~BatchLines()
{
	if (vb)
		delete vb;

	if (ib)
		delete ib;

	if (va)
		delete va;
}

void BatchLines::addLine(float x, float y, float x2, float y2, float r, float g, float b, float a)
{

	// bl, tl, tr, br
	vertices.push_back({});

	(*(vertices.end() - 1)).x1 = x;
	(*(vertices.end() - 1)).y1 = y;
	(*(vertices.end() - 1)).x2 = x2;
	(*(vertices.end() - 1)).y2 = y2;

	(*(vertices.end() - 1)).r1 = r;
	(*(vertices.end() - 1)).g1 = g;
	(*(vertices.end() - 1)).b1 = b;
	(*(vertices.end() - 1)).a1 = a;

	(*(vertices.end() - 1)).r2 = r;
	(*(vertices.end() - 1)).g2 = g;
	(*(vertices.end() - 1)).b2 = b;
	(*(vertices.end() - 1)).a2 = a;

	buffersReady = false;
}

void BatchLines::removeLine(int i)
{
	vertices.erase(vertices.begin() + i);
	buffersReady = false;
}

BatchLine & BatchLines::getLine(int i)
{
	buffersReady = false;
	return vertices[i];
}

void BatchLines::setLine(int i, float x, float y, float x2, float y2, float r, float g, float b, float a)
{
	vertices[i].x1 = x;
	vertices[i].y1 = y;
	vertices[i].x2 = x2;
	vertices[i].y2 = y2;

	vertices[i].r1 = r;
	vertices[i].g1 = g;
	vertices[i].b1 = b;
	vertices[i].a1 = a;

	vertices[i].r2 = r;
	vertices[i].g2 = g;
	vertices[i].b2 = b;
	vertices[i].a2 = a;

	buffersReady = false;
}

void BatchLines::setLinePos(int i, float x, float y, float x2, float y2)
{
	vertices[i].x1 = x;
	vertices[i].y1 = y;
	vertices[i].x2 = x2;
	vertices[i].y2 = y2;

	buffersReady = false;
}

void BatchLines::setLineColor(int i, float r, float g, float b, float a)
{
	vertices[i].r1 = r;
	vertices[i].g1 = g;
	vertices[i].b1 = b;
	vertices[i].a1 = a;

	vertices[i].r2 = r;
	vertices[i].g2 = g;
	vertices[i].b2 = b;
	vertices[i].a2 = a;

	buffersReady = false;
}

void BatchLines::setBeginLineColor(int i, float r, float g, float b, float a)
{
	vertices[i].r1 = r;
	vertices[i].g1 = g;
	vertices[i].b1 = b;
	vertices[i].a1 = a;

	buffersReady = false;
}

void BatchLines::setEndLineColor(int i, float r, float g, float b, float a)
{
	vertices[i].r2 = r;
	vertices[i].g2 = g;
	vertices[i].b2 = b;
	vertices[i].a2 = a;

	buffersReady = false;
}


void BatchLines::useShader(Shader & shader)
{
	lineShader = &shader;
}

void BatchLines::renderAll(float scale, std::array<float, 2> translation)
{
	if (!buffersReady) {
		updateBuffers();
		buffersReady = true;
	}

	lineShader->setUniform1f("u_aspectRatio", ViewportManager::getCurrentAspectRatio());
	lineShader->setUniform1f("u_stretch", scale);
	lineShader->setUniform2f("u_translation", translation[0], translation[1]);
	lineShader->bind();
	va->bind();
	glDrawElements(GL_LINES, ib->getCount(), GL_UNSIGNED_INT, nullptr);
	lineShader->unbind();
	va->unbind();
}

int BatchLines::size()
{
	return vertices.size();
}

void BatchLines::updateBuffers()
{
	if (!defaultShader) {
		std::string vertexShader =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec2 position;\n"
			"layout(location = 1) in vec4 color;\n"
			"\n"
			"out vec4 v_color;\n"
			"\n"
			"uniform float u_aspectRatio;\n"
			"uniform float u_stretch;\n"
			"uniform vec2 u_translation;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vec4((position[0] * u_stretch + u_translation[0]) / u_aspectRatio, position[1]  * u_stretch + u_translation[1], 0, 1);\n"
			"	gl_Position[2] = 0;\n"
			"   v_color = color;\n"
			"};\n";

		std::string fragmentShader =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 color;\n"
			"\n"
			"in vec4 v_color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	color = v_color;\n"
			"};\n";

		defaultShader = new Shader(vertexShader, fragmentShader);
	}

	if (!lineShader) {
		lineShader = defaultShader;
	}

	std::vector<unsigned int> indices(vertices.size() * 2);
	for (int i = 0; i < vertices.size(); i++) {
		indices[i * 2 + 0] = i * 2 + 0;
		indices[i * 2 + 1] = i * 2 + 1;
	}

	if (buffersInitialized) {
		vb->bufferData(&vertices[0], vertices.size() * sizeof(BatchLine));
		ib->bufferData(&indices[0], indices.size() * sizeof(unsigned int));
	}
	else {
		ib = new IndexBuffer();
		ib->bufferData(&indices[0], indices.size() * sizeof(unsigned int));

		vb = new VertexBuffer();
		vb->bufferData(&vertices[0], vertices.size() * sizeof(BatchLine));

		va = new VertexArray();
		va->setAttributes("ff ffff", *vb, *ib);

		buffersInitialized = true;
	}
}
