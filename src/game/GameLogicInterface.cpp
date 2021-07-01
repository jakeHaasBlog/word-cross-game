#include "game/GameLogicInterface.h"
#include "game/CrosswordBoard.h"

#include "engine/AnimatedSprite.h"
#include "ProgressBar.h"

#include <thread>
#include <memory>

// this stops the variables declared here from becoming globaly accessable
namespace {

	std::shared_ptr<CrosswordBoard> activeBoard = std::make_shared<CrosswordBoard>("assets/myWords.txt", 400, 100);

	std::shared_ptr<CrosswordBoard> reserveBoard_easy = nullptr;
	std::unique_ptr<std::thread> boardLoaderThread_easy = nullptr;
	ProgressBar pBar_easy = ProgressBar(0, 1000, -0.3f, -0.9f, 0.6f, 0.09f);
	int softItterations_easy = 400, deepItterations_easy = 500, easyWidth = 20, easyHeight = 20;

	std::shared_ptr<CrosswordBoard> reserveBoard_medium = std::make_shared<CrosswordBoard>("assets/myWords.txt", 4000, 1000);
	std::unique_ptr<std::thread> boardLoaderThread_medium = nullptr;
	ProgressBar pBar_medium = ProgressBar(0, 1000, -0.3f, -0.9f - 0.09f, 0.6f, 0.09f);
	int softItterations_medium = 4000, deepItterations_medium = 1000, mediumWidth = 20, mediumHeight = 20;
	
	std::shared_ptr<CrosswordBoard> reserveBoard_hard = std::make_shared<CrosswordBoard>("assets/myWords.txt", 40000, 1000);
	std::unique_ptr<std::thread> boardLoaderThread_hard = nullptr;
	ProgressBar pBar_hard = ProgressBar(0, 1000, -0.3f, -0.9f - 0.18f, 0.6f, 0.09f);
	int softItterations_hard = 40000, deepItterations_hard = 1000, hardWidth = 20, hardHeight = 20;


	float tileWidth = 0.07f;
	float tileHeight = 0.07f;

	bool winMessageShowing = false;
	AnimatedSprite winningMessage = AnimatedSprite("assets/merge_from_ofoct_good.jpg");

	bool closeAllThreads = false;

	Button button_easy = Button(0, 0, 1, 1, GameLogicInterface::swapActiveWithReserve_easy);
	Button button_medium = Button(0, 0, 1, 1, GameLogicInterface::swapActiveWithReserve_medium);
	Button button_hard = Button(0, 0, 1, 1, GameLogicInterface::swapActiveWithReserve_hard);

	bool wasWinningLastFrame = false;
}

