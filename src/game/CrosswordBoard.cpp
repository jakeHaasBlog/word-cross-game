#include "CrosswordBoard.h"

#include "engine/Geometry.h"
#include "engine/BitmapText.h"

#include "engine/ViewportManager.h"
#include <stdio.h>
#include <string.h>

#include <algorithm>

ExistentialDictionary CrosswordBoard::dictionary = ExistentialDictionary("assets/myWords.txt");

CrosswordBoard::CrosswordBoard(const std::string& wordList, int softResolution, int deepResolution) :
	softResolution(softResolution),
	deepResolution(deepResolution)
{
}

int CrosswordBoard::getTileAt(float x, float y)
{

	for (int i = 0; i < tiles.size(); i++) {
		int tx = i % tilesWide;
		int ty = i / tilesWide;

		float renderX = tilesPosX + tx * tileWidth;
		float renderY = tilesPosY + ty * tileHeight;
		float renderWidth = tileWidth;
		float renderHeight = tileHeight;

		Geo::Rectangle rect;
		rect.x = renderX;
		rect.y = renderY + renderHeight;
		rect.width = renderWidth;
		rect.height = renderHeight;

		if (rect.containsPoint(x, y)) {
			return i;
		}

	}

	return -1;

}

int CrosswordBoard::getLinearIndex(int x, int y)
{
	if (x < 0 || x >= tilesWide) return -1;
	if (y < 0 || y >= tilesTall) return -1;

	return x + y * tilesWide;
}

int CrosswordBoard::getTileToTheRight(int index)
{
	int x = index % tilesWide;
	int y = index / tilesWide;
	x++;

	return getLinearIndex(x, y);
}

int CrosswordBoard::getTileToTheLeft(int index)
{
	int x = index % tilesWide;
	int y = index / tilesWide;
	x--;

	return getLinearIndex(x, y);
}

int CrosswordBoard::getTileAbove(int index)
{
	int x = index % tilesWide;
	int y = index / tilesWide;
	y++;

	return getLinearIndex(x, y);
}

int CrosswordBoard::getTileBelow(int index)
{
	int x = index % tilesWide;
	int y = index / tilesWide;
	y--;

	return getLinearIndex(x, y);
}

CrosswordTile& CrosswordBoard::getTile(int index)
{
	return tiles[index];
}

void CrosswordBoard::reset(int tilesWide, int tilesTall)
{
	tiles.clear();
	tiles.reserve(tilesWide * tilesTall);
	
	for (int i = 0; i < tilesWide * tilesTall; i++) {
		tiles.push_back(CrosswordTile());
		tiles[tiles.size() - 1].correctLetter = '\0';
	}

	this->tilesWide = tilesWide;
	this->tilesTall = tilesTall;

	tilesPosX = -tilesWide / 2 * tileWidth;
	tilesPosY = -tilesTall / 2 * tileHeight;


	std::string word = "";
	int failiures = 0;
	bool firstWord = true;
	for (int i = 0; i < softResolution ; i++) {

		int targetWidth = (rand() % 10) + 3;
		bool isHorizontal = rand() % 100 > 50;

		int x, y;
		if (firstWord) {
			targetWidth = 10;
			if (tilesWide < targetWidth) targetWidth = tilesWide;
			if (tilesTall < targetWidth) targetWidth = tilesTall;

			x = 0;
			y = tilesTall - 1;
		}
		else {
			while (true) {
				x = rand() % tilesWide;
				y = rand() % tilesTall;

				targetWidth = (rand() % 10) + 3;
				isHorizontal = rand() % 100 > 50;

				bool willHitWord = false;
				for (int k = 0; k < targetWidth; k++) {
					int x1 = x;
					int y1 = y;
					if (isHorizontal) x1 = x + k;
					else y1 = y - k;

					if (getLinearIndex(x1, y1) != -1) {
						if (tiles[getLinearIndex(x1, y1)].correctLetter != '\0') {
							willHitWord = true;
						}
					}

				}

				if (willHitWord) break;
			}
		}
		firstWord = false;

		failiures = 0;
		while (failiures < deepResolution) {

			word = dictionary.getRandomWord(targetWidth);
			while (word.length() != targetWidth) {
				word = dictionary.getRandomWord(targetWidth);
			}

			if (!wordFits(x, y, isHorizontal, word)) {
				failiures++;
			}
			else {
				// add the word
				for (int i = 0; i < word.size(); i++) {

					if (isHorizontal) {
						tiles[getLinearIndex(x + i, y)].correctLetter = word[i];
					}
					else {
						tiles[getLinearIndex(x, y - i)].correctLetter = word[i];
					}
				}

				resetWordsList();

				break;
			}
		}

	}

	resetWordsList();
	fillRandomWord();
	resetCrossedWordsList();
}

