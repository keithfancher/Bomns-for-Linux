// ai.cpp
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL/SDL.h>

#include "ai.h"
#include "bomns.h"
#include "level.h"


extern int        g_anLevel[80][58];
extern CPlayer    g_Player1;  //assume that AI will always be player 2
extern bool       g_bSound;


CAI_Player::CAI_Player(int nNextActionDelay) 
  // strangest syntax ever, specify base constructor AND init data
  : CPlayer(PLAYER_TWO), m_nNextAction(ACTION_NONE), m_nNextActionDelay(nNextActionDelay),
    m_dwNextActionTimer(SDL_GetTicks()), m_nGeneralDirection(0)
{
}

CAI_Player::~CAI_Player()
{
}

// do next action, then figure out the NEXT action
void CAI_Player::Update()
{
  if(SDL_GetTicks() - m_dwNextActionTimer >= m_nNextActionDelay)
  {
    m_dwNextActionTimer = SDL_GetTicks();
    
    int nDirection = Scan(5);

    // see if it's actually possible to MOVE in the suggested direction,
    // if not then move to the side first
    switch(nDirection)
    {
      //trying to move up, may not be able to
      case UP: 
        if(g_anLevel[GetX() / 10][GetY() / 10 - 1] == OBJ_WALL) //running into wall
        {
          if(g_anLevel[GetX() / 10 - 1][GetY() / 10] != OBJ_WALL)
            Move(LEFT, 0, 0);  //here we KNOW that the player is UP, so we can move up without checking
          else
            Move(RIGHT, 0, 0);
        }
        else //not running into a wall
        {
          if(Move(UP, g_Player1.GetX(), g_Player1.GetY()))
            g_Player1.Hurt(1);
        }
        break;

      //trying to move down, may not be able to
      case DOWN:
        if(g_anLevel[GetX() / 10][GetY() / 10 + 1] == OBJ_WALL) //running into a wall
        {
          if(g_anLevel[GetX() / 10 - 1][GetY() / 10] != OBJ_WALL)
            Move(LEFT, 0, 0); //here we KNOW that the player is DOWN, so we can move up without checking
          else
            Move(RIGHT, 0, 0);
        }
        else //not running into a wall
        {
          if(Move(DOWN, g_Player1.GetX(), g_Player1.GetY()))
            g_Player1.Hurt(1);
        }
        break;

      //trying to move left, may not be able to
      case LEFT:
        if(g_anLevel[GetX() / 10 - 1][GetY() / 10] == OBJ_WALL)
        {
          if(g_anLevel[GetX() / 10][GetY() / 10 - 1] != OBJ_WALL)
            Move(UP, 0, 0);  //here we KNOW that the player is LEFT, so we can move up without checking
          else
            Move(DOWN, 0, 0);
        }
        else
        {
          if(Move(LEFT, g_Player1.GetX(), g_Player1.GetY()))
            g_Player1.Hurt(1);
        }
        break;

      //trying to move right, may not be able to
      case RIGHT:
        if(g_anLevel[GetX() / 10 + 1][GetY() / 10] == OBJ_WALL) //running into a wall
        {
          if(g_anLevel[GetX() / 10][GetY() / 10 - 1] != OBJ_WALL)
            Move(UP, 0, 0); //here we KNOW that the player is RIGHT, so we can move up without checking
          else
            Move(DOWN, 0, 0);
        }
        else //not running into a wall
        {
          if(Move(RIGHT, g_Player1.GetX(), g_Player1.GetY()))
            g_Player1.Hurt(1);
        }
        break;
    }
     
/*    switch(m_nNextAction)
    {
      case ACTION_NONE:
        break;

      case ACTION_LAY_BOMN:
        Drop();
        break;

      case ACTION_MOVE_LEFT:
        if(Move(LEFT, g_Player1.GetX(), g_Player1.GetY()))
          g_Player1.Hurt(1);
        break;

      case ACTION_MOVE_RIGHT:
        if(Move(RIGHT, g_Player1.GetX(), g_Player1.GetY()))
          g_Player1.Hurt(1);
        break;
    
      case ACTION_MOVE_UP:
        if(Move(UP, g_Player1.GetX(), g_Player1.GetY()))
          g_Player1.Hurt(1);
        break;
    
      case ACTION_MOVE_DOWN:
        if(Move(DOWN, g_Player1.GetX(), g_Player1.GetY()))
          g_Player1.Hurt(1);
        break;
    }//switch */
      
//    m_nNextAction = rand() % 6;
    
    
  } //if(SDL...)


  /////////////////////////////////////////////////////////////////////////////////////////////
  //************ BEGIN CODE PASTED FROM CPLAYER's UPDATE FUNCTION ***************************//
  /////////////////////////////////////////////////////////////////////////////////////////////
  
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

// take radius to scan, return direction to move
int CAI_Player::Scan(int nRadius)
{
  //add basic melee AI first, if sees player ATTACK!!!
 /* if( (GetX() - nRadius <= g_Player1.GetX()) && 
      (GetX() + nRadius >= g_Player1.GetX()) &&
      (GetY() - nRadius <= g_Player1.GetY()) && 
      (GetY() + nRadius >= g_Player1.GetY()) )
  {*/
    
    // same X values, needs to move up or down
    if(GetX() == g_Player1.GetX())
    {
      if(GetY() > g_Player1.GetY())
        return (g_Player1.GetState() == STATE_INVULNERABLE ? DOWN : UP);  //if p1 is invulnerable, run away
      else
        return (g_Player1.GetState() == STATE_INVULNERABLE ? UP : DOWN);  //ditto
    }
    
    // otherwise try and line up X values
    else
    {
      if(GetX() > g_Player1.GetX())
        return (g_Player1.GetState() == STATE_INVULNERABLE ? RIGHT : LEFT); //if p1 is invulnerable, run away
      else
        return (g_Player1.GetState() == STATE_INVULNERABLE ? LEFT : RIGHT); //ditto
    }
 // }
  
  return 0;
}
