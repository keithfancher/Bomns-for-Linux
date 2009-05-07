// ai.h
// Copyright (C) 2001-2009 Keith Fancher <discostoo at users.sourceforge.net> 
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


#ifndef __AI_H__
#define __AI_H__

#include "player.h"


#define ACTION_NONE       0
#define ACTION_LAY_BOMN   1
#define ACTION_MOVE_LEFT  2
#define ACTION_MOVE_RIGHT 3
#define ACTION_MOVE_UP    4
#define ACTION_MOVE_DOWN  5


class CAI_Player : public CPlayer
{
  public:
    CAI_Player(int);
    ~CAI_Player();

    void Update();

  private:
    int           Scan(int);
    
    int           m_nGeneralDirection;
    int           m_nNextAction;
    int           m_nNextActionDelay;
    unsigned long m_dwNextActionTimer;
};

#endif
