// level.cpp
// Copyright (C) 2001-2009 Keith Fancher <discostoo at users.sourceforge.net> 
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "level.h"
#include "bomns.h"
#include "player.h"


extern SDL_Surface * g_psdlsHUD;
extern SDL_Surface * g_psdlsObjects;
extern int           g_nGameTime;

extern CPlayer       g_Player1;
extern CPlayer       g_Player2;

extern int           g_nP1StartX;
extern int           g_nP1StartY;
extern int           g_nP2StartX;
extern int           g_nP2StartY;

int g_anLevel[80][58];


void InitLevel()
{
    
}

void FillLevel(unsigned long dwSeed, int nWalls, int nInvulns, int nHealths, int nPowUps, int nPowDowns, int nBomns, int nWarps)
{
  srand(dwSeed);
  
  memset(g_anLevel, OBJ_NONE, sizeof(g_anLevel));
  for(int i = 0; i < nWalls; i++)
    g_anLevel[rand() % 80][rand() % 58] = OBJ_WALL;
  for(int i = 0; i < nInvulns; i++)
    g_anLevel[rand() % 80][rand() % 58] = OBJ_INVULNERABILITY;
  for(int i = 0; i < nHealths; i++)
    g_anLevel[rand() % 80][rand() % 58] = OBJ_HEALTH;
  for(int i = 0; i < nPowUps; i++)
    g_anLevel[rand() % 80][rand() % 58] = OBJ_POWUP;
  for(int i = 0; i < nPowDowns; i++)
    g_anLevel[rand() % 80][rand() % 58] = OBJ_POWDOWN;
  for(int i = 0; i < nBomns; i++)
    g_anLevel[rand() % 80][rand() % 58] = OBJ_BOMN;
  for(int i = 0; i < nWarps; i++)
    g_anLevel[rand() % 80][rand() % 58] = OBJ_WARP;
}

//TODO: more efficiency?
bool DrawLevel(SDL_Surface * psdlsDest, int nP1Health, int nP2Health, int nP1Bomns, int nP2Bomns)
{
  SDL_Rect rcSrc  = {0, 0, 10, 10};
  SDL_Rect rcDest = {0, 0, 10, 10};

  for(int i = 0; i < LEVEL_WIDTH; i++)
  {
    for(int j = 0; j < LEVEL_HEIGHT; j++)
    {
      SetRect(&rcDest, i * 10, j * 10, 10, 10);

      switch(g_anLevel[i][j])
      {
        case OBJ_NONE:
          break;
        case OBJ_WALL:
          SetRect(&rcSrc, 0, 0, 10, 10);
          break;
        case OBJ_INVULNERABILITY:
          SetRect(&rcSrc, 10, 0, 10, 10);
          break;
        case OBJ_HEALTH:
          SetRect(&rcSrc, 40, 0, 10, 10);
          break;
        case OBJ_POWUP:
          SetRect(&rcSrc, 20, 0, 10, 10);
          break;
        case OBJ_POWDOWN:
          SetRect(&rcSrc, 30, 0, 10, 10);
          break;
        case OBJ_BOMN:
          SetRect(&rcSrc, 70, 0, 10, 10);
          break;
        case OBJ_WARP:
          SetRect(&rcSrc, 80, 0, 10, 10);
          break;
      }

      if(g_anLevel[i][j] != OBJ_NONE)
      {
        if(SDL_BlitSurface(g_psdlsObjects, &rcSrc, psdlsDest, &rcDest) < 0)
          return FALSE;
      }
    }//for j
  }//for i

  if(!DrawHUD(psdlsDest, nP1Health, nP2Health, nP1Bomns, nP2Bomns))
    return FALSE;
  
  return TRUE;
}

