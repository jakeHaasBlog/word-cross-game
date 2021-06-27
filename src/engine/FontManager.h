#pragma once

#include "engine/BitmapFont.h"
#include <map>
#include <string>

/* This class manages global instances of BitmapFont
	- keep track of your fonts by their string filepaths then access them here
	- if a font is not loaded when you try to access it, then the font will be loaded
*/
class FontManager {
public:
	// DO NOT CALL THIS FUNCTION
	static void init();

	// returns the default font, poorRichard <- its apparently a default Microsoft font
	static BitmapFont* getDefaultFont();

	// the font will be loaded if it is not already
	static BitmapFont* getFont(const std::string& filepath);

	// DO NOT CALL THIS FUNCTION
	static void cleanup();

private:
	static std::map<std::string, BitmapFont*> fonts;

};