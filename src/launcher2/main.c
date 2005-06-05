/* main.c
 * Copyright (C) 2001-2005 Keith Fancher <discostoo at users.sourceforge.net> 
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


/*
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
*/

#include <gtk/gtk.h>
#include <stdlib.h>

#include "main.h"
#include "interface.h"
#include "support.h"


GtkWidget * bfl_launcher_dialog;


int main (int argc, char *argv[])
{
  gtk_set_locale ();
  gtk_init (&argc, &argv);

  bfl_launcher_dialog = create_bfl_launcher_dialog ();
  gtk_widget_show (bfl_launcher_dialog);

  /* I like the launcher to not always start with default values... it's
     better if it reads your ~/.bomnsrc and adjusts itself to it */
  load_bomns_rc();

  gtk_main ();
  return 0;
}

void set_defaults()
{
  /* this is an annoying way to get all the widgets, but whatever */
  GtkWidget * walls_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "walls_spinner");
  GtkWidget * powerups_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "powerups_spinner");
  GtkWidget * invulnerabilities_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "invulnerabilities_spinner");
  GtkWidget * warps_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "warps_spinner");
  GtkWidget * bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "bomns_spinner");
  GtkWidget * powdowns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "powdowns_spinner");
  GtkWidget * health_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "health_spinner");
  GtkWidget * seed_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "seed_spinner");
  GtkWidget * p1bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "p1bomns_spinner");
  GtkWidget * p2bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "p2bomns_spinner");
  GtkWidget * matchtime_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "matchtime_spinner");
  GtkWidget * fullscreen_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "fullscreen_checkbutton");
  GtkWidget * sound_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "sound_checkbutton");
  GtkWidget * showfps_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "showfps_checkbutton");
  GtkWidget * levelfile_textentry = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "levelfile_textentry");
  GtkWidget * generate_level_radio = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "generate_level_radio");
  GtkWidget * load_from_file_radio = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "load_from_file_radio");

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(walls_spinner), 900);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(powerups_spinner), 50);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(invulnerabilities_spinner), 10);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(warps_spinner), 2);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(bomns_spinner), 20);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(powdowns_spinner), 10);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(health_spinner), 10);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(seed_spinner), 0);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(p1bomns_spinner), 10);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(p2bomns_spinner), 10);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(matchtime_spinner), 100);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fullscreen_checkbutton), TRUE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sound_checkbutton), TRUE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(showfps_checkbutton), TRUE);
/*  gtk_entry_set_text(GTK_ENTRY(levelfile_textentry), "");   I guess defaults shouldn't delete the selected level, that's annoying */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(generate_level_radio), TRUE);
}

