#ifndef __EDITOR_H__
#define __EDITOR_H__


#define MODE_WINDOWED   0
#define MODE_FULLSCREEN 1


void InitSDL();
void SetVideoMode(SDL_Surface *, int);
void ShutDown();
void QuitWithError(const char * = NULL);


#endif