void CrosswordBoard::render()
{

	for (int i = 0; i < tiles.size(); i++) {
		int x = i % tilesWide;
		int y = i / tilesWide;

		if (tiles[i].correctLetter == '\0') {
			Geo::Rectangle::fillRect(tilesPosX + x * tileWidth, tilesPosY + y * tileHeight, tileWidth, tileHeight, 0, 0, 0, 1);
		}

	}

	for (int i = 0; i < tiles.size(); i++) {
		int x = i % tilesWide;
		int y = i / tilesWide;

		if (tiles[i].correctLetter != '\0') {
			Geo::Rectangle::fillRect(tilesPosX + x * tileWidth, tilesPosY + y * tileHeight, tileWidth, tileHeight, 0.5f, 0.5f, 0.5f, 1.0f);
			Geo::Rectangle::outlineRect(tilesPosX + x * tileWidth, tilesPosY + y * tileHeight, tileWidth, tileHeight, 1, 1, 1, 1);

			static BitmapText text;
			char letter[2];
			sprintf_s(letter, "%c", tiles[i].currentLetter);
			text.setText(letter);
			text.setFont(FontManager::getFont("assets/fonts/arial"));
			text.setColor(0, 1, 0, 1); // text color
			text.setCharHeight(tileHeight * 0.9f);
			text.setPosition({ tilesPosX + x * tileWidth + tileWidth / 3, tilesPosY + y * tileHeight + tileHeight * 0.05f });
			text.render();
		}

	}

	renderWordsList();
	renderRowAndColumnSizes();
}

void CrosswordBoard::renderWithSolution()
{
	for (int i = 0; i < tiles.size(); i++) {
		int x = i % tilesWide;
		int y = i / tilesWide;

		if (tiles[i].correctLetter == '\0') {
			Geo::Rectangle::fillRect(tilesPosX + x * tileWidth, tilesPosY + y * tileHeight, tileWidth, tileHeight, 0, 0, 0, 1);
		}

	}

	for (int i = 0; i < tiles.size(); i++) {
		int x = i % tilesWide;
		int y = i / tilesWide;

		if (tiles[i].correctLetter != '\0') {
			Geo::Rectangle::fillRect(tilesPosX + x * tileWidth, tilesPosY + y * tileHeight, tileWidth, tileHeight, 0.5f, 0.5f, 0.5f, 1.0f);
			Geo::Rectangle::outlineRect(tilesPosX + x * tileWidth, tilesPosY + y * tileHeight, tileWidth, tileHeight, 1, 1, 1, 1);

			static BitmapText text;
			char letter[2];
			sprintf_s(letter, "%c", tiles[i].correctLetter);
			text.setText(letter);
			text.setFont(FontManager::getFont("assets/fonts/arial"));
			text.setColor(0, 1, 0, 1); // text color
			text.setCharHeight(tileHeight * 0.9f);
			text.setPosition({ tilesPosX + x * tileWidth + tileWidth / 3, tilesPosY + y * tileHeight + tileHeight * 0.05f });
			text.render();
		}

	}

	renderWordsList();
	renderRowAndColumnSizes();
}

void CrosswordBoard::initPartialReset()
{
	tiles.clear();
	tiles.reserve(tilesWide * tilesTall);

	for (int i = 0; i < tilesWide * tilesTall; i++) {
		tiles.push_back(CrosswordTile());
		tiles[tiles.size() - 1].correctLetter = '\0';
	}

	this->tilesWide = tilesWide;
	this->tilesTall = tilesTall;

	tilesPosX = -tilesWide / 2 * tileWidth;
	tilesPosY = -tilesTall / 2 * tileHeight;
}

