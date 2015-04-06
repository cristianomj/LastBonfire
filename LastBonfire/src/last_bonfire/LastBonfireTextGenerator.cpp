#include "last_bonfire_VS\stdafx.h"
#include "last_bonfire\LastBonfire.h"
#include "last_bonfire\LastBonfireTextGenerator.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\text\GameText.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

static const wstring	W_MOUSE_COORDS_TEXT = L"Mouse: (";
static const int		W_TEXT_X = 200;
static const int		W_TEXT_Y = 10;

void LastBonfireTextGenerator::appendMouseCoords(Game *game)
{
	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	textToGenerate.append(W_MOUSE_COORDS_TEXT);
	wss << input->getMousePoint()->x;
	wss << L", ";
	wss << input->getMousePoint()->y;
	wss << L")";
	wss << L"\n";
	textToGenerate.append(wss.str());
}

/*
initText - Provides an example of how to render text to our screen every frame,
allowing for the text to continually change. This method adds a text object
to the text manager (GameText), which allows it to be updated each frame. This
method should be called only once for each object.
*/
void LastBonfireTextGenerator::initText(Game *game)
{
	// FIRST UPDATE THE TEXT THIS GENERATOR IS USING
	appendMouseCoords(game);

	// AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	GameText *text = game->getText();
	text->addText(&textToGenerate, W_TEXT_X, W_TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
}

/*
updateText - updates the text. Note that the text must already have been added
to the text manager via the initText method. Also not that there should be a
separate TextGenerator for each line of text we wish to render.
*/
void LastBonfireTextGenerator::updateText(Game *game)
{
	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.
	textToGenerate.clear();
	appendMouseCoords(game);
}