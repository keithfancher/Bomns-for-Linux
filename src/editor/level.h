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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#ifndef __LEVEL_H__
#define __LEVEL_H__


// level dimensions
#define LEVEL_WIDTH          80
#define LEVEL_HEIGHT         58

// the chars to use in the .lvl file for each object
#define NUM_OBJECTS          9   // change this if you add or remove objects - DON'T count CHAR_NONE or CHAR_COMMENT
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


class Level
{
  public:
    Level(const char * = NULL);
    ~Level();

    void CreateSurfaces();

    void DeleteLevel();
    void GenerateLevel();
    bool ReadFromFile(const char * = NULL);
    bool WriteToFile(const char * = NULL);
    void SetTile(int, int, Object *);
    void DeleteTile(int, int);
    bool DrawLevel(SDL_Surface *);

    void ReplaceP1Start(int, int); // this only sets coordinates, doesn't actually create any OBJECTS
    void ReplaceP2Start(int, int);

  private:
//    char abyLevel[LEVEL_WIDTH][LEVEL_HEIGHT];
    Object * apobjLevel[LEVEL_WIDTH][LEVEL_HEIGHT]; // take THAT, hungarian notation!
//    Object * apobjLevel[LEVEL_HEIGHT][LEVEL_WIDTH]; // accessed in ROWxCOL format
    char szFileName[80];

    // should only have one starting point for each player in the level
    bool existP1Start;
    bool existP2Start;
    int  p1StartX, p1StartY;
    int  p2StartX, p2StartY;
};


#endif
