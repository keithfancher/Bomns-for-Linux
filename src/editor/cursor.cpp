#include <SDL/SDL.h>

#include "object.h"
#include "level.h"
#include "cursor.h"


Cursor::Cursor(int initXLevelPos, int initYLevelPos)
{
  xLevelPos = initXLevelPos;
  yLevelPos = initYLevelPos;
  xScreenPos = 10 * xLevelPos;
  yScreenPos = 10 * yLevelPos;

//  currentObject = CHAR_NONE;
  apobjObjects[0] = new Wall(0, 0);
  apobjObjects[1] = new Invulnerability(0, 0);
  apobjObjects[2] = new Health(0, 0);
  apobjObjects[3] = new Powup(0, 0);
  apobjObjects[4] = new Powdown(0, 0);
  apobjObjects[5] = new Bomn(0, 0);
  apobjObjects[6] = new Warp(0, 0);
  apobjObjects[7] = new P1start(0, 0);
  apobjObjects[8] = new P2start(0, 0);
  currentObject = 0;

  apobjObjects[currentObject]->SetPosition(xScreenPos, yScreenPos);
}

void Cursor::MoveUp()
{
  if(--yLevelPos < 0)
    yLevelPos = 0;
  yScreenPos = 10 * yLevelPos;
  apobjObjects[currentObject]->SetPosition(xScreenPos, yScreenPos);
}

void Cursor::MoveDown()
{
  if(++yLevelPos >= LEVEL_HEIGHT)
    yLevelPos = LEVEL_HEIGHT - 1;
  yScreenPos = 10 * yLevelPos;
  apobjObjects[currentObject]->SetPosition(xScreenPos, yScreenPos);
}

void Cursor::MoveLeft()
{
  if(--xLevelPos < 0)
    xLevelPos = 0;
  xScreenPos = 10 * xLevelPos;
  apobjObjects[currentObject]->SetPosition(xScreenPos, yScreenPos);
}

void Cursor::MoveRight()
{
  if(++xLevelPos >= LEVEL_WIDTH)
    xLevelPos = LEVEL_WIDTH - 1;
  xScreenPos = xLevelPos * 10;
  apobjObjects[currentObject]->SetPosition(xScreenPos, yScreenPos);
}

void Cursor::ForwardObject()
{
  if(++currentObject >= NUM_OBJECTS)
    currentObject = 0;
  apobjObjects[currentObject]->SetPosition(xScreenPos, yScreenPos);
}

void Cursor::BackwardObject()
{
  if(--currentObject < 0)
    currentObject = NUM_OBJECTS - 1;
  apobjObjects[currentObject]->SetPosition(xScreenPos, yScreenPos);
}

bool Cursor::DrawCursor(SDL_Surface * sdlsSurface)
{
  if(!apobjObjects[currentObject]->BlitToSurface(sdlsSurface))
    return false;
  return true;
}

void Cursor::StampCurrentObject(Level * pLevel)
{
  pLevel->SetTile(xLevelPos, yLevelPos, apobjObjects[currentObject]);
}