void CrosswordBoard::partialReset(int softItterations)
{
	std::string word = "";
	int failiures = 0;
	bool firstWord = true;
	for (int i = 0; i < softItterations; i++) {

		int targetWidth = (rand() % 10) + 3;
		bool isHorizontal = rand() % 100 > 50;

		int x, y;
		if (firstWord) {
			targetWidth = 10;
			if (tilesWide < targetWidth) targetWidth = tilesWide;
			if (tilesTall < targetWidth) targetWidth = tilesTall;

			x = 0;
			y = tilesTall - 1;
		}
		else {
			while (true) {
				x = rand() % tilesWide;
				y = rand() % tilesTall;

				targetWidth = (rand() % 10) + 3;
				isHorizontal = rand() % 100 > 50;

				bool willHitWord = false;
				for (int k = 0; k < targetWidth; k++) {
					int x1 = x;
					int y1 = y;
					if (isHorizontal) x1 = x + k;
					else y1 = y - k;

					if (getLinearIndex(x1, y1) != -1) {
						if (tiles[getLinearIndex(x1, y1)].correctLetter != '\0') {
							willHitWord = true;
						}
					}

				}

				if (willHitWord) break;
			}
		}
		firstWord = false;

		failiures = 0;
		while (failiures < deepResolution) {

			word = dictionary.getRandomWord(targetWidth);
			while (word.length() != targetWidth) {
				word = dictionary.getRandomWord(targetWidth);
			}

			if (!wordFits(x, y, isHorizontal, word)) {
				failiures++;
			}
			else {
				// add the word
				for (int i = 0; i < word.size(); i++) {

					if (isHorizontal) {
						tiles[getLinearIndex(x + i, y)].correctLetter = word[i];
					}
					else {
						tiles[getLinearIndex(x, y - i)].correctLetter = word[i];
					}
				}

				resetWordsList();

				break;
			}
		}
	}
}

void CrosswordBoard::finalizePartialReset()
{
	resetWordsList();
	fillRandomWord();
	resetCrossedWordsList();
}

float CrosswordBoard::getTileRenderX(int i)
{
	int tx = i % tilesWide;
	return tilesPosX + tx * tileWidth;
}

float CrosswordBoard::getTileRenderY(int i)
{
	int ty = i / tilesWide;
	return tilesPosY + ty * tileHeight;
}

bool CrosswordBoard::wordFits(int x, int y, bool isHorizontal, const std::string& word)
{

	// word cannot be a duplicate
	for (std::string& w : wordList) {
		if (strcmp(word.c_str(), w.c_str()) == 0) {
			return false;
		}
	}

	// word must not overwrite any characters or go off the board
	for (int i = 0; i < word.length(); i++) {
		int dx = x;
		int dy = y;
		if (isHorizontal) dx += i;
		else dy -= i;

		int index = getLinearIndex(dx, dy);
		if (index == -1) return false;
		if (tiles[index].correctLetter != word[i] && tiles[index].correctLetter != '\0') return false;
	}

	// word must not end or begin next to another letter
	if (isHorizontal) {
		int index = getLinearIndex(x - 1, y);
		if (index != -1) {
			if (tiles[index].correctLetter != '\0') return false;
		}

		index = getLinearIndex(x + word.length(), y);
		if (index != -1) {
			if (tiles[index].correctLetter != '\0') return false;
		}
	} else {
		int index = getLinearIndex(x, y + 1);
		if (index != -1) {
			if (tiles[index].correctLetter != '\0') return false;
		}

		index = getLinearIndex(x, y - word.length());
		if (index != -1) {
			if (tiles[index].correctLetter != '\0') return false;
		}
	}

	// if word is written along another word then newly formed words must be valid
	for (int i = 0; i < word.length(); i++) {
		int dx = x;
		int dy = y;
		if (isHorizontal) dx += i;
		else dy -= i;

		int index = getLinearIndex(dx, dy);

		if (isHorizontal) {
			if (!checkColumnPotential(dx, dy, word[i])) return false;
		}
		else {
			if (!checkRowPotential(dx, dy, word[i])) return false;
		}
	}


	return true;
}

bool CrosswordBoard::checkColumnPotential(int x, int y, char newChar)
{
	int topY = y;
	while (true) {
		topY++;

		int index = getLinearIndex(x, topY);
		if (index != -1) {
			if (tiles[index].correctLetter != '\0')
				topY;
			else {
				topY--;
				break;
			}
		}
		else {
			topY--;
			break;
		}

	}

	int bottomY = y;
	while (true) {
		bottomY--;

		int index = getLinearIndex(x, bottomY);
		if (index != -1) {
			if (tiles[index].correctLetter != '\0')
				bottomY;
			else {
				bottomY++;
				break;
			}
		}
		else {
			bottomY++;
			break;
		}

	}


	std::string topToBottom = "";
	for (int y1 = topY; y1 >= bottomY; y1--) {
		
		if (y1 != y) topToBottom += tiles[getLinearIndex(x, y1)].correctLetter;
		else topToBottom += newChar;

	}

	if (topToBottom.length() < 2) return true;


	// must be a real word
	return dictionary.wordExists(topToBottom);
}


