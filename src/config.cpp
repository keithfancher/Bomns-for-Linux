// config.cpp
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
#include <ctype.h>

#include "config.h"
#include "bomns.h"


bool          g_bFullScreen        = DEF_FULLSCREEN;
bool          g_bSound             = DEF_SOUND;
int           g_nGameTime          = DEF_GAMETIME;
int           g_nBomns             = DEF_BOMNS;
int           g_nWalls             = DEF_WALLS;
int           g_nWarps             = DEF_WARPS;
int           g_nPowUps            = DEF_POWUPS;
int           g_nPowDowns          = DEF_POWDOWNS;
int           g_nInvulnerabilities = DEF_INVULNERABILITIES;
int           g_nHealth            = DEF_HEALTH;
int           g_nP1StartBomns      = DEF_STARTBOMNS;
int           g_nP2StartBomns      = DEF_STARTBOMNS;
bool          g_bShowFps           = DEF_SHOWFPS;
unsigned long g_dwSeed             = SEED_RAND;


bool LoadConfigFile()
{
  FILE * fpConfig       =  NULL;
  char   szBuf[80]      = {0};
  char   szFileName[80] = {0};

  sprintf(szFileName, "%s/.bomnsrc", getenv("HOME"));
  fpConfig = fopen(szFileName, "r");
  if(!fpConfig)
  {
    if(!CreateDefaultConfig())
      return FALSE;
    if(!LoadConfigFile())
      return FALSE;
  }
  else
  {
    while(!feof(fpConfig))
    {
      fgets(szBuf, 80, fpConfig);
      Parse(szBuf);
    }
  }

  if(fpConfig)
    fclose(fpConfig);
  return TRUE;
}

bool CreateDefaultConfig()
{
  FILE * fpConfig       =  NULL;
  char   szFileName[80] = {0};

  sprintf(szFileName, "%s/.bomnsrc", getenv("HOME"));
  fpConfig = fopen(szFileName, "w");
  if(!fpConfig)
    return FALSE;

  fprintf(fpConfig, "# This is the config file for Bomns for Linux.\n");
  fprintf(fpConfig, "# In here you can specify the values for a bunch\n");
  fprintf(fpConfig, "# of different things, you'll figure it out.\n\n");

  fprintf(fpConfig, "# this can be either true or false, I suggest setting it to true\n");
  fprintf(fpConfig, "# if it doesn't crash your computer (which it shouldn't)\n");
  fprintf(fpConfig, "fullscreen = %s\n\n", (DEF_FULLSCREEN ? "true" : "false"));

  fprintf(fpConfig, "# show the framerate or... not\n");
  fprintf(fpConfig, "showfps = %s\n\n", (DEF_SHOWFPS ? "true" : "false"));
  
  fprintf(fpConfig, "# pretty obvious, play sounds or not\n");
  fprintf(fpConfig, "sound = %s\n\n", (DEF_SOUND ? "true" : "false"));

  fprintf(fpConfig, "# length of each match in seconds, set to 0 for unlimited time\n");
  fprintf(fpConfig, "gamelength = %d\n\n", DEF_GAMETIME);

  fprintf(fpConfig, "# these can be any integer value (within reason)\n");
  fprintf(fpConfig, "bomns = %d\n", DEF_BOMNS);
  fprintf(fpConfig, "walls = %d\n", DEF_WALLS);
  fprintf(fpConfig, "warps = %d\n", DEF_WARPS);
  fprintf(fpConfig, "powups = %d\n", DEF_POWUPS);
  fprintf(fpConfig, "powdowns = %d\n", DEF_POWDOWNS);
  fprintf(fpConfig, "invulnerabilities = %d\n", DEF_INVULNERABILITIES);
  fprintf(fpConfig, "health = %d\n\n", DEF_HEALTH);

  fprintf(fpConfig, "# number of bomns each player starts with.\n");
  fprintf(fpConfig, "# (set these to 0 for an interesting match)\n");
  fprintf(fpConfig, "p1startbomns = %d\n", DEF_STARTBOMNS);
  fprintf(fpConfig, "p2startbomns = %d\n\n", DEF_STARTBOMNS);

  fprintf(fpConfig, "# this is an unsigned long, or random\n");
  fprintf(fpConfig, "# (you can also use seed = 0 for random)\n");
  fprintf(fpConfig, "seed = random\n");

  fclose(fpConfig);
  return TRUE;
}

void Parse(const char * cszBuf)
{
  char szTemp1[80] = {0};
  char szTemp2[80] = {0};
  int  i           =  0;
  int  j           =  0;

  if(cszBuf[0] == '#')
    return;

  memset(szTemp1, 0, sizeof(szTemp1));
  memset(szTemp2, 0, sizeof(szTemp2));

  while(isspace(cszBuf[i]))                       //beginning spaces
    i++;
  while(!isspace(cszBuf[i]) && cszBuf[i] != '=')  //first word
    szTemp1[j++] = cszBuf[i++];
  while(isspace(cszBuf[i]) || cszBuf[i] == '=')   //middle spaces and equals sign
    i++;
  j = 0;
  while(!isspace(cszBuf[i]) && cszBuf[i] != '\n' && cszBuf[i] != EOF) //whatever it equals
    szTemp2[j++] = cszBuf[i++];

  if(!strcmp("fullscreen", szTemp1))
  {
    if(!strcmp("true", szTemp2))
      g_bFullScreen = TRUE;
    else  //to be safe
      g_bFullScreen = FALSE;
  }
  if(!strcmp("showfps", szTemp1))
  {
    if(!strcmp("true", szTemp2))
      g_bShowFps = TRUE;
    else
      g_bShowFps = FALSE;
  }
  if(!strcmp("sound", szTemp1))
  {
    if(!strcmp("true", szTemp2))
      g_bSound = TRUE;
    else //ditto
      g_bSound = FALSE;
  }
  
  if(!strcmp("gamelength", szTemp1))
    g_nGameTime = atoi(szTemp2);
  if(g_nGameTime > 999 || g_nGameTime < 0)
    g_nGameTime = DEF_GAMETIME;
    
  if(!strcmp("bomns", szTemp1))
    g_nBomns = atoi(szTemp2);  
  if(!strcmp("walls", szTemp1))
    g_nWalls = atoi(szTemp2);
  if(!strcmp("warps", szTemp1))
    g_nWarps = atoi(szTemp2);
  if(!strcmp("powups", szTemp1))
    g_nPowUps = atoi(szTemp2);
  if(!strcmp("powdowns", szTemp1))
    g_nPowDowns = atoi(szTemp2);
  if(!strcmp("invulnerabilities", szTemp1))
    g_nInvulnerabilities = atoi(szTemp2);
  if(!strcmp("health", szTemp1))
    g_nHealth = atoi(szTemp2);
  
  if(!strcmp("p1startbomns", szTemp1))
    g_nP1StartBomns = atoi(szTemp2);
  if(g_nP1StartBomns > 99 || g_nP1StartBomns < 0)
    g_nP1StartBomns = DEF_STARTBOMNS;
    
  if(!strcmp("p2startbomns", szTemp1))
    g_nP2StartBomns = atoi(szTemp2);
  if(g_nP2StartBomns > 99 || g_nP2StartBomns < 0)
    g_nP2StartBomns = DEF_STARTBOMNS;
  
  if(!strcmp("seed", szTemp1))
    g_dwSeed = atol(szTemp2);
}
