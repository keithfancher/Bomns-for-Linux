#ifndef __OBJECT_H__
#define __OBJECT_H__


// top class
class Object
{
  public:
    Object(int, int); // for now these take actual SCREEN COORDINATES, not level coords

//    virtual bool BlitToSurface(SDL_Surface *) = 0;
    bool BlitToSurface(SDL_Surface *);
    char GetChar();
    void SetPosition(int, int); // still taking SCREEN COORDs

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
};
class Invulnerability : public Object
{
  public:
    Invulnerability(int, int);
};
class Health : public Object
{
  public:
    Health(int, int);
};
class Powup : public Object
{
  public:
    Powup(int, int);
};
class Powdown : public Object
{
  public:
    Powdown(int, int);
};
class Bomn : public Object
{
  public:
    Bomn(int, int);
};
class Warp : public Object
{
  public:
    Warp(int, int);
};
class P1start : public Object
{
  public:
    P1start(int, int);
    bool BlitToSurface(SDL_Surface *); // need this 'cause it uses a different surface
};
class P2start : public Object
{
  public:
    P2start(int, int);
    bool BlitToSurface(SDL_Surface *); // need this 'cause it uses a different surface
};

/*
class None : public Object
{
  public:
    None(int, int);
    bool BlitToSurface(SDL_Surface *); // this needs its own 'cause it won't actually DO anything (to save time)
};
*/

#endif
