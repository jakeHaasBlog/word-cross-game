#include "game/GameLogicInterface.h"
#include "game/CrosswordBoard.h"

#include "engine/AnimatedSprite.h"
#include "ProgressBar.h"

#include <thread>
#include <memory>

// this stops the variables declared here from becoming globaly accessable
namespace {

	std::shared_ptr<CrosswordBoard> activeBoard = std::make_shared<CrosswordBoard>("assets/myWords.txt", 400, 100);
	std::shared_ptr<CrosswordBoard> reserveBoard = nullptr;
	bool exiting = false;

	std::unique_ptr<std::thread> boardLoaderThread = nullptr;

	ProgressBar pBar = ProgressBar(0, 1000, -0.3f, -0.9f, 0.6f, 0.09f);


	int initialWidth = 20;
	int initialHeight = 20;
	int nextSoftRes = 5000;
	int nextDeepRes = 500;

	float tileWidth = 0.07f;
	float tileHeight = 0.07f;

	bool winMessageShowing = false;
	AnimatedSprite winningMessage = AnimatedSprite("assets/merge_from_ofoct_good.jpg");

}

void GameLogicInterface::init() {

	window.setResolution(1920, 1080);
	
	srand(time(NULL));

	glClearColor(0.6f, 0.6f, 0.62f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	activeBoard->tileWidth = tileWidth;
	activeBoard->tileHeight = tileHeight;
	activeBoard->reset(initialWidth, initialHeight);

	boardLoaderThread = std::make_unique<std::thread>(loadNewBoard);

	int xi = 0;
	int yi = 0;
	for (int i = 0; i < 81; i++) {

		xi++;
		if (xi >= 9) {
			xi = 0;
			yi++;
		}

		int x = xi * 480;
		int y = yi * -384;
		winningMessage.addFrame(0.06f, x, y, 480, 384);
	}

}

void GameLogicInterface::update(float deltaTime)
{
	glClearColor(0.6f, 0.6f, 0.62f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	Geo::Rectangle::fillRect(activeBoard->tilesPosX - 0.07, activeBoard->tilesPosY - 0.07, activeBoard->tilesWide * activeBoard->tileWidth + 0.14, activeBoard->tilesTall * activeBoard->tileHeight + 0.14, 0, 0, 0, 1);

	activeBoard->tilesPosX = 0.0f;
	activeBoard->render();
	//board.renderWithSolution();

	int mouseoverIndex = activeBoard->getTileAt(window.getMouseX(), window.getMouseY());
	if (mouseoverIndex != -1) {
		if (activeBoard->getTile(mouseoverIndex).correctLetter != '\0') {
			float x = activeBoard->getTileRenderX(mouseoverIndex);
			float y = activeBoard->getTileRenderY(mouseoverIndex);
			Geo::Rectangle::fillRect(x, y, activeBoard->tileWidth, activeBoard->tileHeight, 1.0f, 215.0f / 255.0f, 0.2f);

			static BitmapText text;
			char letter[2];
			sprintf_s(letter, "%c", activeBoard->getTile(mouseoverIndex).currentLetter);
			text.setText(letter);
			text.setFont(FontManager::getFont("assets/fonts/arial"));
			text.setColor(0, 0, 0, 1); // text color
			text.setCharHeight(activeBoard->tileHeight * 0.9f);
			text.setPosition({ x + activeBoard->tileWidth / 3, y + activeBoard->tileHeight * 0.05f });
			text.render();
		}
	}

	if (winMessageShowing) {
		winningMessage.setX(activeBoard->tilesPosX);
		winningMessage.setY(activeBoard->tilesPosY);
		winningMessage.setScaleOrigin(2);
		winningMessage.setWidth(activeBoard->tileWidth * activeBoard->tilesWide);

		static double totalTime = 0.0f;
		totalTime += deltaTime / 1000.0f;
		winningMessage.render(totalTime);
	}

	pBar.setSize(activeBoard->tileWidth * activeBoard->tilesWide, 0.05f);
	pBar.setPosition(activeBoard->getTileRenderX(0), activeBoard->getTileRenderY(0) - (pBar.getHeight() * 2.5f));
	pBar.render();

	if (pBar.getCurrent() >= pBar.getMax()) {
		boardLoaderThread.release();
		boardLoaderThread = nullptr;
	}
}

void GameLogicInterface::cleanup() {
	exiting = true;

	boardLoaderThread.release();
	//boardLoaderThread.get()->detach();
	//boardLoaderThread = nullptr;
	activeBoard = nullptr;
	reserveBoard = nullptr;
}

void GameLogicInterface::mouseMoveCallback(double xPos, double yPos)
{

}

void GameLogicInterface::mouseButtonCallback(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (activeBoard->checkWin()) {
			winMessageShowing = !winMessageShowing;
		}
	}
}

void GameLogicInterface::keyCallback(int key, int scancode, int action, int mods)
{
	static bool wasWinningLastFrame = false;

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {

		if (pBar.getCurrent() == pBar.getMax()) {
			if (boardLoaderThread) boardLoaderThread->join();
			boardLoaderThread = nullptr;

			activeBoard = reserveBoard;
			reserveBoard = nullptr;

			boardLoaderThread = std::make_unique<std::thread>(loadNewBoard);

			winMessageShowing = false;
			wasWinningLastFrame = false;
		}

	}

	
	int mouseoverIndex = activeBoard->getTileAt(window.getMouseX(), window.getMouseY());
	if (mouseoverIndex != -1) {
		if (activeBoard->getTile(mouseoverIndex).correctLetter != '\0' && !activeBoard->getTile(mouseoverIndex).locked) {
			char input = key + 32;

			if (97 <= input && input <= 122) {
				activeBoard->getTile(mouseoverIndex).currentLetter = input;
				activeBoard->resetCrossedWordsList();
			}
			else {
				if (key == GLFW_KEY_BACKSPACE) {
					activeBoard->getTile(mouseoverIndex).currentLetter = ' ';
					activeBoard->resetCrossedWordsList();
				}
			}
		}
	}

	if (!wasWinningLastFrame) {
		if (activeBoard->checkWin()) {
			wasWinningLastFrame = true;
			winMessageShowing = true;
		}
	}

}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}

void GameLogicInterface::loadNewBoard()
{
	srand(time(NULL));

	reserveBoard = std::make_shared<CrosswordBoard>("assets/myWords.txt", nextSoftRes, nextDeepRes);
	reserveBoard->tilesWide = initialWidth;
	reserveBoard->tilesTall = initialHeight;
	reserveBoard->tileWidth = tileWidth;
	reserveBoard->tileHeight = tileHeight;

	reserveBoard->initPartialReset();

	pBar = ProgressBar(0, nextSoftRes, pBar.getX(), pBar.getY(), pBar.getWidth(), pBar.getHeight());

	int itterationsPerLoop = 500;
	for (int i = 0; i < nextSoftRes / itterationsPerLoop; i++) {
		
		if (exiting) return;

		reserveBoard->partialReset(itterationsPerLoop);
		pBar.setCurrent(i * itterationsPerLoop);
	}

	reserveBoard->finalizePartialReset();
	pBar.setCurrent(pBar.getMax());

}
