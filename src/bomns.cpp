// bomns.cpp
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
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "bomns.h"
#include "player.h"
#include "ai.h"
#include "level.h"
#include "config.h"
#include "net.h"


#define NUM_BEGIN_SOUNDS 6 // change this to the number of sounds for the beginning wav

extern bool g_bFullScreen;
extern bool g_bSound;
extern bool g_bShowFps;
extern int  g_nGameTime;
extern int  g_nBomns;
extern int  g_nWalls;
extern int  g_nWarps;
extern int  g_nPowUps;
extern int  g_nPowDowns;
extern int  g_nInvulnerabilities;
extern int  g_nHealth;
extern unsigned long g_dwSeed;

SDL_Surface * g_psdlsScreen    = NULL;
SDL_Surface * g_psdlsPlayers   = NULL;
SDL_Surface * g_psdlsFont      = NULL;
SDL_Surface * g_psdlsIntro     = NULL;
SDL_Surface * g_psdlsWinDialog = NULL;
SDL_Surface * g_psdlsObjects   = NULL;
SDL_Surface * g_psdlsHUD       = NULL;
SDL_Surface * g_psdlsWMIcon    = NULL;   // testing icon setting
SDL_Surface * g_psdlsNetMsgs   = NULL;

Mix_Chunk   * g_mcExplosion    = NULL;   // Explosion noise
Mix_Chunk   * g_mcWinner       = NULL;   //Game ends (needed a bit more BANG)
//Mix_Chunk   * g_mcBegin        = NULL;   // When match starts
Mix_Chunk   * g_amcBegin[NUM_BEGIN_SOUNDS] = {NULL}; // change the size of this array to add more sounds

bool          g_bExploding     = FALSE;  // Fixes keyboard bug, so we know when NOT to get input


CPlayer       g_Player1(PLAYER_ONE);
CPlayer       g_Player2(PLAYER_TWO);
//CAI_Player    g_Player2(100);


//Timer vars - gotta be global so Explode() can see 'em
Uint32        g_dwGameTimer    = 0;
int           g_nGameTimeLeft  = g_nGameTime;

// default player starting coordinates
int           g_nP1StartX      = 250;
int           g_nP1StartY      = 280;
int           g_nP2StartX      = 550;
int           g_nP2StartY      = 280;


int main(int argc, char * argv[])
{
	int nP1Wins            = 0;
	int nP2Wins            = 0;

  //Framecount vars
  int     nFps           = 0;
  //int     nFramecount    = 0;
  Uint32  dwFpsTimer     = 0;
//  bool    bDrawFps       = TRUE;

  ////////////////////////////
  bool    bDrawLevel = TRUE;  ///// <== This is experimental, only draw level when there's an explosion
  ////////////////////////////
  bool    bLoadLevelFromFile  = FALSE;
  char    szLevelFileName[512] = {0};


  CheckNetplayCommandLineArgs(&argc, argv);

  // get command line options, show usage if not right
  if(argc > 3) // too many
  {
    ShowUsage();
    exit(1);
  }
  if(argv[1] && argc > 1)
  {
    if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help") || !strcmp(argv[1], "--help"))
    {
      ShowUsage();
      exit(0);
    }
  }
  

	fprintf(stderr, "\n----------------------\nBomns for Linux %s\nBy Keith Fancher\nOriginal Bomns concept by Charles Lindsay\n----------------------\n\n", VERSION);
  
  fprintf(stderr, "Loading settings in config file... ");
	if(!LoadConfigFile())
	{
		fprintf(stderr, "Unable to create and/or load config file, using defaults.\n");
	}
  else
  	fprintf(stderr, "Success!\n");
    
  // check this AFTER the config file loads so the command-line takes priority.
  if(argv[1] && argc > 1)
  {
    if(!strcmp(argv[1], "-nosound") || !strcmp(argv[1], "--nosound"))
    {
      g_bSound = FALSE;
      fprintf(stderr, "Sound was disabled because of commandline option %s\n", argv[1]);
    }
    else // if it's not --nosound, just treat it like a level to be loaded
    {
      bLoadLevelFromFile = TRUE;
      strncpy(szLevelFileName, argv[1], 512);
      szLevelFileName[511] = 0;
    }
  }

  if(argv[2] && argc > 2) // this HAS to be the level file the way command line args work now
  {
    bLoadLevelFromFile = TRUE;
    strncpy(szLevelFileName, argv[2], 512);
    szLevelFileName[511] = 0; // just in case
  }

  MaybeStartNetplay();

  // I discovered that passing SDL_INIT_AUDIO when the sound isn't available
  // causes an unnecissary pause when first loading BfL.  No more!
  if(g_bSound)
  {
    fprintf(stderr, "Initializing SDL... ");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) // TODO: is SDL_INIT_AUDIO unnecessary? and breaking shit?
    {
      fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
      exit(1);
    }
    fprintf(stderr, "Success!\n");

    fprintf(stderr, "Setting audio mode @ mono 16-bit 22050 Hz... ");
//    if (Mix_OpenAudio(11025, AUDIO_U8, 1, 512) < 0) 
//    if(Mix_OpenAudio(22050, AUDIO_S16SYS, 1, 1024) < 0)
    if(Mix_OpenAudio(22050, AUDIO_S16SYS, 1, 512) < 0) // 1024 caused a delay for audio, 256 was too small and caused stuttering
    {
	  	fprintf(stderr, "Warning: Couldn't set 22050 Hz 16-bit mono audio\n- Reason: %s\n", SDL_GetError());
      g_bSound = FALSE;    
	  }
    else
      fprintf(stderr, "Success!\n");
  }//if(g_bSound)
  
  else
  {
    fprintf(stderr, "Initializing SDL... ");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
      fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
      exit(1);
    }
    fprintf(stderr, "Success!\n");
  }

  fprintf(stderr, "Setting window icon... ");
