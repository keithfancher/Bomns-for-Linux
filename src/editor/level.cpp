#include <SDL/SDL.h>
#include <string.h>
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
  memset(abyLevel, CHAR_NONE, sizeof(abyLevel));
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

void Level::SetTile(int xPos, int yPos, char object)
{
  abyLevel[xPos][yPos] = object;
}

bool Level::DrawLevel(SDL_Surface *)
{
  return true;
}
