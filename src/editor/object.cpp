#include <SDL/SDL.h>
#include "object.h"
#include "editor.h"


// init the surface
SDL_Surface * Object::psdlsObjects = NULL;


Object::Object(int x, int y)
{
  SetRect(&rcDest, x, y, 10, 10);
  
  if(!psdlsObjects)
  {
    psdlsObjects = SDL_LoadBMP(LoadResource("objects.bmp", RESOURCE_GRAPHIC));
    if(!psdlsObjects)
      QuitWithError("Error creating surface for objects.bmp resource!\n");
  }
}

bool Object::BlitToSurface(SDL_Surface * psdlsDest)
{
  return true;
}


/////////////////////
//    WALL JUNK    /
///////////////////
Wall::Wall(int x, int y)
 :Object(x, y)
{
  SetRect(&rcSource, 0, 0, 10, 10);
}

//bool Wall::BlitToSurface(SDL_Surface * psdlsDest)
//{
//  return true;
//}