//  g_psdlsWMIcon = LoadImage("bomn32.bmp");  // not working? (SDL_DisplayFormat hates this bmp... doesn't matter, don't need fast blitting for wm icon
  g_psdlsWMIcon = SDL_LoadBMP(LoadResource("bomn32.bmp", RESOURCE_GRAPHIC));
  if(g_psdlsWMIcon)
  {
    Uint32 colorkey;
    colorkey = SDL_MapRGB(g_psdlsWMIcon->format, 0, 0, 0);
    SDL_SetColorKey(g_psdlsWMIcon, SDL_SRCCOLORKEY, colorkey);
    SDL_WM_SetIcon(g_psdlsWMIcon, NULL);
    fprintf(stderr, "Success!\n");
  }
  else
  {
    fprintf(stderr, "AW JUNK! Something fishy happened...\n");
  }
  
	fprintf(stderr, "Setting video mode to 800x600... ");
  char szCaptionText[25] = {0};
  sprintf(szCaptionText, "Bomns for Linux %s", VERSION);
	SDL_WM_SetCaption(szCaptionText, szCaptionText);
	if(g_bFullScreen)
		g_psdlsScreen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	else
    g_psdlsScreen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

  if(!g_psdlsScreen)
  {
    fprintf(stderr, "Unable to set 800x600 video: %s\n", SDL_GetError());
    QuitWithError(NULL);
  }
  SDL_ShowCursor(FALSE);
  fprintf(stderr, "Success!\n");

  if(g_bNetPlay)
  {
    fprintf(stderr, "Loading netplay messages... ");
    g_psdlsNetMsgs = LoadImage("netmsgs.bmp");
    if(!g_psdlsNetMsgs)
      QuitWithError("Unable to load file \"graphics/netmsgs.bmp\"\n");
  }

	bool bMainDone = FALSE;
	while(!bMainDone)
	{
		fprintf(stderr, "Showing intro screen...\n");
		int nRet = Intro();
		if(nRet == RET_ERROR)
		{
			QuitWithError("Something fishy happened!\n");
		}
		if(nRet == RET_EXIT)
		{
			bMainDone = TRUE;
			break;
		}
		fprintf(stderr, "Success!\n");
    
    //############## NOT GONNA LEAVE THIS HERE ###################
    LoadSounds();
    //############################################################

		fprintf(stderr, "Loading fonts... ");
    g_psdlsFont = LoadImage("font.bmp");
//		g_psdlsFont = SDL_LoadBMP(LoadResource("font.bmp", RESOURCE_GRAPHIC));
		if(!g_psdlsFont)
		{
			QuitWithError("Unable to load graphics/font.bmp!\n");
		}
		if(SDL_SetColorKey(g_psdlsFont, SDL_SRCCOLORKEY, 0) < 0)
		{
			QuitWithError("Unable to set color key on surface g_psdlsFont!\n");
		}
		fprintf(stderr, "Success!\n");

		fprintf(stderr, "Loading win dialog box... ");
    g_psdlsWinDialog = LoadImage("winscreen.bmp");
//		g_psdlsWinDialog = SDL_LoadBMP(LoadResource("winscreen.bmp", RESOURCE_GRAPHIC));
		if(!g_psdlsWinDialog)
		{
			QuitWithError("Unable to load file \"graphics/winscreen.bmp\"\n");
		}
		fprintf(stderr, "Success!\n");

		fprintf(stderr, "Loading level... ");
    g_psdlsHUD = LoadImage("hud.bmp");
    g_psdlsObjects = LoadImage("objects.bmp");
//		g_psdlsHUD = SDL_LoadBMP(LoadResource("hud.bmp", RESOURCE_GRAPHIC));
//		g_psdlsObjects = SDL_LoadBMP(LoadResource("objects.bmp", RESOURCE_GRAPHIC));
		if(!g_psdlsHUD || !g_psdlsObjects)
		{
			QuitWithError("Failed!\n");
		}
		fprintf(stderr, "Success!\n");
		
		// fills level with correct seed
    if(bLoadLevelFromFile)
    {
      fprintf(stderr, "Loading level from file: %s... ", szLevelFileName);
      if(!LoadLevelFromFile(szLevelFileName))
      {
        fprintf(stderr, "ERROR!  Just creating a random level instead\n");
        FillLevel( (g_dwSeed == SEED_RAND ? time(NULL) : g_dwSeed) ,g_nWalls, g_nInvulnerabilities, g_nHealth, g_nPowUps, g_nPowDowns, g_nBomns, g_nWarps);
      }
      else
        fprintf(stderr, "Success!\n");
    }
    else
    {
      fprintf(stderr, "Randomly filling level with objects... ");
      FillLevel( (g_dwSeed == SEED_RAND ? time(NULL) : g_dwSeed) ,g_nWalls, g_nInvulnerabilities, g_nHealth, g_nPowUps, g_nPowDowns, g_nBomns, g_nWarps);
      fprintf(stderr, "Success!\n");
    }
    MaybeTransferLevel();
		
		fprintf(stderr, "Loading players surface into memory... ");
    g_psdlsPlayers = LoadImage("players.bmp");
//		g_psdlsPlayers = SDL_LoadBMP(LoadResource("players.bmp", RESOURCE_GRAPHIC));
		if(!g_psdlsPlayers)
		{
			QuitWithError("Failed! Couldn't load file \"graphics/players.bmp\"\n");
		}
		fprintf(stderr, "Success!\n");

		fprintf(stderr, "Initializing players... ");
		g_Player1.Init(g_nP1StartX, g_nP1StartY); // these can get changed through LoadLevelFromFile()
		g_Player2.Init(g_nP2StartX, g_nP2StartY);
		fprintf(stderr, "Success!\n");

        MaybeWaitForOpponent();

    // set the timer 
    if(g_nGameTime)
    {
      fprintf(stderr, "Match time: %d seconds\n", g_nGameTime);
      g_dwGameTimer = SDL_GetTicks();
    }

		fprintf(stderr, "Entering main loop...\n");
		bool bDone = FALSE;
    
    //play beginning match sound
//    PlayWav(g_mcBegin);
//    srand(time(NULL));
    PlayWav(g_amcBegin[rand() % NUM_BEGIN_SOUNDS]);
    
		while(!bDone)
		{
			SDL_Event sdleEvent;

      //this clears the event queue the frame after an explosion,
      //thus preventing the jumpy keyboard bug.  YAY!
      if(g_bExploding)
      {
        while(SDL_PollEvent(&sdleEvent))
          ;
        g_bExploding = FALSE;
      }

			while(SDL_PollEvent(&sdleEvent))
			{
        if (sdleEvent.type == SDL_QUIT)
					bDone = bMainDone = TRUE;

				if (sdleEvent.type == SDL_KEYDOWN)
				{
					if(sdleEvent.key.keysym.sym == SDLK_ESCAPE)
						bDone = TRUE;
					else
					{
	          ProcInput(sdleEvent.key.keysym.sym);
					}
				}
			}
      
      g_Player1.Update();
			g_Player2.Update();

      // getticks so we can figure out how long it takes to draw a frame
      if(g_bShowFps)
      {
        dwFpsTimer = SDL_GetTicks();
      }

      ///////////////experimental//////////////////
      if(bDrawLevel)
      {
        if(SDL_FillRect(g_psdlsScreen, NULL, 0) < 0)
			  {
				  QuitWithError("Unable to clear screen\n");
        }
      }
      //////////////experimental////////////////////
      
/*      ClearTile(0, 0, g_psdlsScreen);
      ClearTile(0, 1, g_psdlsScreen);
      ClearTile(1, 0, g_psdlsScreen);
      ClearTile(1, 1, g_psdlsScreen);
      ClearTile(0, 2, g_psdlsScreen);
      ClearTile(1, 2, g_psdlsScreen);
      ClearTile(1, 3, g_psdlsScreen);*/


      //////////////////// experimental ///////////////////////
      if(bDrawLevel)
      {
        if(!DrawLevel(g_psdlsScreen, g_Player1.Health(), g_Player2.Health(), g_Player1.Bomns(), g_Player2.Bomns()))
			  {
				  QuitWithError("Unable to draw level to screen\n");
			  }
      }
      ///////////////////////////////////////////////////////////

			if(!g_Player1.Draw(g_psdlsScreen))
			{
				QuitWithError("Unable to draw player1 to screen\n");
			}
			if(!g_Player2.Draw(g_psdlsScreen))
			{
				QuitWithError("Unable to draw player2 to screen\n");
			}
      
      // this is the OLD fps counting behavior, it kinda sucked
      /*if(bDrawFps)
      {
        nFramecount++;
        if(SDL_GetTicks() - dwFpsTimer >= 1000)
        {
          if(nFramecount > 99) //do we REALLY care if it's over 99?
            nFramecount = 99;
          nFps = nFramecount;
          
          dwFpsTimer = 0;
          nFramecount = 0;
          dwFpsTimer = SDL_GetTicks();
        }
        
        DrawNum(nFps, 10, 10, g_psdlsScreen, RED);
      }*/

      // update and draw the timer
      if(g_nGameTime)
      {
        g_nGameTimeLeft = g_nGameTime - ( (int)((SDL_GetTicks() - g_dwGameTimer) / 1000) );
        if(g_nGameTimeLeft <= 0)
          g_nGameTimeLeft = 0;
//        DrawNum(g_nGameTimeLeft, 395, 584, g_psdlsScreen, RED, TRUE);
        DrawNum(g_nGameTimeLeft, 392, 584, g_psdlsScreen, RED, TRUE);
      }

      // Draw the framerate
      if(g_bShowFps)
        DrawNum(nFps, 0, 0, g_psdlsScreen, RED);
            
      // FLIP THE BUFFERS!
      SDL_Flip(g_psdlsScreen);

      // calculate how long it took to draw that last frame
      if(g_bShowFps)
      {
        dwFpsTimer = SDL_GetTicks() - dwFpsTimer;
        nFps = 1000l / dwFpsTimer;
      }
      
      
      if(!g_nGameTime)
      {
        
        if(g_Player1.Health() <= 0 && g_Player2.Health() <= 0)
	  		{
		  		bDone = TRUE;
	  			DrawWinDialog(WINNER_TIE, nP1Wins, nP2Wins);
	  		}
	  		else if(g_Player1.Health() <= 0)
	  		{
	  			bDone = TRUE;
	  			DrawWinDialog(WINNER_P2, nP1Wins, ++nP2Wins);
	  		}
	  		else if(g_Player2.Health() <= 0)
	  		{
	  			bDone = TRUE;
	  			DrawWinDialog(WINNER_P1, ++nP1Wins, nP2Wins);
	  		}
      
      }
      else // g_nGameTime
      {
        if(g_nGameTimeLeft == 0)
        {
          bDone = TRUE;
          if(g_Player1.Health() == g_Player2.Health())
            DrawWinDialog(WINNER_TIE, nP1Wins, nP2Wins);
          else if(g_Player1.Health() > g_Player2.Health())
            DrawWinDialog(WINNER_P1, ++nP1Wins, nP2Wins);
          else
            DrawWinDialog(WINNER_P2, nP1Wins, ++nP2Wins);
        }
        else //g_nGameTimeLeft != 0
        {
          if(g_Player1.Health() <= 0 && g_Player2.Health() <= 0)
	  	  	{
		    		bDone = TRUE;
	  	  		DrawWinDialog(WINNER_TIE, nP1Wins, nP2Wins);
	  	  	}
	  	  	else if(g_Player1.Health() <= 0)
	  	  	{
	  	  		bDone = TRUE;
	  	  		DrawWinDialog(WINNER_P2, nP1Wins, ++nP2Wins);
	  	  	}
	  	  	else if(g_Player2.Health() <= 0)
	  	  	{
	  	  		bDone = TRUE;
	  	  		DrawWinDialog(WINNER_P1, ++nP1Wins, nP2Wins);
	  	  	}

        }
      }
      
      
		}// !bDone

	}// !bMainDone

  Shutdown();
  return 0;
}

