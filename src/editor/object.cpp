// object.cpp
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
#include "editor.h"


// init the surfaces
SDL_Surface * Object::psdlsObjects = NULL;
SDL_Surface * Object::psdlsPlayers = NULL;


Object::Object(int x, int y)
{
  SetRect(&rcDest, x, y, 10, 10);
  
  if(!psdlsObjects)
  {
    psdlsObjects = SDL_LoadBMP(LoadResource("objects.bmp", RESOURCE_GRAPHIC));
    if(!psdlsObjects)
      QuitWithError("Error creating surface for objects.bmp resource!\n");
  }
  if(!psdlsPlayers)
  {
    psdlsPlayers = SDL_LoadBMP(LoadResource("players.bmp", RESOURCE_GRAPHIC));
    if(!psdlsPlayers)
      QuitWithError("Error creating surface for players.bmp resource!\n");
  }
}

bool Object::BlitToSurface(SDL_Surface * psdlsDest)
{
  if(SDL_BlitSurface(psdlsObjects, &rcSource, psdlsDest, &rcDest) < 0)
    return false;
  return true;
}

char Object::GetChar()
{
  return byObjChar;
}

void Object::SetPosition(int x, int y)
{
  SetRect(&rcDest, x, y, 10, 10);
}

/*
int Object::GetLevelX()
{
  return rcDest.x / 10;
}

int Object::GetLevelY()
{
  return rcDest.y / 10;
}*/


//////////////////////////////////
//  ALL THE INHERITING OBJECTS  /
////////////////////////////////
Wall::Wall(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 0, 0, 10, 10);
  byObjChar = CHAR_WALL;
}
Wall::Wall(const Object & obj)
  :Object(obj)
{
}

Invulnerability::Invulnerability(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 10, 0, 10, 10);
  byObjChar = CHAR_INVULNERABILITY;
}
Invulnerability::Invulnerability(const Object & obj)
  :Object(obj)
{
}

Health::Health(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 40, 0, 10, 10);
  byObjChar = CHAR_HEALTH;
}
Health::Health(const Object & obj)
  :Object(obj)
{
}

Powup::Powup(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 20, 0, 10, 10);
  byObjChar = CHAR_POWUP;
}
Powup::Powup(const Object & obj)
  :Object(obj)
{
}

Powdown::Powdown(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 30, 0, 10, 10);
  byObjChar = CHAR_POWDOWN;
}
Powdown::Powdown(const Object & obj)
  :Object(obj)
{
}

Bomn::Bomn(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 70, 0, 10, 10);
  byObjChar = CHAR_BOMN;
}
Bomn::Bomn(const Object & obj)
  :Object(obj)
{
}

Warp::Warp(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 80, 0, 10, 10);
  byObjChar = CHAR_WARP;
}
Warp::Warp(const Object & obj)
  :Object(obj)
{
}

P1start::P1start(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 0, 0, 10, 10);
  byObjChar = CHAR_P1START;
}
P1start::P1start(const Object & obj)
  :Object(obj)
{
}
bool P1start::BlitToSurface(SDL_Surface * psdlsDest)
{
  if(SDL_BlitSurface(psdlsPlayers, &rcSource, psdlsDest, &rcDest) < 0)
    return false;
  return true;
}

P2start::P2start(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 10, 0, 10, 10);
  byObjChar = CHAR_P2START;
}
P2start::P2start(const Object & obj)
  :Object(obj)
{
}
bool P2start::BlitToSurface(SDL_Surface * psdlsDest)
{
 if(SDL_BlitSurface(psdlsPlayers, &rcSource, psdlsDest, &rcDest) < 0)
    return false;
  return true;
}

