#include <SDL/SDL.h>
#include <string.h>
#include "object.h"
#include "level.h"


Level::Level(const char * cszFileName)
{
  DeleteLevel();
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
  for(int i = 0; i < LEVEL_WIDTH; i++)
  {
    for(int j = 0; j < LEVEL_HEIGHT; j++)
    {
      DeleteTile(i, j);
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
  apobjLevel[xPos][yPos] = new Object(*object);
}

void Level::DeleteTile(int xPos, int yPos)
{
  if(apobjLevel[xPos][yPos])
    delete apobjLevel[xPos][yPos];
  apobjLevel[xPos][yPos] = NULL;
}

bool Level::DrawLevel(SDL_Surface * psdlsDest)
{
  return true;
}
