// player.cpp
// Copyright (C) 2001-2005 Keith Fancher <discostoo at users.sourceforge.net> 
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


#include <string.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "player.h"
#include "bomns.h"
#include "level.h"


extern SDL_Surface * g_psdlsPlayers;
extern SDL_Surface * g_psdlsObjects;
extern int           g_anLevel[80][58];
extern int           g_nP1StartBomns;
extern int           g_nP2StartBomns;

extern bool          g_bSound;



CPlayer::CPlayer(int nPlayer)
  :m_nState(STATE_NORMAL), m_nPlayer(nPlayer), m_nHealth(10), m_dwInvulnerability(0),
	 m_nBlastRadius(1), m_dwBomnTime(0), m_mcOw(NULL), m_mcBeep(NULL), m_mcWarp(NULL), m_bBeep(FALSE)

{
  memset(&m_rcDestRect, 0, sizeof(m_rcDestRect));
	memset(&m_rcSrcRect,  0, sizeof(m_rcSrcRect));
  memset(&m_rcBomnDest, 0, sizeof(m_rcBomnDest));
  memset(&m_rcBomnSrc,  0, sizeof(m_rcBomnSrc));
  m_nBomns = (m_nPlayer == PLAYER_ONE ? g_nP1StartBomns : g_nP2StartBomns);
  memset(m_abBeeped, FALSE, sizeof(m_abBeeped));
}

CPlayer::~CPlayer()
{
  if(m_mcOw)
    Mix_FreeChunk(m_mcOw);
  if(m_mcBeep)
    Mix_FreeChunk(m_mcBeep);
  if(m_mcWarp)
    Mix_FreeChunk(m_mcWarp);
  if(m_mcInvulnerable)
    Mix_FreeChunk(m_mcInvulnerable);
}

bool CPlayer::Init(int nXStart, int nYStart)
{
  SetRect(&m_rcDestRect, nXStart, nYStart, 10, 10);
	g_anLevel[nXStart / 10][nYStart / 10] = OBJ_NONE;
	if(m_nPlayer == PLAYER_ONE)
	  SetRect(&m_rcSrcRect, 0, 0, 10, 10);
	if(m_nPlayer == PLAYER_TWO)
	  SetRect(&m_rcSrcRect, 10, 0, 10, 10);

	m_nState = STATE_NORMAL;
	m_nHealth = 10;
	m_nBomns  = (m_nPlayer == PLAYER_ONE ? g_nP1StartBomns : g_nP2StartBomns);
	m_nBlastRadius = 1;
	m_dwInvulnerability = 0;
	m_dwBomnTime = 0;
  
  // don't know if I should check for NULLs or not
//  m_mcOw = Mix_LoadWAV((m_nPlayer == PLAYER_ONE ? LoadResource("ow1.wav", RESOURCE_SOUND) : LoadResource("ow2.wav", RESOURCE_SOUND)));
  m_mcOw = Mix_LoadWAV(LoadResource("hurt.wav", RESOURCE_SOUND));
  m_mcBeep = Mix_LoadWAV(LoadResource("beep.wav", RESOURCE_SOUND));
  m_mcWarp = Mix_LoadWAV(LoadResource("warp.wav", RESOURCE_SOUND));
  m_mcInvulnerable = Mix_LoadWAV(LoadResource("invulnerable.wav", RESOURCE_SOUND));
  
	return TRUE;
}

bool CPlayer::Move(int nDirection, int nP2X, int nP2Y)
{
	int nOldX = m_rcDestRect.x;
	int nOldY = m_rcDestRect.y;

	switch(nDirection)
	{
		case LEFT:
   		m_rcDestRect.x -= 10;
			break;
		case RIGHT:
			m_rcDestRect.x += 10;
			break;
		case UP:
			m_rcDestRect.y -= 10;
		  break;
		case DOWN:
			m_rcDestRect.y += 10;
		  break;
		case DONT_MOVE:
		  break;
	}

	if(m_rcDestRect.x == nP2X && m_rcDestRect.y == nP2Y)
	{
		m_rcDestRect.x = nOldX;
		m_rcDestRect.y = nOldY;
		return TRUE;  //true if he's running into another playa
	}

	//powups 'n crap
	switch(g_anLevel[m_rcDestRect.x / 10][m_rcDestRect.y / 10])
	{
		case OBJ_HEALTH:
			if(m_nHealth < 10)
		  {
		    m_nHealth++;
		  	g_anLevel[m_rcDestRect.x / 10][m_rcDestRect.y / 10] = OBJ_NONE;
		  }
			break;

		case OBJ_INVULNERABILITY:
			if(m_nState != STATE_INVULNERABLE)
			{
        PlayWav(m_mcInvulnerable);
				m_nState = STATE_INVULNERABLE;
				m_dwInvulnerability = SDL_GetTicks();
				g_anLevel[m_rcDestRect.x / 10][m_rcDestRect.y / 10] = OBJ_NONE;
        
        if(m_nPlayer == PLAYER_ONE)
          SetRect(&m_rcSrcRect, 20, 0, 10, 10);
        if(m_nPlayer == PLAYER_TWO)
          SetRect(&m_rcSrcRect, 30, 0, 10, 10);
//				SetRect(&m_rcSrcRect, 20, 0, 10, 10);
			}
			break;

		case OBJ_BOMN:
			if(m_nBomns < 99)
			{
				m_nBomns ++;
				g_anLevel[m_rcDestRect.x / 10][m_rcDestRect.y / 10] = OBJ_NONE;
			}
			break;

		case OBJ_POWUP:
			m_nBlastRadius++;
			g_anLevel[m_rcDestRect.x / 10][m_rcDestRect.y / 10] = OBJ_NONE;
			break;

		case OBJ_POWDOWN:
			if(m_nBlastRadius > 1)
			{
				m_nBlastRadius--;
				g_anLevel[m_rcDestRect.x / 10][m_rcDestRect.y / 10] = OBJ_NONE;
			}
			break;

		case OBJ_WARP:
      //this noise is yo' daddy
      PlayWav(m_mcWarp);
      
      do
			{
				m_rcDestRect.x = ((rand() % 80) * 10);
				m_rcDestRect.y = ((rand() % 58) * 10);
			} while(g_anLevel[m_rcDestRect.x / 10][m_rcDestRect.y / 10] == OBJ_WALL
							|| (m_rcDestRect.x == nP2X && m_rcDestRect.y == nP2Y));
           
			Move(DONT_MOVE, -1, -1);  //pick up the powerup if you landed on one (p2 coords don't matter)
			break;
	}

	//walls 'n crap
	if(g_anLevel[m_rcDestRect.x / 10][m_rcDestRect.y / 10] == OBJ_WALL || m_rcDestRect.x < 0
		 || m_rcDestRect.x > 790 || m_rcDestRect.y < 0 || m_rcDestRect.y > 570)
	{
		m_rcDestRect.x = nOldX;
		m_rcDestRect.y = nOldY;
	}

	return FALSE;
}

