#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "engine/Window.h"
#include "engine/Texture.h"
#include "engine/TexturedQuad.h"
#include "engine/Geometry.h"
#include "yse.hpp"
#include "engine/BatchQuads.h"
#include "engine/BatchLines.h"
#include "engine/SoundBite.h"
#include "engine/LoopingSound.h"
#include "engine/ViewportManager.h"
#include "engine/BitmapFont.h"
#include "engine/BitmapText.h"
#include "engine/TextureManager.h"
#include "engine/ExistentialDictionary.h"

#include "game/CrosswordBoard.h"

namespace GameLogicInterface {
	void init();
	void update(float deltaTime);
	void cleanup();

	void mouseMoveCallback(double xPos, double yPos);
	void mouseButtonCallback(int button, int action, int mods);
	void keyCallback(int key, int scancode, int action, int mods);
	void characterCallback(unsigned int codepoint);

	void loadNewBoard();

};