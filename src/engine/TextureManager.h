#pragma once

#include "engine/Texture.h"
#include <map>

class TextureManager {
public:
	// use this function to globally access images, if the texture is not already loaded it will be loaded, otherwise it returns you the existing one
	static Texture* getTexture(const std::string& filepath);

	// don't call this function
	static void cleanup();

private:
	static std::map<std::string, Texture*> textures;

};