void CPlayer::Hurt(int nDamage)
{
	if(m_nState != STATE_INVULNERABLE)
	{
		m_nHealth -= nDamage;
		if(m_nHealth < 0)
	 	  m_nHealth = 0;
    PlayWav(m_mcOw);
	}
}

void CPlayer::Drop()
{
	if(m_nBomns > 0 && !m_dwBomnTime)
	{
		m_nBomns--;
		m_dwBomnTime = SDL_GetTicks();
		SetRect(&m_rcBomnDest, m_rcDestRect.x, m_rcDestRect.y, 10, 10);		
	}
}

void CPlayer::Update()
{
	//static bool abBeeped[5] = {FALSE};
  //static bool bBeep       =  FALSE;
  
  if(m_dwInvulnerability)
	{
		if(SDL_GetTicks() - m_dwInvulnerability >= 10000)
		{
			m_nState = STATE_NORMAL;
			m_dwInvulnerability = 0;
			if(m_nPlayer == PLAYER_ONE)
			  SetRect(&m_rcSrcRect, 0, 0, 10, 10);
			if(m_nPlayer == PLAYER_TWO)
			  SetRect(&m_rcSrcRect, 10, 0, 10, 10);
		}
	}
	
	if(m_dwBomnTime)
	{
		Uint32 dwTmp = SDL_GetTicks() - m_dwBomnTime; //don wan time ta pass while figuring out if time passed...
		
    //has it beeped yet in this second?
    if(m_bBeep)
    {
      PlayWav(m_mcBeep);
      m_bBeep = FALSE;
    }
    
    if(dwTmp >= 5000)
		{
			m_dwBomnTime = 0;
			Explode(m_rcBomnDest.x, m_rcBomnDest.y, m_nBlastRadius);
			SetRect(&m_rcBomnDest, 0, 0, 0, 0);
   
      //reset the beep array for next time
      memset(m_abBeeped, FALSE, sizeof(m_abBeeped));
      m_bBeep = FALSE;

		}
		else
		{
			if(dwTmp <= 1000)
      {
  		  SetRect(&m_rcBomnSrc, 100, 0, 10, 10);
        if(!m_abBeeped[0])
        {
          m_abBeeped[0] = TRUE;
          m_bBeep = TRUE;
        }
      }
  		else if(dwTmp <= 2000)
      {
    	  SetRect(&m_rcBomnSrc, 110, 0, 10, 10);
        if(!m_abBeeped[1])
        {
          m_abBeeped[1] = TRUE;
          m_bBeep = TRUE;
        }
      }
      else if(dwTmp <= 3000)
      {
        SetRect(&m_rcBomnSrc, 120, 0, 10, 10);
        if(!m_abBeeped[2])
        {
          m_abBeeped[2] = TRUE;
          m_bBeep = TRUE;
        }
      }
      else if(dwTmp <= 4000)
      {
		  	SetRect(&m_rcBomnSrc, 130, 0, 10, 10);
        if(!m_abBeeped[3])
        {
          m_abBeeped[3] = TRUE;
          m_bBeep = TRUE;
        }
      }
      else
      {
		   	SetRect(&m_rcBomnSrc, 140, 0, 10, 10);
        if(!m_abBeeped[4])
        {
          m_abBeeped[4] = TRUE;
          m_bBeep = TRUE;
        }  
      }
    }
	}
}

bool CPlayer::Draw(SDL_Surface * psdlsDest)
{
	if(SDL_BlitSurface(g_psdlsPlayers, &m_rcSrcRect, psdlsDest, &m_rcDestRect) < 0)
	  return FALSE;
	
	if(m_dwBomnTime) //draw the bomn nums if they needa be drawn
	{
		if(SDL_BlitSurface(g_psdlsObjects, &m_rcBomnSrc, psdlsDest, &m_rcBomnDest) < 0)
	 		return FALSE;
	}
	
  return TRUE;
}

