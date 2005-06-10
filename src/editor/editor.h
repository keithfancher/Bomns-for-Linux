// editor.h
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


#ifndef __EDITOR_H__
#define __EDITOR_H__


// video modes
#define MODE_WINDOWED   0
#define MODE_FULLSCREEN 1

// resource types
#define RESOURCE_SOUND   0
#define RESOURCE_GRAPHIC 1
#define RESOURCE_LEVEL   2


void InitSDL();
void SetVideoMode(SDL_Surface *, int);
void ShutDown();
void QuitWithError(const char * = NULL);
void ShowUsage();
void SetRect(SDL_Rect *, int, int, int, int);
char * LoadResource(const char *, int);
SDL_Surface * LoadImage(const char *);
void ClearSurface(SDL_Surface *);


#endif
