#ifndef __EDITOR_H__
#define __EDITOR_H__


// video modes
#define MODE_WINDOWED   0
#define MODE_FULLSCREEN 1

// resource types
#define RESOURCE_SOUND   0
#define RESOURCE_GRAPHIC 1
#define RESOURCE_LEVEL   2


void InitSDL();
void SetVideoMode(SDL_Surface *, int);
void ShutDown();
void QuitWithError(const char * = NULL);
void SetRect(SDL_Rect *, int, int, int, int);
char * LoadResource(const char *, int);
void ClearSurface(SDL_Surface *);


#endif
