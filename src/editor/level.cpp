#include <SDL/SDL.h>
#include <string.h>
#include "object.h"
#include "level.h"


Level::Level(const char * cszFileName)
{
  //DeleteLevel();  //<-- somehow calling this causes segfaults
  memset(apobjLevel, 0, sizeof(Object *) * LEVEL_WIDTH * LEVEL_HEIGHT);
  if(cszFileName)
  {
    strncpy(szFileName, cszFileName, 80);
    ReadFromFile(szFileName);
  }
}

Level::~Level()
{
  DeleteLevel();
}

void Level::DeleteLevel()
{
  for(int i = 0; i < LEVEL_HEIGHT; i++)
  {
    for(int j = 0; j < LEVEL_WIDTH; j++)
    {
      DeleteTile(j, i);
    }
  }
}

void Level::GenerateLevel()
{
}

bool Level::ReadFromFile(const char * cszFileName)
{
  return true;
}

bool Level::WriteToFile(const char * cszFileName)
{
  return true;
}

void Level::SetTile(int xPos, int yPos, Object * object)
{
  DeleteTile(xPos, yPos);
 // apobjLevel[xPos][yPos] = new Object(*object);

  // TODO: is there a better way to do this?  giving each object its own blit function didn't
  //       let me create objects this way, but this is okay, I guess.
  switch(object->GetChar())
  {
    case CHAR_WALL:
      apobjLevel[xPos][yPos] = new Wall(*object);
      break;
    case CHAR_INVULNERABILITY:
      apobjLevel[xPos][yPos] = new Invulnerability(*object);
      break;
    case CHAR_HEALTH:
      apobjLevel[xPos][yPos] = new Health(*object);
      break;
    case CHAR_POWUP:
      apobjLevel[xPos][yPos] = new Powup(*object);
      break;
    case CHAR_POWDOWN:
      apobjLevel[xPos][yPos] = new Powdown(*object);
      break;
    case CHAR_BOMN:
      apobjLevel[xPos][yPos] = new Bomn(*object);
      break;
    case CHAR_WARP:
      apobjLevel[xPos][yPos] = new Warp(*object);
      break;
    case CHAR_P1START:
      apobjLevel[xPos][yPos] = new P1start(*object);
      break;
    case CHAR_P2START:
      apobjLevel[xPos][yPos] = new P2start(*object);
      break;
  }
}

void Level::DeleteTile(int xPos, int yPos)
{
  if(apobjLevel[xPos][yPos])
    delete apobjLevel[xPos][yPos];
  apobjLevel[xPos][yPos] = NULL;
}

bool Level::DrawLevel(SDL_Surface * psdlsDest)
{
  for(int i = 0; i < LEVEL_HEIGHT; i++)
  {
    for(int j = 0; j < LEVEL_WIDTH; j++)
    {
      if(apobjLevel[j][i])
      {
        if(!apobjLevel[j][i]->BlitToSurface(psdlsDest))
          return false;
      }
    }
  }
  return true;
}