bool Explode(int nBomnX, int nBomnY, int nRad)
{
	g_bExploding = TRUE;
  
  PlayWav(g_mcExplosion);
  
  for(int h = 1; h <= 3; h++)  //this loop just gets cooler and cooler
	{
		for(int i = nBomnX / 10 - nRad; i <= nBomnX / 10 + nRad; i++)
		{	
			for(int j = nBomnY / 10 - nRad; j <= nBomnY / 10 + nRad; j++)
			{
				if(i < 0 || i > 79 || j < 0 || j > 57)  //THAT'S where all the segfaults were coming from...
				  continue;

				if(h == 1)
				{
					if(g_anLevel[i][j] != OBJ_WARP)
						g_anLevel[i][j] = OBJ_NONE;
					if(g_Player1.GetX() / 10 == i && g_Player1.GetY() / 10 == j)
				 	 	g_Player1.Hurt(BOMN_DAMAGE);
	  			if(g_Player2.GetX() / 10 == i && g_Player2.GetY() / 10 == j)
	  				g_Player2.Hurt(BOMN_DAMAGE);
	  		}
  			
				if(!DrawExplosion(i * 10, j * 10, g_psdlsScreen, h))
				  return FALSE;
			}
		}
		
		if(!DrawHUD(g_psdlsScreen, g_Player1.Health(), g_Player2.Health(), g_Player1.Bomns(), g_Player2.Bomns())) //so it'll update the health and timer
			  return FALSE;

    // update and draw the timer
    if(g_nGameTime)
    {
      g_nGameTimeLeft = g_nGameTime - ( (int)((SDL_GetTicks() - g_dwGameTimer) / 1000) );
      if(g_nGameTimeLeft <= 0)
        g_nGameTimeLeft = 0;
      DrawNum(g_nGameTimeLeft, 395, 584, g_psdlsScreen, RED, TRUE);
   }

    
		SDL_Flip(g_psdlsScreen);
    SDL_Delay(400);
	}
	
  //g_bExploding = FALSE;
	return TRUE;
}

