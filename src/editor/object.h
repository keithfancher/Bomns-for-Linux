#ifndef __OBJECT_H__
#define __OBJECT_H__


// top class
class Object
{
  public:
    Object(int, int); // for now these take actual SCREEN COORDINATES, not level coords

//    virtual bool BlitToSurface(SDL_Surface *) = 0;
    bool BlitToSurface(SDL_Surface *);

  protected:
    static SDL_Surface * psdlsObjects; // I think this means all the objects share this (it'd better mean that...)
    SDL_Rect rcSource;
    SDL_Rect rcDest;
};


// all the other classes will inherit from it
class Wall : public Object
{
  public:
    Wall(int, int);
//    bool BlitToSurface(SDL_Surface *);
};


#endif
