#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "editor.h"


int main(int argc, char ** argv)
{
  SDL_Surface * sdlsScreen  = NULL;
  int           nWindowMode = MODE_WINDOWED;
  bool          bDone       = false;
  
  InitSDL();
  SetVideoMode(sdlsScreen, nWindowMode);
  
  // main input loop
  while(!bDone)
  {
    SDL_Event sdleEvent;
    
    while(SDL_PollEvent(&sdleEvent))
    {
      if(sdleEvent.type == SDL_QUIT)
        bDone = true;
      
      if(sdleEvent.type == SDL_KEYDOWN)
      {
        if(sdleEvent.key.keysym.sym == SDLK_ESCAPE)
          bDone = true;
      }
    } // SDL_PollEvent

  }
  
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
}

void SetVideoMode(SDL_Surface * screen, int nVideoMode)
{
  fprintf(stderr, "Setting video mode to 800x600 %s mode... ", (nVideoMode == MODE_WINDOWED ? "windowed" : "fullscreen"));

  SDL_WM_SetCaption("Bomns for Linux Level Editor", "Bomns for Linux Level Editor");

  switch(nVideoMode)
  {
    case MODE_WINDOWED:
      screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
      break;
    case MODE_FULLSCREEN:
      screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
      break;
  }
  
  if(!screen)
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
  SDL_Quit();
  exit(1);
}