void SetRect(SDL_Rect * rcRect, int nX, int nY, int nW, int nH)
{
  rcRect->x = nX;
  rcRect->y = nY;
  rcRect->w = nW;
  rcRect->h = nH;
}

void ProcInput(SDLKey sdlkKey)
{
  switch(sdlkKey)
	{
		case SDLK_RIGHT:
			if(g_Player1.Move(RIGHT, g_Player2.GetX(), g_Player2.GetY()))
			  g_Player2.Hurt(1);
			break;
		case SDLK_LEFT:
		  if(g_Player1.Move(LEFT, g_Player2.GetX(), g_Player2.GetY()))
			  g_Player2.Hurt(1);
			break;
		case SDLK_UP:
		  if(g_Player1.Move(UP, g_Player2.GetX(), g_Player2.GetY()))
			  g_Player2.Hurt(1);
			break;
		case SDLK_DOWN:
		  if(g_Player1.Move(DOWN, g_Player2.GetX(), g_Player2.GetY()))
			  g_Player2.Hurt(1);
			break;
		case SDLK_RETURN:
			g_Player1.Drop();
		  break;

		case SDLK_d:
			if(g_Player2.Move(RIGHT, g_Player1.GetX(), g_Player1.GetY()))
			  g_Player1.Hurt(1);
			break;
		case SDLK_a:
			if(g_Player2.Move(LEFT, g_Player1.GetX(), g_Player1.GetY()))
			  g_Player1.Hurt(1);
			break;
		case SDLK_w:
		  if(g_Player2.Move(UP, g_Player1.GetX(), g_Player1.GetY()))
			  g_Player1.Hurt(1);
			break;
		case SDLK_s:
		  if(g_Player2.Move(DOWN, g_Player1.GetX(), g_Player1.GetY()))
			  g_Player1.Hurt(1);
			break;
		case SDLK_SPACE:
			g_Player2.Drop();
			break;

		case SDLK_f:
			SwitchMode();
 	}
}

