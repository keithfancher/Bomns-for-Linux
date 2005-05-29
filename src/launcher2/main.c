/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "main.h"
#include "interface.h"
#include "support.h"


GtkWidget * bfl_launcher_dialog;


int main (int argc, char *argv[])
{
  gtk_set_locale ();
  gtk_init (&argc, &argv);

/*  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps"); */

  bfl_launcher_dialog = create_bfl_launcher_dialog ();
  gtk_widget_show (bfl_launcher_dialog);

  gtk_main ();
  return 0;
}

void set_defaults()
{
  // this is an annoying way to get all the widgets, but whatever
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
  	fprintf(fpConfig, "seed = %ld\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(seed_spinner)));  // nothing to get as long?

    fprintf(stderr, "%s written successfully\n", szFileName);
  }

  if(fpConfig)
	  fclose(fpConfig);
}