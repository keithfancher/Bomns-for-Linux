#ifndef __LEVEL_H__
#define __LEVEL_H__


// level dimensions
#define LEVEL_WIDTH          80
#define LEVEL_HEIGHT         58

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


class Level
{
  public:
    Level(const char * = NULL);
    void ZeroLevel();
    void GenerateLevel();
    bool ReadFromFile(const char * = NULL);
    bool WriteToFile(const char * = NULL);
    void SetTile(int, int, char);
    bool DrawLevel(SDL_Surface *);

  private:
    char abyLevel[LEVEL_WIDTH][LEVEL_HEIGHT];
    char szFileName[80];
};


#endif