/* TODO: New color, plus accept 3 digits, pad w/ zeroes
bool DrawNum(int nNum, int nX, int nY, SDL_Surface * psdlsDest, int nColor)  //15x20 nums
{
	int      nDig1  =  (int)floor(nNum / 10);
	int      nDig2  =  nNum % 10;
	SDL_Rect rcSrc  = {0, 0, 0, 0};
	SDL_Rect rcDest = {nX, nY, 15, 20};

	if(nDig1 != 0)
	{
		SetRect(&rcSrc, (nColor == RED ? 15 * nDig1 : 15 * nDig1 + 150), 0, 15, 20);
		if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
		  return FALSE;
		SetRect(&rcSrc, (nColor == RED ? 15 * nDig2 : 15 * nDig2 + 150), 0, 15, 20);
		SetRect(&rcDest, (nDig1 == 1 ? nX + 7 : nX + 10), nY, 15, 20);   //'cause '1' is so dern skinny
		if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
		  return FALSE;
	}
	else
	{
		SetRect(&rcSrc, (nColor == RED ? 15 * nDig2 : 15 * nDig2 + 150), 0, 15, 20);
		if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
		  return FALSE;
	}

	return TRUE;
}*/

bool DrawNum(int nNum, int nX, int nY, SDL_Surface * psdlsDest, int nColor, bool bPad)  //15x20 nums
{
	int      nDig0  = (int)floor(nNum / 100);
  int      nDig1  = (int)floor(nNum / 10);
	int      nDig2  =  nNum % 10;
	SDL_Rect rcSrc  = {0, 0, 0, 0};
	SDL_Rect rcDest = {nX, nY, 15, 20};

	//three digit num
  if(nDig0 || bPad)
  {
    int nTmp = nX;
    int nPenis = (nNum - (nDig0 * 100)); //?
    nDig1 = (int)floor(nPenis / 10);
    SetRect(&rcSrc, (nColor == RED ? 15 * nDig0 : 15 * nDig0 + 150), 0, 15, 20);
    if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
      return FALSE;
    nTmp = (nDig0 == 1 ? nTmp + 7 : nTmp + 10);
    SetRect(&rcSrc, (nColor == RED ? 15 * nDig1 : 15 * nDig1 + 150), 0, 15, 20);
		SetRect(&rcDest, nTmp, nY, 15, 20);   //'cause '1' is so dern skinny
		if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
		  return FALSE;
    nTmp = (nDig1 == 1 ? nTmp + 7 : nTmp + 10);
    SetRect(&rcSrc, (nColor == RED ? 15 * nDig2 : 15 * nDig2 + 150), 0, 15, 20);
		SetRect(&rcDest, nTmp, nY, 15, 20);   //'cause '1' is so dern skinny
		if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
		  return FALSE;

  }
  
  //two digit num 
  else if(nDig1)
	{
		SetRect(&rcSrc, (nColor == RED ? 15 * nDig1 : 15 * nDig1 + 150), 0, 15, 20);
		if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
		  return FALSE;
		SetRect(&rcSrc, (nColor == RED ? 15 * nDig2 : 15 * nDig2 + 150), 0, 15, 20);
		SetRect(&rcDest, (nDig1 == 1 ? nX + 7 : nX + 10), nY, 15, 20);   //'cause '1' is so dern skinny
		if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
		  return FALSE;
	}

  //one digit
	else
	{
		SetRect(&rcSrc, (nColor == RED ? 15 * nDig2 : 15 * nDig2 + 150), 0, 15, 20);
		if(SDL_BlitSurface(g_psdlsFont, &rcSrc, psdlsDest, &rcDest) < 0)
		  return FALSE;
	}

	return TRUE;
}


