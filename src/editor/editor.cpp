// editor.cpp
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "object.h"
#include "editor.h"
#include "level.h"
#include "cursor.h"
#include "hud.h"
#include "config.h"


int main(int argc, char ** argv)
{
  SDL_Surface * psdlsScreen     = NULL;
  SDL_Surface * psdlsWMIcon     = NULL;
  int           nVideoMode      = MODE_WINDOWED;
  bool          bDone           = false;
  char          szFilename[512] = {"default.lvl\0"};
  
  Level         level("fuckyou");
  Cursor        cursor(0, 0);
  Hud           hud;


  // deal with command line shit
  if(argc > 2)
  {
    ShowUsage();
    exit(1);
  }
  if(argc == 2)
  {
    if(!strcmp("--help", argv[1]) || !strcmp("-h", argv[1]) || !strcmp("-help", argv[1]))
    {
      ShowUsage();
      exit(0);
    }
    sprintf(szFilename, "%s", argv[1]);
    if(!level.ReadFromFile(szFilename))
      fprintf(stderr, "Couldn't read level from: %s, assuming file doesn't exist yet\n", szFilename);
    else
      fprintf(stderr, "Level read successfully from: %s\n", szFilename);
  }
  else
    fprintf(stderr, "No file name specified, using: %s\n", szFilename);
  
  InitSDL();
//  SetVideoMode(psdlsScreen, nVideoMode);

  fprintf(stderr, "Setting window icon... ");
  psdlsWMIcon = SDL_LoadBMP(LoadResource("bomn32.bmp", RESOURCE_GRAPHIC));
  if(psdlsWMIcon)
  {
    Uint32 colorkey;
    colorkey = SDL_MapRGB(psdlsWMIcon->format, 0, 0, 0);
    SDL_SetColorKey(psdlsWMIcon, SDL_SRCCOLORKEY, colorkey);
    SDL_WM_SetIcon(psdlsWMIcon, NULL);
    fprintf(stderr, "Success!\n");
  }
  else
    fprintf(stderr, "AW JUNK! Something fishy happened...\n");
  
  // can't fucking use SetVideoMode here, for SOME REASON
  fprintf(stderr, "Setting video mode to 800x600 %s mode... ", (nVideoMode == MODE_WINDOWED ? "windowed" : "fullscreen"));
  SDL_WM_SetCaption("Bomns for Linux Level Editor", "Bomns for Linux Level Editor");
  if(nVideoMode == MODE_WINDOWED)
    psdlsScreen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF); 
  else if(nVideoMode == MODE_FULLSCREEN)
    psdlsScreen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  else
    QuitWithError("Que?\n");
  if(!psdlsScreen)
    QuitWithError("Error setting GODDAM VIDEO MODE... for some fucking reason!\n");
  else
    fprintf(stderr, "Success!\n");
  SDL_ShowCursor(false);

  // do this here so the surfaces can read the display format
  cursor.CreateSurfaces();
  hud.CreateSurfaces();
  level.CreateSurfaces();
  
  // main input loop
  while(!bDone)
  {
    SDL_Event sdleEvent;
    Uint8 *   anKeyState  = NULL;
    Uint8     nMouseState = 0;

    while(SDL_PollEvent(&sdleEvent))
    {
      if(sdleEvent.type == SDL_QUIT)
        bDone = true;

      // TODO: use relative mouse motion so it doesn't jump around
      if(sdleEvent.type == SDL_MOUSEMOTION)
        cursor.SetPosition(sdleEvent.motion.x / 10, sdleEvent.motion.y / 10);

      // process mouse input
      nMouseState = SDL_GetMouseState(NULL, NULL);
      if(nMouseState & SDL_BUTTON_LMASK)
        cursor.StampCurrentObject(&level);
      if(nMouseState & SDL_BUTTON_RMASK)
        cursor.DeleteUnderCursor(&level);

      if(sdleEvent.type == SDL_MOUSEBUTTONDOWN)
      {
        switch(sdleEvent.button.button)
        {
          case SDL_BUTTON_WHEELUP:
            cursor.ForwardObject();
            break;
          case SDL_BUTTON_WHEELDOWN:
            cursor.BackwardObject();
            break;
        }
      }
      
      // process keyboard state data
      anKeyState = SDL_GetKeyState(NULL);
      
      if(anKeyState[SDLK_UP])
        cursor.MoveUp();
      if(anKeyState[SDLK_DOWN])
        cursor.MoveDown();
      if(anKeyState[SDLK_LEFT])
        cursor.MoveLeft();
      if(anKeyState[SDLK_RIGHT])
        cursor.MoveRight();

      if(anKeyState[SDLK_SPACE] || anKeyState[SDLK_RETURN] || anKeyState[SDLK_s])
        cursor.StampCurrentObject(&level);
      if(anKeyState[SDLK_DELETE] || anKeyState[SDLK_d])
        cursor.DeleteUnderCursor(&level);

      if(anKeyState[SDLK_PAGEUP])
        cursor.ForwardObject();
      if(anKeyState[SDLK_PAGEDOWN])
        cursor.BackwardObject();

      // 1 - 9 keys select object
      if(anKeyState[SDLK_1])
        cursor.SelectObject(0);
      if(anKeyState[SDLK_2])
        cursor.SelectObject(1);
      if(anKeyState[SDLK_3])
        cursor.SelectObject(2);
      if(anKeyState[SDLK_4])
        cursor.SelectObject(3);
      if(anKeyState[SDLK_5])
        cursor.SelectObject(4);
      if(anKeyState[SDLK_6])
        cursor.SelectObject(5);
      if(anKeyState[SDLK_7])
        cursor.SelectObject(6);
      if(anKeyState[SDLK_8])
        cursor.SelectObject(7);
      if(anKeyState[SDLK_9])
        cursor.SelectObject(8);

      // buttons that should only be pressed, never held down (keyboard event data)
      if(sdleEvent.key.state == SDL_PRESSED)
      {
        switch(sdleEvent.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            bDone = true;
            break;

          case SDLK_f:
            nVideoMode = !nVideoMode;
            SetVideoMode(psdlsScreen, nVideoMode);
            break;

          case SDLK_l:
            char szTmp[520];
            sprintf(szTmp, "bomns %s", szFilename);
            if(!level.WriteToFile(szFilename))
              fprintf(stderr, "Error writing level to: %s\n", szFilename);
            else
              fprintf(stderr, "Level written successfully to: %s\n", szFilename);

            // can't do this while editor is in fullscreen without a pretty big crash
            if(nVideoMode == MODE_FULLSCREEN) // not changing the nVideoMode variable so down there...
              SetVideoMode(psdlsScreen, MODE_WINDOWED);
            
            fprintf(stderr, "Launching level in bomns...\n");
            system(szTmp);
            fprintf(stderr, "Back to editing!\n");
            
            if(nVideoMode == MODE_FULLSCREEN)  // ...we can just read it again and change accordingly
              SetVideoMode(psdlsScreen, nVideoMode);
            break;

          case SDLK_F2:
            if(!level.WriteToFile(szFilename))
              fprintf(stderr, "Error writing level to: %s\n", szFilename);
            else
              fprintf(stderr, "Level successfully written to: %s\n", szFilename);
            break;

          case SDLK_F12: // clear the level... F12 is hard to hit by mistake, right?
            level.DeleteLevel();
            break;

          default:
            break;
        }
      }
    } // SDL_PollEvent

    ClearSurface(psdlsScreen);

    if(!level.DrawLevel(psdlsScreen))
      QuitWithError("Error drawing level to screen!\n");

    if(!hud.DrawHUD(psdlsScreen, 0, 580))
      QuitWithError("Error drawing HUD to screen!\n");

    if(!cursor.DrawCursor(psdlsScreen))
      QuitWithError("Error drawing cursor to screen!\n");

    SDL_Flip(psdlsScreen);

  } // while(!bDone)
  
  ShutDown();

  return 0;
}


