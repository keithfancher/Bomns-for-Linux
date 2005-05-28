#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "object.h"
#include "editor.h"
#include "level.h"
#include "cursor.h"
#include "hud.h"
#include "../../config.h"


int main(int argc, char ** argv)
{
  SDL_Surface * psdlsScreen     = NULL;
  int           nVideoMode      = MODE_WINDOWED;
  bool          bDone           = false;
  char          szFilename[256] = {"default.lvl\0"};
  
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
      
      // process keyboard input
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

      // buttons that should only be pressed, never held down
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

          case SDLK_F2:
            if(!level.WriteToFile(szFilename))
              fprintf(stderr, "Error writing level to: %s\n", szFilename);
            else
              fprintf(stderr, "Level successfully written to: %s\n", szFilename);
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
    fprintf(stderr, szMessage);
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
  static char szTmp[80] = {0};

  memset(szTmp, 0, sizeof(szTmp));
  sprintf(szTmp, "%s/%s/%s", DATA_PATH, (nResourceType == RESOURCE_SOUND ? "sounds" : (nResourceType == RESOURCE_GRAPHIC ? "graphics" : "levels")), szName);
  return szTmp;
}

void ClearSurface(SDL_Surface * psdlsSurface)
{
  if(SDL_FillRect(psdlsSurface, NULL, 0) < 0)
    QuitWithError("Error clearing surface!\n");
}
