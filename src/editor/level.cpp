// level.cpp
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
#include <stdio.h>
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

  existP1Start = existP2Start = false;
}

Level::~Level()
{
  DeleteLevel();
}

void Level::CreateSurfaces()
{
  Object::CreateSurfaces();
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

bool Level::ReadFromFile(const char * cszFileName) // copied almost verbatim from the bomns level.cpp
{
  FILE * fpLevel     =  NULL;
  char   szLine[256] = {0};
  int    i           =  0;
  int    j           =  0;
  
  fpLevel = fopen(cszFileName, "r");
  if(!fpLevel)
    return false;

  while(!feof(fpLevel) && i < LEVEL_HEIGHT)
  {
    fgets(szLine, 256, fpLevel);
    if(szLine[0] != CHAR_COMMENT && szLine[0] != '\n' && szLine[0] != ' ') // not a comment
    {
      for(j = 0; j < LEVEL_WIDTH; j++) // god I was stupid when I did this shit, now I pay
      {
        switch(szLine[j])
        {
          case CHAR_NONE: // actually have to delete the object to make sure it's empty
            delete apobjLevel[j][i];
            apobjLevel[j][i] = NULL;
            break;
          case CHAR_WALL:
            apobjLevel[j][i] = new Wall(j*10, i*10);
            break;
          case CHAR_INVULNERABILITY:
            apobjLevel[j][i] = new Invulnerability(j*10, i*10);
            break;
          case CHAR_HEALTH:
            apobjLevel[j][i] = new Health(j*10, i*10);
            break;
          case CHAR_POWUP:
            apobjLevel[j][i] = new Powup(j*10, i*10);
            break;
          case CHAR_POWDOWN:
            apobjLevel[j][i] = new Powdown(j*10, i*10);
            break;
          case CHAR_BOMN:
            apobjLevel[j][i] = new Bomn(j*10, i*10);
            break;
          case CHAR_WARP:
            apobjLevel[j][i] = new Warp(j*10, i*10);
            break;
          case CHAR_P1START:
            apobjLevel[j][i] = new P1start(j*10, i*10);
            ReplaceP1Start(j, i);
            break;
          case CHAR_P2START:
            apobjLevel[j][i] = new P2start(j*10, i*10);
            ReplaceP2Start(j, i);
            break;
        }
      }
      i++; // load next row
    }
  }

  if(fpLevel)
    fclose(fpLevel);

  return true;
}

bool Level::WriteToFile(const char * cszFileName)
{
  FILE * fpLevel = NULL;

  fpLevel = fopen(cszFileName, "w");
  if(!fpLevel)
    return false;

  fprintf(fpLevel, "# Level generated with the Bomns for Linux level editor, feel free to fuck with it!\n\n");
  fprintf(fpLevel, "# KEY:\n");
  fprintf(fpLevel, "#   %c - empty space\n", CHAR_NONE);
  fprintf(fpLevel, "#   %c - invulnerability\n", CHAR_INVULNERABILITY);
  fprintf(fpLevel, "#   %c - health\n", CHAR_HEALTH);
  fprintf(fpLevel, "#   %c - bomn powerup\n", CHAR_POWUP);
  fprintf(fpLevel, "#   %c - bomn powerdown\n", CHAR_POWDOWN);
  fprintf(fpLevel, "#   %c - bomn\n", CHAR_BOMN);
  fprintf(fpLevel, "#   %c - warp\n", CHAR_WARP);
  fprintf(fpLevel, "#   %c - p1 start position (only put 1 of these at a time on each map)\n", CHAR_P1START);
  fprintf(fpLevel, "#   %c - p2 start position (only put 1 of these at a time on each map)\n\n", CHAR_P2START);

  for(int i = 0; i < LEVEL_HEIGHT; i++)
  {
    for(int j = 0; j < LEVEL_WIDTH; j++)
    {
      if(apobjLevel[j][i]) // if the object exists, it's not a space, write the char
        fputc(apobjLevel[j][i]->GetChar(), fpLevel);
      else // otherwise write the "none" char
        fputc(CHAR_NONE, fpLevel);
    }
    fputc('\n', fpLevel);
  }
  
  if(fpLevel)
    fclose(fpLevel);
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
      ReplaceP1Start(xPos, yPos);
      break;
    case CHAR_P2START:
      apobjLevel[xPos][yPos] = new P2start(*object);
      ReplaceP2Start(xPos, yPos);
      break;
  }
}

void Level::DeleteTile(int xPos, int yPos)
{
  if(apobjLevel[xPos][yPos])
    delete apobjLevel[xPos][yPos];
  apobjLevel[xPos][yPos] = NULL;

  // make sure the p1 and p2starts get cleared
  if(existP1Start && xPos == p1StartX && yPos == p1StartY)
    existP1Start = false;
  if(existP2Start && xPos == p2StartX && yPos == p2StartY)
    existP2Start = false;
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

void Level::ReplaceP1Start(int xPos, int yPos)
{
  if(existP1Start)
    DeleteTile(p1StartX, p1StartY); // delete the old one
  existP1Start = true;
  p1StartX = xPos;
  p1StartY = yPos;
}

void Level::ReplaceP2Start(int xPos, int yPos)
{
  if(existP2Start)
    DeleteTile(p2StartX, p2StartY); // delete the old one
  existP2Start = true;
  p2StartX = xPos;
  p2StartY = yPos;
}
