#include <SDL/SDL.h>
#include <string.h>
#include "object.h"
#include "level.h"


Level::Level(const char * cszFileName)
{
  ZeroLevel();
  if(cszFileName)
  {
    strncpy(szFileName, cszFileName, 80);
    ReadFromFile(szFileName);
  }
}

void Level::ZeroLevel()
{
//  memset(abyLevel, CHAR_NONE, sizeof(abyLevel));

  /*
  for(int i = 0; i < LEVEL_WIDTH; i++)
  {
    for(int j = 0; j < LEVEL_HEIGHT; j++)
    {
      Level[i][j] = 
    }
  }*/

  memset(apobjLevel, 0, sizeof(apobjLevel)); // assume if there isn't a pointer, it's OBJ_NONE
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
  apobjLevel[xPos][yPos] = new Object(*object);
}

bool Level::DrawLevel(SDL_Surface * psdlsDest)
{
  return true;
}
