// object.h
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


#ifndef __OBJECT_H__
#define __OBJECT_H__


// top class
class Object
{
  public:
    Object(int, int); // for now these take actual SCREEN COORDINATES, not level coords

    static void CreateSurfaces();

//    virtual bool BlitToSurface(SDL_Surface *) = 0;
    virtual bool BlitToSurface(SDL_Surface *);
    char GetChar();
    void SetPosition(int, int); // still taking SCREEN COORDs
//    int  GetLevelX();  // just divides the current screen pos. by tilesize... very useful
//    int  GetLevelY();  // just divides the current screen pos. by tilesize... very useful

  protected:
    static SDL_Surface * psdlsObjects; // I think this means all the objects share this (it'd better mean that...)
    static SDL_Surface * psdlsPlayers; // need this for the P1start and P2start graphics
    SDL_Rect rcSource;
    SDL_Rect rcDest;
    char byObjChar;
};


// all the other classes will inherit from it
class Wall : public Object
{
  public:
    Wall(int, int);
    Wall(const Object &);  // need this to make allocating new Object easier
};
class Invulnerability : public Object
{
  public:
    Invulnerability(int, int);
    Invulnerability(const Object &);  // need this to make allocating new Object easier
};
class Health : public Object
{
  public:
    Health(int, int);
    Health(const Object &);  // need this to make allocating new Object easier
};
class Powup : public Object
{
  public:
    Powup(int, int);
    Powup(const Object &);  // need this to make allocating new Object easier
};
class Powdown : public Object
{
  public:
    Powdown(int, int);
    Powdown(const Object &);  // need this to make allocating new Object easier
};
class Bomn : public Object
{
  public:
    Bomn(int, int);
    Bomn(const Object &);  // need this to make allocating new Object easier
};
class Warp : public Object
{
  public:
    Warp(int, int);
    Warp(const Object &);  // need this to make allocating new Object easier
};
class P1start : public Object
{
  public:
    P1start(int, int);
    P1start(const Object &);  // need this to make allocating new Object easier
    bool BlitToSurface(SDL_Surface *); // need this 'cause it uses a different surface
};
class P2start : public Object
{
  public:
    P2start(int, int);
    P2start(const Object &);  // need this to make allocating new Object easier
    bool BlitToSurface(SDL_Surface *); // need this 'cause it uses a different surface
};


#endif