bool CrosswordBoard::checkRowPotential(int x, int y, char newChar)
{
	int leftX = x;
	while (true) {
		leftX--;

		int index = getLinearIndex(leftX, y);
		if (index != -1) {
			if (tiles[index].correctLetter != '\0')
				leftX;
			else {
				leftX++;
				break;
			}
		}
		else {
			leftX++;
			break;
		}

	}

	int rightX = x;
	while (true) {
		rightX++;

		int index = getLinearIndex(rightX, y);
		if (index != -1) {
			if (tiles[index].correctLetter != '\0')
				rightX;
			else {
				rightX--;
				break;
			}
		}
		else {
			rightX--;
			break;
		}

	}


	std::string leftToRight = "";
	for (int x1 = leftX; x1 <= rightX; x1++) {

		if (x1 != x) leftToRight += tiles[getLinearIndex(x1, y)].correctLetter;
		else leftToRight += newChar;

	}

	if (leftToRight.length() < 2) return true;

	// must be a real word
	return dictionary.wordExists(leftToRight);
}

void CrosswordBoard::resetWordsList()
{
	wordList.clear();

	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i].correctLetter != '\0') {

			int aboveTile = getTileAbove(i);
			int bolowTile = getTileBelow(i);
			if (bolowTile != -1) {
				if ((aboveTile == -1 || tiles[aboveTile].correctLetter == '\0') && tiles[bolowTile].correctLetter != '\0') {
					// word goes down
					int t = i;
					std::string word = "";
					while (tiles[t].correctLetter != '\0') {
						word += tiles[t].correctLetter;
						t = getTileBelow(t);
						if (t == -1) break;
					}
					wordList.push_back(word);

				}
			}


			int leftTile = getTileToTheLeft(i);
			int rightTile = getTileToTheRight(i);
			if (rightTile != -1) {
				if ((leftTile == -1 || tiles[leftTile].correctLetter == '\0') && tiles[rightTile].correctLetter != '\0') {
					// word goes right
					int t = i;
					std::string word = "";
					while (tiles[t].correctLetter != '\0') {
						word += tiles[t].correctLetter;
						t = getTileToTheRight(t);
						if (t == -1) break;
					}
					wordList.push_back(word);

				}
			}


		}
	}

	std::sort(wordList.begin(), wordList.end(), [](const std::string& a, const std::string& b) { 
		if (a.length() > b.length()) {
			return true;
		}
		else {

			if (a.length() == b.length()) {
				return a < b;
			}

			return false;
		}
	});
	
}

void CrosswordBoard::resetCrossedWordsList()
{
	std::vector<std::string> words;

	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i].correctLetter != '\0') {

			int aboveTile = getTileAbove(i);
			int bolowTile = getTileBelow(i);
			if (bolowTile != -1) {
				if ((aboveTile == -1 || tiles[aboveTile].correctLetter == '\0') && tiles[bolowTile].correctLetter != '\0') {
					// word goes down
					int t = i;
					std::string word = "";
					while (tiles[t].correctLetter != '\0') {
						word += tiles[t].currentLetter;
						t = getTileBelow(t);
						if (t == -1) break;
					}
					words.push_back(word);

				}
			}


			int leftTile = getTileToTheLeft(i);
			int rightTile = getTileToTheRight(i);
			if (rightTile != -1) {
				if ((leftTile == -1 || tiles[leftTile].correctLetter == '\0') && tiles[rightTile].correctLetter != '\0') {
					// word goes right
					int t = i;
					std::string word = "";
					while (tiles[t].correctLetter != '\0') {
						word += tiles[t].currentLetter;
						t = getTileToTheRight(t);
						if (t == -1) break;
					}
					words.push_back(word);

				}
			}


		}
	}

	crossedOffList.clear();
	for (int i = 0; i < wordList.size(); i++) {
		for (int j = 0; j < words.size(); j++) {
			if (strcmp(words[j].c_str(), wordList[i].c_str()) == 0) {
				crossedOffList.push_back(i);
				words.erase(words.begin() + j);
				continue;
			}
		}
	}

}

bool CrosswordBoard::checkWin()
{

	resetCrossedWordsList();
	return wordList.size() == crossedOffList.size();

}

