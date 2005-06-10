// hud.cpp
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
#include "hud.h"
#include "editor.h"


Hud::Hud()
{
  /*
  if(!psdlsHUD)
  {
    psdlsHUD = SDL_LoadBMP(LoadResource("editor_hud.bmp", RESOURCE_GRAPHIC));
    if(!psdlsHUD)
      QuitWithError("Error creating surface for editor_hud.bmp resource!\n");
  }*/
}

void Hud::CreateSurfaces()
{
//  psdlsHUD = SDL_LoadBMP(LoadResource("editor_hud.bmp", RESOURCE_GRAPHIC));
  psdlsHUD = LoadImage("editor_hud.bmp");
  if(!psdlsHUD)
    QuitWithError("Error creating surface for editor_hud.bmp resource!\n");
}

bool Hud::DrawHUD(SDL_Surface * psdlsDest, int x, int y)
{
  SDL_Rect rcDest = {x, y, 800, 20};
  if(SDL_BlitSurface(psdlsHUD, NULL, psdlsDest, &rcDest) < 0)
    return false;
  return true;
}

bool Hud::DrawHelp(SDL_Surface * psdlsDest, int x, int y)
{
  return true;
}