void LoadSounds()
{
  if(g_bSound)
  {
    g_mcExplosion = Mix_LoadWAV(LoadResource("explosion.wav", RESOURCE_SOUND));
    g_mcWinner    = Mix_LoadWAV(LoadResource("winner.wav", RESOURCE_SOUND));
//    g_mcBegin     = Mix_LoadWAV(LoadResource("begin.wav", RESOURCE_SOUND));
/*
    g_amcBegin[0] = Mix_LoadWAV(LoadResource("begin.wav", RESOURCE_SOUND));
    g_amcBegin[1] = Mix_LoadWAV(LoadResource("begin2.wav", RESOURCE_SOUND));
    g_amcBegin[2] = Mix_LoadWAV(LoadResource("begin3.wav", RESOURCE_SOUND));
    g_amcBegin[3] = Mix_LoadWAV(LoadResource("begin4.wav", RESOURCE_SOUND));
    g_amcBegin[4] = Mix_LoadWAV(LoadResource("begin5.wav", RESOURCE_SOUND)); */
    
    char szTmp[255] = {0};
    for(int i = 0; i < NUM_BEGIN_SOUNDS; i++)  // load the beginning sounds into their arrray
    {
      sprintf(szTmp, "begin%d.wav", i+1);
      g_amcBegin[i] = Mix_LoadWAV(LoadResource(szTmp, RESOURCE_SOUND));
    }
  }
}

