#include <SDL/SDL.h>

#include "level.h"
#include "cursor.h"


Cursor::Cursor(int initXLevelPos, int initYLevelPos)
{
  xLevelPos = initXLevelPos;
  yLevelPos = initYLevelPos;
  xScreenPos = 10 * xLevelPos;
  yScreenPos = 10 * yLevelPos;

  currentObject = CHAR_NONE;
}

void Cursor::MoveUp()
{
}

void Cursor::MoveDown()
{
}

void Cursor::MoveLeft()
{
}

void Cursor::MoveRight()
{
}

bool Cursor::DrawCursor(SDL_Surface * sdlsSurface)
{
  return true;
}

void Cursor::StampCurrentObject(Level * pLevel)
{
  pLevel->SetTile(xLevelPos, yLevelPos, currentObject);
}
