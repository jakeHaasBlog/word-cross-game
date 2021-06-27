#pragma once

#include <array>
#include <vector>
#include <string>

#include "engine/ExistentialDictionary.h"

class CrosswordTile {
public:
	char correctLetter = '\0'; // if correct letter is '\0' then the tile is not able to be set
	char currentLetter;

	bool locked = false;
};

class CrosswordBoard {
public:
	CrosswordBoard(const std::string& wordList, int softResolution = 1000, int deepResolution = 10000);

	int getTileAt(float x, float y);
	int getLinearIndex(int x, int y);

	int getTileToTheRight(int index);
	int getTileToTheLeft(int index);
	int getTileAbove(int index);
	int getTileBelow(int index);

	CrosswordTile& getTile(int index);

	void reset(int tilesWide, int tilesTall);
	void render();
	void renderWithSolution();

	void initPartialReset();
	void partialReset(int softItterations);
	void finalizePartialReset();

	float getTileRenderX(int index);
	float getTileRenderY(int index);

	void resetCrossedWordsList(); // call this any time the board is changed

	bool checkWin();

private:
	bool wordFits(int x, int y, bool isHorizontal, const std::string& word);
	bool checkColumnPotential(int x, int y, char newChar);
	bool checkRowPotential(int x, int y, char newChar);
	void resetWordsList();
	void renderWordsList();
	void fillRandomWord();
	void renderRowAndColumnSizes();

private:
	static ExistentialDictionary dictionary;

	std::vector<CrosswordTile> tiles;
	std::vector<std::string> wordList;
	std::vector<int> crossedOffList;

public:
	int tilesWide = 20;
	int tilesTall = 20;

	float tileWidth = 0.05;
	float tileHeight = 0.05;
	float tilesPosX = -0.5f;
	float tilesPosY = -0.5f;

	int softResolution = 1000; // the number of position for words
	int deepResolution = 10000; // how many words are tested in each chosen position
};