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


//////////////////////////////////
//  ALL THE INHERITING OBJECTS  /
////////////////////////////////
Wall::Wall(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 0, 0, 10, 10);
  byObjChar = CHAR_WALL;
}
Invulnerability::Invulnerability(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 10, 0, 10, 10);
  byObjChar = CHAR_INVULNERABILITY;
}
Health::Health(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 40, 0, 10, 10);
  byObjChar = CHAR_HEALTH;
}
Powup::Powup(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 20, 0, 10, 10);
  byObjChar = CHAR_POWUP;
}
Powdown::Powdown(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 30, 0, 10, 10);
  byObjChar = CHAR_POWDOWN;
}
Bomn::Bomn(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 70, 0, 10, 10);
  byObjChar = CHAR_BOMN;
}
Warp::Warp(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 80, 0, 10, 10);
  byObjChar = CHAR_WARP;
}

P1start::P1start(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 0, 0, 10, 10);
  byObjChar = CHAR_P1START;
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
bool P2start::BlitToSurface(SDL_Surface * psdlsDest)
{
 if(SDL_BlitSurface(psdlsPlayers, &rcSource, psdlsDest, &rcDest) < 0)
    return false;
  return true;
}

/*
None::None(int x, int y)
 :Object(x, y)
{
//  SetRect(&rcSource, 0, 0, 10, 10);
  byObjChar = CHAR_NONE;
}
bool None::BlitToSurface(SDL_Surface * psdlsDest)
{
  return true;
}*/
