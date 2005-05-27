#include <SDL/SDL.h>
#include "hud.h"
#include "editor.h"


Hud::Hud()
{
  if(!psdlsHUD)
  {
    psdlsHUD = SDL_LoadBMP(LoadResource("editor_hud.bmp", RESOURCE_GRAPHIC));
    if(!psdlsHUD)
      QuitWithError("Error creating surface for editor_hud.bmp resource!\n");
  }

  //
  // create help surface here
  // 
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
