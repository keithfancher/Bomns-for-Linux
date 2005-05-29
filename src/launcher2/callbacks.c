/*
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
*/

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "main.h"


extern GtkWidget * bfl_launcher_dialog;


void
on_load_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget * level_select_dialog;

  level_select_dialog = create_level_select_dialog();
  gtk_widget_show(level_select_dialog);
}


void
on_play_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget * levelfile_textentry  = lookup_widget(GTK_WIDGET(button), "levelfile_textentry");
  GtkWidget * generate_level_radio = lookup_widget(GTK_WIDGET(button), "generate_level_radio");
  char * szFileName = (char *)gtk_entry_get_text(GTK_ENTRY(levelfile_textentry));
  char szTmp[525] = {0};

  /* so the executable will use our settings */
  write_bomns_rc();
  
  /* should we generate random level... */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(generate_level_radio)))
    sprintf(szTmp, "bomns");

  /* ...or load level from a file */
  else
    sprintf(szTmp, "bomns %s", szFileName);

  /*g_free(szFileName);  causing crashes? */
  fprintf(stderr, "Launching bomns...\n");
  system(szTmp);
}


void
on_editor_button_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget * levelfile_textentry = lookup_widget(GTK_WIDGET(button), "levelfile_textentry");
  char * szFilename = (char *)gtk_entry_get_text(GTK_ENTRY(levelfile_textentry));
  char   szTmp[525] = {0};
  sprintf(szTmp, "bomnsedit %s", szFilename);
  system(szTmp);
}


void
on_defaults_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  set_defaults();
}


void
on_cancel_button_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget * level_select_dialog = gtk_widget_get_toplevel(GTK_WIDGET(button));
  gtk_widget_destroy(level_select_dialog);
}


void
on_open_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget * level_select_dialog = gtk_widget_get_toplevel(GTK_WIDGET(button));
  char *      szFileName          = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(level_select_dialog));
  GtkWidget * levelfile_textentry = lookup_widget(GTK_WIDGET(bfl_launcher_dialog), "levelfile_textentry");

  gtk_entry_set_text(GTK_ENTRY(levelfile_textentry), szFileName);

/*  g_free(szFileName);  causing crashes?*/
  gtk_widget_destroy(level_select_dialog);
}