void write_bomns_rc()
{
  GtkWidget * walls_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "walls_spinner");
  GtkWidget * powerups_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "powerups_spinner");
  GtkWidget * invulnerabilities_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "invulnerabilities_spinner");
  GtkWidget * warps_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "warps_spinner");
  GtkWidget * bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "bomns_spinner");
  GtkWidget * powdowns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "powdowns_spinner");
  GtkWidget * health_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "health_spinner");
  GtkWidget * seed_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "seed_spinner");
  GtkWidget * p1bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "p1bomns_spinner");
  GtkWidget * p2bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "p2bomns_spinner");
  GtkWidget * matchtime_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "matchtime_spinner");
  GtkWidget * fullscreen_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "fullscreen_checkbutton");
  GtkWidget * sound_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "sound_checkbutton");
  GtkWidget * showfps_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "showfps_checkbutton");
  GtkWidget * levelfile_textentry = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "levelfile_textentry");

  FILE * fpConfig        =  NULL;
  char   szFileName[256] = {0};

  sprintf(szFileName, "%s/.bomnsrc", getenv("HOME"));
  fpConfig = fopen(szFileName, "w");
  if(!fpConfig)
  {
	  fprintf(stderr, "Unable to write to file: %s\n", szFileName);
  }
  else
  {
	  fprintf(fpConfig, "# This is the config file for Bomns for Linux.\n");
	  fprintf(fpConfig, "# In here you can specify the values for a bunch\n");
	  fprintf(fpConfig, "# of different things, you'll figure it out.\n\n");

	  fprintf(fpConfig, "# this can be either true or false, I suggest setting it to true\n");
	  fprintf(fpConfig, "# if it doesn't crash your computer (which it shouldn't)\n");
	  fprintf(fpConfig, "fullscreen = %s\n\n", (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fullscreen_checkbutton)) ? "true" : "false"));

    fprintf(fpConfig, "# show the framerate or... not\n");
    fprintf(fpConfig, "showfps = %s\n\n", (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(showfps_checkbutton)) ? "true" : "false"));
  
    fprintf(fpConfig, "# pretty obvious, play sounds or not\n");
    fprintf(fpConfig, "sound = %s\n\n", (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sound_checkbutton)) ? "true" : "false"));

    fprintf(fpConfig, "# length of each match in seconds, set to 0 for unlimited time\n");
    fprintf(fpConfig, "gamelength = %d\n\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(matchtime_spinner)));

  	fprintf(fpConfig, "# these can be any integer value (within reason)\n");
  	fprintf(fpConfig, "bomns = %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(bomns_spinner)));
  	fprintf(fpConfig, "walls = %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(walls_spinner)));
  	fprintf(fpConfig, "warps = %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(warps_spinner)));
	  fprintf(fpConfig, "powups = %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(powerups_spinner)));
	  fprintf(fpConfig, "powdowns = %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(powdowns_spinner)));
	  fprintf(fpConfig, "invulnerabilities = %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(invulnerabilities_spinner)));
	  fprintf(fpConfig, "health = %d\n\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(health_spinner)));

	  fprintf(fpConfig, "# number of bomns each player starts with.\n");
	  fprintf(fpConfig, "# (set these to 0 for an interesting match)\n");
	  fprintf(fpConfig, "p1startbomns = %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(p1bomns_spinner)));
    fprintf(fpConfig, "p2startbomns = %d\n\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(p2bomns_spinner)));

  	fprintf(fpConfig, "# this is an unsigned long, or random\n");
  	fprintf(fpConfig, "# (you can also use seed = 0 for random)\n");
  	fprintf(fpConfig, "seed = %ld\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(seed_spinner)));  /* nothing to get as long? */

    fprintf(stderr, "%s written successfully\n", szFileName);
  }

  if(fpConfig)
	  fclose(fpConfig);
}

void load_bomns_rc()
{
  FILE * fpConfig        =  NULL;
	char   szBuf[128]      = {0};
  char   szFileName[512] = {0};

  sprintf(szFileName, "%s/.bomnsrc", getenv("HOME"));
  fpConfig = fopen(szFileName, "r");
	if(!fpConfig)
	{
    fprintf(stderr, "Couldn't read %s, using default settings\n", szFileName);
	}
	else
	{
    fprintf(stderr, "%s found, loading settings\n", szFileName);
		while(!feof(fpConfig))
		{
			fgets(szBuf, 128, fpConfig);
			parse_bomns_rc(szBuf);
		}
	}

	if(fpConfig)
		fclose(fpConfig);
}

void parse_bomns_rc(const char * cszBuf) /* ripped pretty much straight from config.cpp in BfL's main src */
{
  GtkWidget * walls_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "walls_spinner");
  GtkWidget * powerups_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "powerups_spinner");
  GtkWidget * invulnerabilities_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "invulnerabilities_spinner");
  GtkWidget * warps_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "warps_spinner");
  GtkWidget * bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "bomns_spinner");
  GtkWidget * powdowns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "powdowns_spinner");
  GtkWidget * health_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "health_spinner");
  GtkWidget * seed_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "seed_spinner");
  GtkWidget * p1bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "p1bomns_spinner");
  GtkWidget * p2bomns_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "p2bomns_spinner");
  GtkWidget * matchtime_spinner = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "matchtime_spinner");
  GtkWidget * fullscreen_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "fullscreen_checkbutton");
  GtkWidget * sound_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "sound_checkbutton");
  GtkWidget * showfps_checkbutton = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "showfps_checkbutton");
  GtkWidget * levelfile_textentry = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "levelfile_textentry");
  
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
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fullscreen_checkbutton), TRUE); 
		else  
		  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fullscreen_checkbutton), FALSE); 
	}
  if(!strcmp("sound", szTemp1))
  {
    if(!strcmp("true", szTemp2))
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sound_checkbutton), TRUE); 
    else 
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sound_checkbutton), FALSE); 
  }
  if(!strcmp("showfps", szTemp1))
  {
    if(!strcmp("true", szTemp2))
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(showfps_checkbutton), TRUE); 
    else 
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(showfps_checkbutton), FALSE); 
  }
  
  if(!strcmp("gamelength", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(matchtime_spinner), atoi(szTemp2));
	
  if(!strcmp("bomns", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(bomns_spinner), atoi(szTemp2)); 
	if(!strcmp("walls", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(walls_spinner), atoi(szTemp2));
	if(!strcmp("warps", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(warps_spinner), atoi(szTemp2));
	if(!strcmp("powups", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(powerups_spinner), atoi(szTemp2));
	if(!strcmp("powdowns", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(powdowns_spinner), atoi(szTemp2));
	if(!strcmp("invulnerabilities", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(invulnerabilities_spinner), atoi(szTemp2));
	if(!strcmp("health", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(health_spinner), atoi(szTemp2));

	if(!strcmp("p1startbomns", szTemp1))
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(p1bomns_spinner), atoi(szTemp2));
  if(!strcmp("p2startbomns", szTemp1))
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(p2bomns_spinner), atoi(szTemp2));
  
	if(!strcmp("seed", szTemp1))
	  gtk_spin_button_set_value(GTK_SPIN_BUTTON(seed_spinner), atoi(szTemp2));
}
