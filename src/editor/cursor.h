// cursor.h
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


#ifndef __CURSOR_H__
#define __CURSOR_H__


class Cursor
{
  public:
    Cursor(int = 0, int = 0); // the cursor takes LEVEL x and y

    void CreateSurfaces(); // have to call this before doing any drawing, but AFTER setting video mode

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void SetPosition(int, int); // again... LEVEL x and y

    void ForwardObject();
    void BackwardObject();
    void SelectObject(int);  // a number 0-(NUM_OBJECTS-1) for the object

    bool DrawCursor(SDL_Surface *);
    void StampCurrentObject(Level *);
    void DeleteUnderCursor(Level *);

  private:
    int xLevelPos;
    int yLevelPos;
    int xScreenPos;
    int yScreenPos;

    SDL_Surface * psdlsCursorBorder;

    Object * apobjObjects[NUM_OBJECTS];
    int currentObject;
};


#endif
