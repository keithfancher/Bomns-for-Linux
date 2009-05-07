/* callbacks.c
 * Copyright (C) 2001-2009 Keith Fancher <discostoo at users.sourceforge.net> 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later
 * version. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "callbacks.h"


extern GtkWidget * spinbutton10;
extern GtkWidget * spinbutton11;
extern GtkWidget * spinbutton12;
extern GtkWidget * spinbutton13;
extern GtkWidget * spinbutton14;
extern GtkWidget * spinbutton15;
extern GtkWidget * spinbutton16;
extern GtkWidget * spinbutton18;
extern GtkWidget * spinbutton19;
extern GtkWidget * spinbutton20;
extern GtkWidget * spinbutton21;
extern GtkWidget * checkbutton1;
extern GtkWidget * checkbutton2;


int delete_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
  fprintf(stderr, "delete_event() called\n");
  return FALSE; 
}

void destroy(GtkWidget * widget, gpointer data)
{
  fprintf(stderr, "destroy() called\n");
  gtk_main_quit ();
}

void LaunchBomns()
{
  FILE * fpConfig  =  NULL;
  char   szTmp[80] = {0};

  sprintf(szTmp, "%s/.bomnsrc", getenv("HOME"));
  fpConfig = fopen(szTmp, "w");
  if(!fpConfig)
  {
    fprintf(stderr, "Unable to write to 'config'\n");
  }
  else
  {
     fprintf(fpConfig, "# This is the config file for Bomns for Linux.\n");
     fprintf(fpConfig, "# In here you can specify the values for a bunch\n");
     fprintf(fpConfig, "# of different things, you'll figure it out.\n\n");

     fprintf(fpConfig, "# this can be either true or false, I suggest setting it to true\n");
     fprintf(fpConfig, "# if it doesn't crash your computer (which it shouldn't)\n");
     fprintf(fpConfig, "fullscreen = %s\n\n", ((int)gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton1)) ? "true" : "false"));

     fprintf(fpConfig, "# pretty obvious, play sounds or not\n");
     fprintf(fpConfig, "sound = %s\n\n", ((int)gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton2)) ? "true" : "false"));

     fprintf(fpConfig, "# length of each match in seconds, set to 0 for unlimited time\n");
     fprintf(fpConfig, "gamelength = %d\n\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton21)));
     
     fprintf(fpConfig, "# these can be any integer value (within reason)\n");
     fprintf(fpConfig, "bomns = %d\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton11)));
     fprintf(fpConfig, "walls = %d\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton10)));
     fprintf(fpConfig, "warps = %d\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton15)));
     fprintf(fpConfig, "powups = %d\n",(int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton12)));
     fprintf(fpConfig, "powdowns = %d\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton13)));
     fprintf(fpConfig, "invulnerabilities = %d\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton14)));
     fprintf(fpConfig, "health = %d\n\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton18)));

     fprintf(fpConfig, "# number of bomns each player starts with.\n");
     fprintf(fpConfig, "# (set these to 0 for an interesting match)\n");
     fprintf(fpConfig, "p1startbomns = %d\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton16)));
     fprintf(fpConfig, "p2startbomns = %d\n\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton20)));

     fprintf(fpConfig, "# this is an unsigned long, or random\n");
     fprintf(fpConfig, "# (you can also use seed = 0 for random)\n");
     fprintf(fpConfig, "seed = %d\n", (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton19)));
  }
     
  if(fpConfig)
    fclose(fpConfig);
  fprintf(stderr, "Calling the bomns executable...\n");
  system("bomns");
}

void SetDefaults()
{
  fprintf(stderr, "Restoring default settings\n");
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton10), 900);  /* walls */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton11), 20);   /* bomns */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton12), 50);   /* powerups */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton13), 10);   /* powerdowns */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton14), 10);   /* invulnerabilities */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton15), 2);    /* warps */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton16), 10);   /* p1bomns */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton18), 10);   /* health */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton19), 0);    /* seed */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton20), 10);   /* p2bomns */
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton21), 100);  /* matchtime */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), TRUE);   /* fullscreen */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), TRUE);   /* sound */
}

/* I got tired of the launcher always starting with the
 * default values, I want it to load your config if you
 * have one.
 */
void LoadSettings()
{
  FILE * fpConfig       =  NULL;
  char   szFilename[80] = {0};
  char   szBuf[80]      = {0};

  sprintf(szFilename, "%s/.bomnsrc", getenv("HOME"));
  fpConfig = fopen(szFilename, "r");

  /* if there's no config file then there's nothing to do */
  if(!fpConfig)
  {
    fprintf(stderr, "Didn't find ~/.bomnsrc, using defaults\n");
    return;
  }

  fprintf(stderr, "Found ~/.bomnsrc, loading settings\n");
  while(!feof(fpConfig))
	{
		fgets(szBuf, 80, fpConfig);
		Parse(szBuf);
	}

  if(fpConfig)
    fclose(fpConfig);
}


/* This is just ripped from BfL's config.cpp, but I still want 
 * to keep the projects seperately compilable/distributable.
 */
void Parse(const char * cszBuf)
{
	char szTemp1[80] = {0};
	char szTemp2[80] = {0};
	int  i           =  0;
	int  j           =  0;

	if(cszBuf[0] == '#')
	  return;

	memset(szTemp1, 0, sizeof(szTemp1));
	memset(szTemp2, 0, sizeof(szTemp2));

	while(isspace(cszBuf[i]))                       /* beginning spaces */
		i++;
	while(!isspace(cszBuf[i]) && cszBuf[i] != '=')  /* first word */
		szTemp1[j++] = cszBuf[i++];
	while(isspace(cszBuf[i]) || cszBuf[i] == '=')   /* middle spaces and equals sign */
	  i++;
	j = 0;
	while(!isspace(cszBuf[i]) && cszBuf[i] != '\n' && cszBuf[i] != EOF) /* whatever it equals */
	  szTemp2[j++] = cszBuf[i++];

	if(!strcmp("fullscreen", szTemp1))
	{
		if(!strcmp("true", szTemp2))
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), TRUE); 
		else  
		  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), FALSE); 
	}
  if(!strcmp("sound", szTemp1))
  {
    if(!strcmp("true", szTemp2))
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), TRUE); 
    else 
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), FALSE); 
  }
  
  if(!strcmp("gamelength", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton21), atoi(szTemp2));
	
  if(!strcmp("bomns", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton11), atoi(szTemp2)); 
	if(!strcmp("walls", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton10), atoi(szTemp2));
	if(!strcmp("warps", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton15), atoi(szTemp2));
	if(!strcmp("powups", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton12), atoi(szTemp2));
	if(!strcmp("powdowns", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton13), atoi(szTemp2));
	if(!strcmp("invulnerabilities", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton14), atoi(szTemp2));
	if(!strcmp("health", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton18), atoi(szTemp2));

	if(!strcmp("p1startbomns", szTemp1))
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton16), atoi(szTemp2));
  if(!strcmp("p2startbomns", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton20), atoi(szTemp2));
  
	if(!strcmp("seed", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton19), atoi(szTemp2));
}