void CrosswordBoard::renderWordsList()
{
	float x = ViewportManager::getLeftViewportBound() + 0.1f;
	float yOffset = 0.06f;
	float y = 0.9f + 2 * yOffset;
	float xOffset = 0.45f;
	float minY = -0.9f;
	int currentSize = 0;
	for (int i = 0; i < wordList.size(); i++) {

		bool newCategory = false;
		if (wordList[i].size() != currentSize) {
			newCategory = true;
		}
		currentSize = wordList[i].size();

		if (newCategory) {
			y -= yOffset * 2;
			if (y - yOffset < minY) {
				x += xOffset;
				y = 0.9f;
			}

			char newCategory[60];
			sprintf_s(newCategory, "------- %d -------", currentSize);

			static BitmapText text;
			text.setFont(FontManager::getFont("assets/fonts/arial"));
			text.setText(newCategory);
			text.setColor(0, 0, 0, 1);
			text.setCharHeight(tileHeight * 1.2f);
			text.setPosition({ x, y });
			text.render();
		}

		y -= yOffset;
		if (y < minY) {
			y = 0.9f;
			x += xOffset;
		}

		static BitmapText text;
		text.setFont(FontManager::getFont("assets/fonts/arial"));
		text.setText(wordList[i]);
		text.setColor(0, 0, 0, 1);
		text.setCharHeight(tileHeight * 1.2f);
		text.setPosition({ x, y });
		text.render();


		for (int cross : crossedOffList) {
			if (cross == i) {

				Geo::Rectangle::fillRect(x, y + 0.035f, 0.35f, 0.005f, 1, 0, 0, 1);
				//Geo::LineSeg::renderLine(x, y + 0.03f, x + 0.3f, y + 0.03f, 1, 0, 0, 1);

			}
		}

	}
}

void CrosswordBoard::fillRandomWord()
{

	while (true) {

		int i = rand() % tiles.size();

		if (tiles[i].correctLetter != '\0') {

			int aboveTile = getTileAbove(i);
			int bolowTile = getTileBelow(i);
			if (aboveTile != -1 && bolowTile != -1) {
				if (tiles[aboveTile].correctLetter == '\0' && tiles[bolowTile].correctLetter != '\0') {

					int t = i;
					while (tiles[t].correctLetter != '\0') {

						tiles[t].currentLetter = tiles[t].correctLetter;
						tiles[t].locked = true;

						t = getTileBelow(t);
						if (t == -1) break;
					}

					return;
				}
			}


			int leftTile = getTileToTheLeft(i);
			int rightTile = getTileToTheRight(i);
			if (leftTile != -1 && rightTile != -1) {
				if (tiles[leftTile].correctLetter == '\0' && tiles[rightTile].correctLetter != '\0') {

					int t = i;
					while (tiles[t].correctLetter != '\0') {

						tiles[t].currentLetter = tiles[t].correctLetter;
						tiles[t].locked = true;

						t = getTileToTheRight(t);
						if (t == -1) break;
					}

					return;
				}
			}


		}
		
	}

}

void CrosswordBoard::renderRowAndColumnSizes()
{
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i].correctLetter != '\0') {

			int aboveTile = getTileAbove(i);
			int bolowTile = getTileBelow(i);
			if (bolowTile != -1) {
				if ((aboveTile == -1 || tiles[aboveTile].correctLetter == '\0') && tiles[bolowTile].correctLetter != '\0') {
					
					// word goes down
					int t = i;
					int length = 0;
					while (tiles[t].correctLetter != '\0') {
						t = getTileBelow(t);
						length++;
						if (t == -1) break;
					}

					float x = getTileRenderX(i);
					float y = getTileRenderY(i);
					y += tileHeight + 0.003f;
					x += tileWidth / 3;

					char lengthString[10];
					sprintf_s(lengthString, "%d", length);

					static BitmapText text;
					text.setFont(FontManager::getFont("assets/fonts/arial"));
					text.setText(lengthString);
					text.setColor(1, 1, 1, 1);
					text.setCharHeight(tileHeight * 0.5f);
					text.setPosition({ x, y });
					text.render();

				}
			}


			int leftTile = getTileToTheLeft(i);
			int rightTile = getTileToTheRight(i);
			if (rightTile != -1) {
				if ((leftTile == -1 || tiles[leftTile].correctLetter == '\0') && tiles[rightTile].correctLetter != '\0') {
					
					// word goes right
					int t = i;
					int length = 0;
					while (tiles[t].correctLetter != '\0') {
						t = getTileToTheRight(t);
						length++;
						if (t == -1) break;
					}

					float x = getTileRenderX(i);
					float y = getTileRenderY(i);
					x -= 0.025f;
					y += tileHeight / 3;

					char lengthString[10];
					sprintf_s(lengthString, "%d", length);

					static BitmapText text;
					text.setFont(FontManager::getFont("assets/fonts/arial"));
					text.setText(lengthString);
					text.setColor(1, 1, 1, 1);
					text.setCharHeight(tileHeight * 0.5f);
					text.setPosition({ x, y });
					text.render();


				}
			}


		}
	}

}
