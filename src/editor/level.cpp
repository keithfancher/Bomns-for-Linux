#include <SDL/SDL.h>
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
  for(int i = 0; i < LEVEL_WIDTH; i++)
  {
    for(int j = 0; j < LEVEL_HEIGHT; j++)
      abyLevel[i][j] = CHAR_NONE;
  }
}

bool Level::ReadFromFile(const char * cszFileName)
{
  return true;
}

bool Level::WriteToFile(const char * cszFileName)
{
  return true;
}

void Level::SetTile(int xPos, int yPos, char object)
{
  abyLevel[xPos][yPos] = object;
}

bool Level::DrawLevel(SDL_Surface *)
{
  return true;
}
