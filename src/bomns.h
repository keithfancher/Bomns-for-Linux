// bomns.h
// Copyright (C) 2001-2002 Keith Fancher <discostoo@punkass.com> 
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


#ifndef __BOMNS_H__
#define __BOMNS_H__

#include "../config.h"

#define RESOURCE_SOUND   0
#define RESOURCE_GRAPHIC 1

#define TRUE             1
#define FALSE            0

#define RED              1
#define BLUE             2

#define RET_ERROR        0
#define RET_EXIT         1
#define RET_NORM         2

#define WINNER_NONE     -1
#define WINNER_P1        1
#define WINNER_P2        2
#define WINNER_TIE       3

#define BOMN_DAMAGE      5

//only play if the sound is enabled
#define PlayWav(x) {if(g_bSound) Mix_PlayChannel(-1, x, 0);}


bool Explode(int, int, int);
void SetRect(SDL_Rect *, int, int, int, int);
void ProcInput(SDLKey);
bool DrawNum(int, int, int, SDL_Surface *, int, bool = FALSE);
void LoadSounds();
int  Intro();
void Shutdown();
void SwitchMode();
int  DrawWinDialog(int, int, int);
void QuitWithError(const char *);
char * LoadResource(char *, int);


#endif
