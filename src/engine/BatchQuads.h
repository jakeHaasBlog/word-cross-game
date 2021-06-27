#pragma once

#include <vector>
#include <array>

#include <engine/Texture.h>

#include "engine/OpenglBufferObjects.h"
#include "engine/Shader.h"

#include <memory>

// This is the internal storage for each quad, ignore this unless you want to produce bizarre efect
struct BatchQuad {
	float x1, y1;
	float texX1, texY1;
	float r1, g1, b1, a1;
	float texture1;

	float x2, y2;
	float texX2, texY2;
	float r2, g2, b2, a2;
	float texture2;

	float x3, y3;
	float texX3, texY3;
	float r3, g3, b3, a3;
	float texture3;

	float x4, y4;
	float texX4, texY4;
	float r4, g4, b4, a4;
	float texture4;
};

class BatchQuads {
public:
	BatchQuads();
	BatchQuads(int numOfQuads);

	~BatchQuads();

	void addQuad(float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, int texture = -1);
	void removeQuad(int i);
	BatchQuad& getQuad(int i); // use this to modify quads to produce bizarre effects, NEVER create a pointer to the returned BatchQuad

	// removes all quads from render queue
	void clear();

	void setQuad(int i, float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
	void setQuad(int i, float x, float y, float width, float height, int texture);
	void setQuadPos(int i, float x, float y, float width, float height);
	void setQuadColor(int i, float r, float g, float b, float a = 1.0f);

	// these functions can be used to make gradients from each corner of the quad
	void setTopLeftQuadColor(int i, float r, float g, float b, float a = 1.0f);
	void setTopRightQuadColor(int i, float r, float g, float b, float a = 1.0f);
	void setBottomLeftQuadColor(int i, float r, float g, float b, float a = 1.0f);
	void setBottomRightQuadColor(int i, float r, float g, float b, float a = 1.0f);

	// sets the texture index of the target quad. Indexes into boundTextures[15] which are set using setTextureSlot() 
	void setQuadTexture(int i, int textureSlot);

	// when using these functions a texture must be set for that quad
	void setQuadTextureSampleBounds(int i, int pixX, int pixY, int pixWidth, int pixHeight);
	void setQuadTextureSampleBounds(int i, float screenCoordX, float screenCoordY, float width, float height);

	// there are only 15 slots for textures in the GPU, so set the textures here then each quad has an index to the right one, use setQuadTexture() to set the quad indexes
	void setTextureSlot(int slot, Texture* tex); 

	// there is a default shader but you can set a custom one as long as it sets all required uniforms and has the same vertex attributes, see renderAll() to see required uniforms
	void useShader(Shader& shader); 

	void renderAll(float scale = 1.0f, std::array<float, 2> translation = { 0.0f, 0.0f });

	// returns the total number of quads
	int size();

private:
	std::vector<BatchQuad> vertices;
	IndexBuffer* ib;
	VertexBuffer* vb;
	VertexArray* va;
	void updateBuffers();
	void bindTextures();

	static Shader* defaultShader;
	Shader* quadShader;

	std::array<Texture*, 15> boundTextures;

	bool buffersReady = false;
	bool buffersInitialized = false;
};