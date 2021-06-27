#pragma once

#include <string>
#include <array>
#include <vector>
#include <unordered_map>

#include "engine/Texture.h"

struct BitmapFontCharacterInfo {
	float xSample;
	float ySample;
	float widthSample;
	float heightSample;

	float xOffset;
	float yOffset;
	float xAdvance;
};

/* Every font should only be instanciated once, then used by multiple BitmapText objects to render text in that font so use FontManager to keep track of global instances of fonts 
	- There is a default BitmapFont located in assets/fonts/defaultFont
	- To define your own fonts look at the default font for reference
	- I used a program called bmfont to generate the bitmap font from a true type font
	
	- If you use bmfont there are some settings you must adjust:
	--- options->Export Options->Force offsets to zero       <--- must be checked
	--- options->Export Options->Equalize the cell heights   <--- must be checked
	--- options->Export Options->Texttures                   <--- must be set to png

	- alternatively you can just copy paste the default font and photoshop texture.png
*/
class BitmapFont {
public:
	// filepath is to a folder containing required files, look at existing fonts for reference
	BitmapFont(const std::string& filepath);

	const std::string& getFilepath();

	// the texture containing all letters and numbers
	Texture& getTexture();

	// gets the sample area of the texture bounding the specified character if it exists
	std::array<float, 4> getSampleArea(char c);

	// returns BitmapFontCharacterInfo containing information about how to render the given character
	const BitmapFontCharacterInfo& getCharacterInfo(char c);

private:
	const std::string filepath;

	std::array<BitmapFontCharacterInfo, 256> loadedSampleCoords;
	Texture loadedTextures;

	bool hasInitialized = false;
	void tryInitialize();
};