void GameLogicInterface::init() {

	window.setResolution(1920, 1080);
	
	srand(time(NULL));

	glClearColor(0.6f, 0.6f, 0.62f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	activeBoard->tileWidth = tileWidth;
	activeBoard->tileHeight = tileHeight;
	activeBoard->reset(easyWidth, easyHeight);
	boardLoaderThread_easy = std::make_unique<std::thread>(std::thread( loadNewBoard, &reserveBoard_easy, &pBar_easy, softItterations_easy, deepItterations_easy, easyWidth, easyHeight ));

	reserveBoard_medium->tileWidth = tileWidth;
	reserveBoard_medium->tileHeight = tileHeight;
	boardLoaderThread_medium = std::make_unique<std::thread>(loadNewBoard, &reserveBoard_medium, &pBar_medium, softItterations_medium, deepItterations_medium, mediumWidth, mediumHeight);
	
	reserveBoard_hard->tileWidth = tileWidth;
	reserveBoard_hard->tileHeight = tileHeight;
	boardLoaderThread_hard = std::make_unique<std::thread>(loadNewBoard, &reserveBoard_hard, &pBar_hard, softItterations_hard, deepItterations_hard, hardWidth, hardHeight);

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

	button_easy.setPrompt("easy");
	button_easy.setTextHeight(0.06f);
	button_easy.setTextXOffset(0.05f);
	button_easy.setForegroundColor(0, 0.7f, 0);
	button_easy.setBackgroundColor(0, 0.0f, 0);

	button_medium.setPrompt("medium");
	button_medium.setTextHeight(0.05f);
	button_medium.setTextXOffset(0.04f);
	button_medium.setForegroundColor(0.7f, 0.7f, 0);
	button_medium.setBackgroundColor(0, 0.0f, 0);

	button_hard.setPrompt("hard");
	button_hard.setTextHeight(0.05f);
	button_hard.setTextXOffset(0.06f);
	button_hard.setForegroundColor(0.7f, 0.0f, 0);
	button_hard.setBackgroundColor(0, 0.0f, 0);
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

	pBar_easy.setSize(activeBoard->tileWidth * activeBoard->tilesWide, 0.05f);
	pBar_easy.setPosition(activeBoard->getTileRenderX(0), activeBoard->getTileRenderY(0) - (pBar_easy.getHeight() * 2.5f));
	pBar_easy.render();
	button_easy.setPosition(pBar_easy.getX() + pBar_easy.getWidth(), pBar_easy.getY());
	button_easy.setSize(0.2f, pBar_easy.getHeight());
	button_easy.render();

	pBar_medium.setSize(activeBoard->tileWidth * activeBoard->tilesWide, 0.05f);
	pBar_medium.setPosition(activeBoard->getTileRenderX(0), activeBoard->getTileRenderY(0) - (pBar_medium.getHeight() * 3.5f));
	pBar_medium.render();
	button_medium.setPosition(pBar_medium.getX() + pBar_medium.getWidth(), pBar_medium.getY());
	button_medium.setSize(0.2f, pBar_medium.getHeight());
	button_medium.render();

	pBar_hard.setSize(activeBoard->tileWidth * activeBoard->tilesWide, 0.05f);
	pBar_hard.setPosition(activeBoard->getTileRenderX(0), activeBoard->getTileRenderY(0) - (pBar_hard.getHeight() * 4.5f));
	pBar_hard.render();
	button_hard.setPosition(pBar_hard.getX() + pBar_hard.getWidth(), pBar_hard.getY());
	button_hard.setSize(0.2f, pBar_hard.getHeight());
	button_hard.render();
}

void GameLogicInterface::cleanup() {
	closeAllThreads = true;

	boardLoaderThread_easy->join();
	boardLoaderThread_medium->join();
	boardLoaderThread_hard->join();

	activeBoard = nullptr;
	reserveBoard_easy = nullptr;
	reserveBoard_medium = nullptr;
	reserveBoard_hard = nullptr;
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

	button_easy.mouseListener(window.getMouseX(), window.getMouseY(), button, action, mods);
	button_medium.mouseListener(window.getMouseX(), window.getMouseY(), button, action, mods);
	button_hard.mouseListener(window.getMouseX(), window.getMouseY(), button, action, mods);
}

void GameLogicInterface::keyCallback(int key, int scancode, int action, int mods)
{

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

void GameLogicInterface::swapActiveWithReserve_easy()
{
	if (pBar_easy.getCurrent() == pBar_easy.getMax()) {
		if (boardLoaderThread_easy) boardLoaderThread_easy->join();
		boardLoaderThread_easy = nullptr;

		activeBoard = reserveBoard_easy;
		reserveBoard_easy = nullptr;

		boardLoaderThread_easy = std::make_unique<std::thread>(std::thread{ loadNewBoard, &reserveBoard_easy, &pBar_easy, softItterations_easy, deepItterations_easy, easyWidth, easyHeight });


		winMessageShowing = false;
		wasWinningLastFrame = false;
	}
}

void GameLogicInterface::swapActiveWithReserve_medium()
{
	if (pBar_medium.getCurrent() == pBar_medium.getMax()) {
		if (boardLoaderThread_medium) boardLoaderThread_medium->join();
		boardLoaderThread_medium = nullptr;

		activeBoard = reserveBoard_medium;
		reserveBoard_medium = nullptr;

		boardLoaderThread_medium = std::make_unique<std::thread>(loadNewBoard, &reserveBoard_medium, &pBar_medium, softItterations_medium, deepItterations_medium, mediumWidth, mediumHeight);

		winMessageShowing = false;
		wasWinningLastFrame = false;
	}
}

void GameLogicInterface::swapActiveWithReserve_hard()
{
	if (pBar_medium.getCurrent() == pBar_medium.getMax()) {
		if (boardLoaderThread_hard) boardLoaderThread_hard->join();
		boardLoaderThread_hard = nullptr;

		activeBoard = reserveBoard_hard;
		reserveBoard_hard = nullptr;

		boardLoaderThread_hard = std::make_unique<std::thread>(loadNewBoard, &reserveBoard_hard, &pBar_hard, softItterations_hard, deepItterations_hard, hardWidth, hardHeight);

		winMessageShowing = false;
		wasWinningLastFrame = false;
	}
}

void GameLogicInterface::loadNewBoard(std::shared_ptr<CrosswordBoard>* board, ProgressBar* pBar, int softItterations, int deepItterations, int width, int height)
{
	srand(time(NULL));

	*board = std::make_shared<CrosswordBoard>("assets/myWords.txt", softItterations, deepItterations);
	(*board)->tilesWide = width;
	(*board)->tilesTall = height;
	(*board)->tileWidth = tileWidth;
	(*board)->tileHeight = tileHeight;
	
	(*board)->initPartialReset();

	*pBar = ProgressBar(0, softItterations, pBar->getX(), pBar->getY(), pBar->getWidth(), pBar->getHeight());

	int itterationsPerLoop = 100;
	for (int i = 0; i < softItterations / itterationsPerLoop; i++) {

		if (closeAllThreads) return;

		(*board)->partialReset(itterationsPerLoop);
		pBar->setCurrent(i * itterationsPerLoop);
	}

	(*board)->finalizePartialReset();
	pBar->setCurrent(pBar->getMax());

}