bool DrawHUD(SDL_Surface * psdlsDest, int nP1Health, int nP2Health, int nP1Bomns, int nP2Bomns)
{
  SDL_Rect rcDest = {0, 580, 800, 20};
  SDL_Rect rcSrc  = {0, 0, 800, 20};
  if(SDL_BlitSurface(g_psdlsHUD, &rcSrc, psdlsDest, &rcDest) < 0)
    return FALSE;

  //This blits whatever should be there instead of the timer
  if(!g_nGameTime)
  {
    SetRect(&rcDest, 293, 582, 215, 16);
    SetRect(&rcSrc, 0, 20, 215, 16);
    if(SDL_BlitSurface(g_psdlsHUD, &rcSrc, psdlsDest, &rcDest) < 0)
      return FALSE;
  }
  
  SetRect(&rcSrc, 50, 0, 10, 10);
  SetRect(&rcDest, 162, 585, 10, 10);
  for(int i = 0; i < nP1Health; i++, rcDest.x += 11)
  {
    if(SDL_BlitSurface(g_psdlsObjects, &rcSrc, psdlsDest, &rcDest) < 0)
      return FALSE;
  }

  SetRect(&rcSrc, 60, 0, 10, 10);
  SetRect(&rcDest, 667, 585, 10, 10);
  for(int i = 0; i < nP2Health; i++, rcDest.x += 11)
  {
    if(SDL_BlitSurface(g_psdlsObjects, &rcSrc, psdlsDest, &rcDest) < 0)
      return FALSE;
  }

  DrawNum(nP1Bomns, 67, 584, psdlsDest, RED);
  DrawNum(nP2Bomns, 571, 584, psdlsDest, BLUE);

  

  return TRUE;
}

bool DrawExplosion(int nX, int nY, SDL_Surface * psdlsDest, int nExplosionState)
{
  SDL_Rect rcDest = {nX, nY, 10, 10};
  SDL_Rect rcSrc  = {(nExplosionState == EXPLOSION_1 ? 90 : (nExplosionState == EXPLOSION_2 ? 150 : 160)), 0, 10, 10};
  if(SDL_BlitSurface(g_psdlsObjects, &rcSrc, psdlsDest, &rcDest) < 0)
    return FALSE;
    
  return TRUE;
}

bool ClearTile(int nX, int nY, SDL_Surface * psdlsDest)
{
  SDL_Rect rcDest = {nX * 10, nY * 10, 10, 10};
//  SDL_Rect rcSrc  = {X: 246 - 255}
  SDL_Rect rcSrc = {246, 0, 10, 10};
  if(SDL_BlitSurface(g_psdlsObjects, &rcSrc, psdlsDest, &rcDest) < 0)
    return FALSE;
}

bool LoadLevelFromFile(const char * filename)
{
  FILE * fpLevel     =  NULL;
  char   szLine[100] = {0};
  int    i           =  0;
  int    j           =  0;
  
  fpLevel = fopen(filename, "r");
  if(!fpLevel) // if not in the current path, try the default bomns level path (such as /usr/local/share/bomns/levels)
  {
    fpLevel = fopen(LoadResource(filename, RESOURCE_LEVEL), "r");
    if(!fpLevel) // if still not able to load there's a problem
      return FALSE;
  }

  while(!feof(fpLevel) && i < LEVEL_HEIGHT)
  {
    fgets(szLine, 100, fpLevel);
    if(szLine[0] != CHAR_COMMENT && szLine[0] != '\n' && szLine[0] != ' ') // not a comment
    {
      for(j = 0; j < LEVEL_WIDTH; j++) // god I was stupid when I did this shit, now I pay
      {
        switch(szLine[j])
        {
          case CHAR_WALL:
            g_anLevel[j][i] = OBJ_WALL;
            break;
          case CHAR_INVULNERABILITY:
            g_anLevel[j][i] = OBJ_INVULNERABILITY;
            break;
          case CHAR_HEALTH:
            g_anLevel[j][i] = OBJ_HEALTH;
            break;
          case CHAR_POWUP:
            g_anLevel[j][i] = OBJ_POWUP;
            break;
          case CHAR_POWDOWN:
            g_anLevel[j][i] = OBJ_POWDOWN;
            break;
          case CHAR_BOMN:
            g_anLevel[j][i] = OBJ_BOMN;
            break;
          case CHAR_WARP:
            g_anLevel[j][i] = OBJ_WARP;
            break;
          case CHAR_P1START:
            g_nP1StartX = j * 10;
            g_nP1StartY = i * 10;
            break;
          case CHAR_P2START:
            g_nP2StartX = j * 10;
            g_nP2StartY = i * 10;
            break;
        }
      }
      i++; // load next row
    }
    
  }

  if(fpLevel)
    fclose(fpLevel);

  return TRUE;
}