void InitSDL()
{
  fprintf(stderr, "Initializing SDL... ");
  if(!SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  fprintf(stderr, "Success!\n");

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void SetVideoMode(SDL_Surface * psdlsScreen, int nVideoMode)
{
  fprintf(stderr, "Setting video mode to 800x600 %s mode... ", (nVideoMode == MODE_WINDOWED ? "windowed" : "fullscreen"));

  SDL_WM_SetCaption("Bomns for Linux Level Editor", "Bomns for Linux Level Editor");

  switch(nVideoMode)
  {
    case MODE_WINDOWED:
      psdlsScreen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
      break;
    case MODE_FULLSCREEN:
      psdlsScreen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
      break;
  }
  
  if(!psdlsScreen)
    QuitWithError("Error setting video mode!\n");
  else
    fprintf(stderr, "Success!\n");
  SDL_ShowCursor(false);
}

void ShutDown()
{
  SDL_Quit();
  fprintf(stderr, "Shutdown message received, shutdown successful!\n");
}

void QuitWithError(const char * szMessage)
{
  if(szMessage)
    fputs(szMessage, stderr);
  fprintf(stderr, "SDL error message: %s\n", SDL_GetError());
  SDL_Quit();
  exit(1);
}

void ShowUsage()
{
  printf("Usage:   bomnsedit [options] <levelfile>\n");
  printf("Options: --help       shows this message\n");
  printf("         <levelfile>  the level file to use, if not provided \"default.lvl\" will be used\n");
}

void SetRect(SDL_Rect * rcRect, int nX, int nY, int nW, int nH)
{
  rcRect->x = nX;
  rcRect->y = nY;
  rcRect->w = nW;
  rcRect->h = nH;
}

char * LoadResource(const char * szName, int nResourceType)
{
  static char szTmp[512] = {0};

  memset(szTmp, 0, sizeof(szTmp));
  sprintf(szTmp, "%s/%s/%s", DATA_PATH, (nResourceType == RESOURCE_SOUND ? "sounds" : (nResourceType == RESOURCE_GRAPHIC ? "graphics" : "levels")), szName);
  return szTmp;
}

SDL_Surface * LoadImage(const char * cszFile)
{
  SDL_Surface * tmp = SDL_LoadBMP(LoadResource(cszFile, RESOURCE_GRAPHIC));
  SDL_Surface * ret = NULL;
  ret = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
  return ret;
}

void ClearSurface(SDL_Surface * psdlsSurface)
{
  if(SDL_FillRect(psdlsSurface, NULL, 0) < 0)
    QuitWithError("Error clearing surface!\n");
}
