// cursor.cpp
// Copyright (C) 2001-2005 Keith Fancher <discostoo at users.sourceforge.net> 
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later
// version. 
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details. 
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include <SDL/SDL.h>

#include "object.h"
#include "level.h"
#include "cursor.h"
#include "editor.h"


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

  psdlsCursorBorder = SDL_LoadBMP(LoadResource("editor_cursor.bmp", RESOURCE_GRAPHIC));
  if(!psdlsCursorBorder)
    QuitWithError("Error creating surface for editor_cursor.bmp!\n");
  if(SDL_SetColorKey(psdlsCursorBorder, SDL_SRCCOLORKEY, 0) < 0)
    QuitWithError("Error setting color key for psdlsCursorBorder!\n");

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

void Cursor::SetPosition(int x, int y)
{
  xLevelPos = x;
  yLevelPos = y;

  if(xLevelPos >= LEVEL_WIDTH)
    xLevelPos = LEVEL_WIDTH-1;
  if(xLevelPos < 0)
    xLevelPos = 0;
  if(yLevelPos >= LEVEL_HEIGHT)
    yLevelPos = LEVEL_HEIGHT-1;
  if(yLevelPos < 0)
    yLevelPos = 0;
  
  xScreenPos = 10 * xLevelPos;
  yScreenPos = 10 * yLevelPos;
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
  // draw the cursor border
  SDL_Rect rcDest = {xScreenPos - 3, yScreenPos - 3, 16, 16};
  if(SDL_BlitSurface(psdlsCursorBorder, NULL, sdlsSurface, &rcDest) < 0)
    return false;
  
  if(!apobjObjects[currentObject]->BlitToSurface(sdlsSurface))
    return false;
  return true;
}

void Cursor::StampCurrentObject(Level * pLevel)
{
  pLevel->SetTile(xLevelPos, yLevelPos, apobjObjects[currentObject]);
}

void Cursor::DeleteUnderCursor(Level * pLevel)
{
  pLevel->DeleteTile(xLevelPos, yLevelPos);
}
