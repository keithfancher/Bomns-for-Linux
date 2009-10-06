// level.h
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


#ifndef __LEVEL_H__
#define __LEVEL_H__

//80x58 level
#define LEVEL_WIDTH         80
#define LEVEL_HEIGHT        58

// different objects filling the level
#define OBJ_NONE            0
#define OBJ_WALL            1
#define OBJ_INVULNERABILITY 2
#define OBJ_HEALTH          3
#define OBJ_POWUP           4
#define OBJ_POWDOWN         5
#define OBJ_BOMN            6
#define OBJ_WARP            7

// the chars to use in the .lvl file for each object
#define CHAR_NONE            '.'
#define CHAR_WALL            '@'
#define CHAR_INVULNERABILITY '!'
#define CHAR_HEALTH          '+'
#define CHAR_POWUP           '^'
#define CHAR_POWDOWN         '-'
#define CHAR_BOMN            'o'
#define CHAR_WARP            '*'
#define CHAR_P1START         '1'
#define CHAR_P2START         '2'
#define CHAR_COMMENT         '#' // lines beginning with this will be ignored in the .lvl file

// different stages of exposions (affects the sprite used)
#define EXPLOSION_1         1
#define EXPLOSION_2         2
#define EXPLOSION_3         3

void InitLevel();
void FillLevel(unsigned long, int, int, int, int, int, int, int);  //seed, wall, invul, health, powup, powdown, bomns, warps
bool DrawLevel(SDL_Surface *, int, int, int, int);
bool DrawHUD(SDL_Surface *, int, int, int, int);    //don't ask
bool DrawExplosion(int nX, int nY, SDL_Surface * psdlsDest, int = EXPLOSION_1);
bool ClearTile(int, int, SDL_Surface *);
bool LoadLevelFromFile(const char *);


extern int g_anLevel[LEVEL_WIDTH][LEVEL_HEIGHT];


#endif