// loads the image on to the surface, but converts it (testing for higher fps)
SDL_Surface * LoadImage(const char * cszFile)
{
  SDL_Surface * tmp = NULL;
  SDL_Surface * ret = NULL;

  tmp = SDL_LoadBMP(LoadResource(cszFile, RESOURCE_GRAPHIC));
  if(!tmp)
    return NULL;
  ret = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
  return ret;
}

int Intro()
{
  /*
	SDL_Surface * psdlsTux = SDL_LoadBMP(LoadResource("tux.bmp", RESOURCE_GRAPHIC));
  if(!psdlsTux)
	  return RET_ERROR; */
	
  g_psdlsIntro = LoadImage("intro.bmp");
//	g_psdlsIntro = SDL_LoadBMP(LoadResource("intro.bmp", RESOURCE_GRAPHIC));
	if(!g_psdlsIntro)
	  return RET_ERROR;
//	if(SDL_SetColorKey(g_psdlsIntro, SDL_SRCCOLORKEY, 0) < 0)
//		return RET_ERROR;
	

	bool bDone = FALSE;
	while(!bDone)
  {
    SDL_Event sdleEvent;
    while(SDL_PollEvent(&sdleEvent))
		{
      if (sdleEvent.type == SDL_QUIT)
        return RET_EXIT;
      if (sdleEvent.type == SDL_KEYDOWN)
      {
        if(sdleEvent.key.keysym.sym == SDLK_ESCAPE)
          return RET_EXIT;
				if(sdleEvent.key.keysym.sym == SDLK_f)
				  SwitchMode();
				if(sdleEvent.key.keysym.sym == SDLK_SPACE)
          bDone = TRUE;
			}
		}

		if(SDL_FillRect(g_psdlsScreen, NULL, 0) < 0)
		  return FALSE;
		
    /*
		SDL_Rect rcDest = {270, 197, 266, 350};
	  SDL_Rect rcSrc  = {0, 0, 560, 211};
		if(SDL_BlitSurface(psdlsTux, NULL, g_psdlsScreen, &rcDest) < 0)
		  return FALSE;
		SetRect(&rcDest, 115, 10, 560, 185);
		SetRect(&rcSrc, 0, 0, 560, 185);
		if(SDL_BlitSurface(g_psdlsIntro, &rcSrc, g_psdlsScreen, &rcDest) < 0)
		  return RET_ERROR;
		SetRect(&rcDest, 150, 554, 500, 50);
		SetRect(&rcSrc, 35, 186, 500, 50); */

    /*
		if(SDL_BlitSurface(g_psdlsIntro, &rcSrc, g_psdlsScreen, &rcDest) < 0)
		  return FALSE; */
		if(SDL_BlitSurface(g_psdlsIntro, NULL, g_psdlsScreen, NULL) < 0)
		  return FALSE;
		
		SDL_Flip(g_psdlsScreen);
	}
	return RET_NORM;
}

