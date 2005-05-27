#ifndef __HUD_H__
#define __HUD_H__


class Hud
{
  public:
    Hud();

    bool DrawHUD(SDL_Surface *, int, int);  // dest surface, dest coords
    bool DrawHelp(SDL_Surface *, int, int); // dest surface, dest coords

  private:
    SDL_Surface * psdlsHUD;
    SDL_Surface * psdlsHelp;
};


#endif
