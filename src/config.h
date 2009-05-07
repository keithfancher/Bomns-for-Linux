// config.h
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA

#ifndef __CONFIG_H__
#define __CONFIG_H__

//change these to change the defaults in the config file
#define DEF_FULLSCREEN     FALSE 
#define DEF_SOUND           TRUE
#define DEF_GAMETIME         100 
#define DEF_BOMNS             20
#define DEF_WALLS            900
#define DEF_WARPS              2
#define DEF_POWUPS            50
#define DEF_POWDOWNS          10
#define DEF_INVULNERABILITIES 10
#define DEF_HEALTH            10
#define DEF_STARTBOMNS        10
#define DEF_SHOWFPS         TRUE

#define SEED_RAND              0


bool LoadConfigFile();
bool CreateDefaultConfig();
void Parse(const char *);


#endif