void Shutdown()
{
  if(g_mcExplosion)
    Mix_FreeChunk(g_mcExplosion);
  if(g_mcWinner)
    Mix_FreeChunk(g_mcWinner);
  
  for(int i = 0; i < NUM_BEGIN_SOUNDS; i++)
  {
    if(g_amcBegin[i])
      Mix_FreeChunk(g_amcBegin[i]);
  }
     
//  if(g_mcBegin)
//    Mix_FreeChunk(g_mcBegin);
    
  Mix_CloseAudio();
	SDL_Quit();
  fprintf(stderr, "Exit message received, shutdown successful!\n");
}

void SwitchMode()
{
	fprintf(stderr, "Changing to %s mode... ", (g_bFullScreen ? "windowed" : "fullscreen"));
	if(g_bFullScreen)
		g_psdlsScreen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	else
	  g_psdlsScreen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

	if(!g_psdlsScreen)
	{
		fprintf(stderr, "Unable to change to %s mode!\n", (g_bFullScreen ? "windowed" : "fullscreen"));
		QuitWithError(NULL);
	}
	fprintf(stderr, "Success!\n");
	g_bFullScreen = !g_bFullScreen;
}

int DrawWinDialog(int nWinner, int nP1Wins, int nP2Wins)
{
	bool bDone = FALSE;

  PlayWav(g_mcWinner);
  
	while(!bDone)
  {
    SDL_Event sdleEvent;
    while(SDL_PollEvent(&sdleEvent))
		{
      if (sdleEvent.type == SDL_QUIT)
        return RET_EXIT;
      if (sdleEvent.type == SDL_KEYDOWN)
      {
       	if(sdleEvent.key.keysym.sym == SDLK_SPACE)
          bDone = TRUE;
			}
		}

    SDL_Rect rcDest = {250, 200, 300, 200};
	  SDL_Rect rcSrc  = {0, 0, 300, 200};
		if(SDL_BlitSurface(g_psdlsWinDialog, &rcSrc, g_psdlsScreen, &rcDest) < 0)
		  return RET_ERROR;

		switch(nWinner)
		{
			case WINNER_TIE:
				SetRect(&rcSrc, 179, 202, 117, 20);
				break;

			case WINNER_P1:
				SetRect(&rcSrc, 0, 202, 134, 20);
				break;

			case WINNER_P2:
				SetRect(&rcSrc, 0, 226, 134, 20);
				break;
		}
		SetRect(&rcDest, (nWinner == WINNER_TIE ? 342 : 333), 231, 134, 20);
		if(SDL_BlitSurface(g_psdlsWinDialog, &rcSrc, g_psdlsScreen, &rcDest) < 0)
		  return RET_ERROR;

		if(!DrawNum(nP1Wins, (nP1Wins < 10 ? 355 : 350), 315, g_psdlsScreen, RED))
		  return FALSE;
		if(!DrawNum(nP2Wins, (nP2Wins < 10 ? 438 : 435), 315, g_psdlsScreen, BLUE))
		  return FALSE;

		SDL_Flip(g_psdlsScreen);
	}

	return RET_NORM;
}

void QuitWithError(const char * szMessage)
{
  if(szMessage)
    fprintf(stderr, "%s", szMessage);
  Mix_CloseAudio();
  SDL_Quit();
  exit(1);
}

void QuitWithErrorErrno(const char * szMessage, int errnum)
{
  fprintf(stderr, "%s: %s\n", szMessage, strerror(errnum));
  Mix_CloseAudio();
  SDL_Quit();
  exit(1);
}

char * LoadResource(const char * szName, int nResourceType)
{
  static char szTmp[512] = {0};

  memset(szTmp, 0, sizeof(szTmp));
  sprintf(szTmp, "%s/%s/%s", DATA_PATH, (nResourceType == RESOURCE_SOUND ? "sounds" : (nResourceType == RESOURCE_GRAPHIC ? "graphics" : "levels")), szName);
  return szTmp;
}

void ShowUsage()
{
  printf("Usage:   bomns [options] <levelfile>\n");
  printf("Options: --nosound    turns off sound, takes precidence over config file\n");
  printf("         --help       shows this message\n");
  printf("         --listen IP  listens for netplay on IP\n");
  printf("         --connect IP connects to IP for netplay\n");
  printf("                      (IP should be like 'ip:port' where ip is the IP address\n");
  printf("                      of the local adapter to listen on (for --listen), or the\n");
  printf("                      remote machine to connect to (for --connect); and port is\n");
  printf("                      the port number to listen on/connect to.)\n");
  printf("         <levelfile>  optional - the path to the level for bomns to load\n");
}
