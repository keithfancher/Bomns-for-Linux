#ifndef __CURSOR_H__
#define __CURSOR_H__


class Cursor
{
  public:
    Cursor(int = 0, int = 0); // the cursor takes LEVEL x and y

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void SetPosition(int, int); // again... LEVEL x and y

    void ForwardObject();
    void BackwardObject();

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
