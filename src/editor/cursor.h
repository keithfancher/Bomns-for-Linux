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

    void ForwardObject();
    void BackwardObject();

    bool DrawCursor(SDL_Surface *);
    void StampCurrentObject(Level *);

  private:
    int xLevelPos;
    int yLevelPos;
    int xScreenPos;
    int yScreenPos;

    Object * apobjObjects[NUM_OBJECTS];
    int currentObject;
};


#endif
