#ifndef __CURSOR_H__
#define __CURSOR_H__


class Cursor
{
  public:
    Cursor(int = 0, int = 0);

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    bool DrawCursor(SDL_Surface *);
    void StampCurrentObject(Level *);

  private:
    int xLevelPos;
    int yLevelPos;
    int xScreenPos;
    int yScreenPos;

    char currentObject;
};


#endif
