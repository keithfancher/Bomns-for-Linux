// player.h
// Copyright (C) 2001-2002 Keith Fancher <discostoo@punkass.com> 
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA


#ifndef __PLAYER_H__
#define __PLAYER_H__


#define PLAYER_ONE         1
#define PLAYER_TWO         2

#define DONT_MOVE          0   // <--- if you wanna get a powerup in the spot you are w/out moving, ya
#define LEFT               1   //      know, cause all the pickup code is in the move() function and...
#define RIGHT              2   //      hey, I don't need to explain myself to the likes of you!!!
#define UP                 3
#define DOWN               4

#define STATE_NORMAL       0
#define STATE_INVULNERABLE 1


class CPlayer
{
  public:
    CPlayer(int = PLAYER_ONE);
    ~CPlayer();

		bool Init(int, int);
		bool Move(int, int, int);
		void Hurt(int);
		void Drop();
		
		void Update();
		bool Draw(SDL_Surface *);

		int  Health() {return m_nHealth;}
		int  Bomns()  {return m_nBomns;}
		int  GetX()   {return m_rcDestRect.x;}
		int  GetY()   {return m_rcDestRect.y;}
		
  private:
		int           m_nPlayer;
		int           m_nState;
		int           m_nHealth;
		int           m_nBomns;
		int           m_nBlastRadius;
		Uint32        m_dwInvulnerability;
		Uint32        m_dwBomnTime;
		SDL_Rect      m_rcDestRect;
		SDL_Rect      m_rcSrcRect;
		SDL_Rect      m_rcBomnDest;
		SDL_Rect      m_rcBomnSrc;

    //sounds
    Mix_Chunk *   m_mcOw;
    Mix_Chunk *   m_mcBeep;
    bool          m_abBeeped[5];
    bool          m_bBeep;
};


#endif
