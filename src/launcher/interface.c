/* interface.c
 * Copyright (C) 2001-2002 Keith Fancher <discostoo@punkass.com> 
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


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"


GtkWidget *dialog1;
GtkWidget *dialog_vbox1;
GtkWidget *vbox3;
GtkWidget *vbox3_2;
GtkWidget *hpaned1;
GtkWidget *vbox2;
GtkWidget *hbox9;
GtkWidget *label11;
GtkObject *spinbutton10_adj;
GtkWidget *spinbutton10;
GtkWidget *hbox11;
GtkWidget *label13;
GtkObject *spinbutton12_adj;
GtkWidget *spinbutton12;
GtkWidget *hbox13;
GtkWidget *label15;
GtkObject *spinbutton14_adj;
GtkWidget *spinbutton14;
GtkWidget *hbox15;
GtkWidget *label16;
GtkObject *spinbutton15_adj;
GtkWidget *spinbutton15;
GtkWidget *hbox17;
GtkWidget *label17;
GtkObject *spinbutton16_adj;
GtkWidget *spinbutton16;
GtkWidget *vbox3_3;
GtkWidget *hbox10;
GtkWidget *label12;
GtkObject *spinbutton11_adj;
GtkWidget *spinbutton11;
GtkWidget *hbox12;
GtkWidget *label14;
GtkObject *spinbutton13_adj;
GtkWidget *spinbutton13;
GtkWidget *hbox14;
GtkWidget *label19;
GtkObject *spinbutton18_adj;
GtkWidget *spinbutton18;
GtkWidget *hbox16;
GtkWidget *label20;
GtkObject *spinbutton19_adj;
GtkWidget *spinbutton19;
GtkWidget *hbox18;
GtkWidget *label21;
GtkObject *spinbutton20_adj;
GtkWidget *spinbutton20;
GtkWidget *hseparator2;
GtkWidget *hbox20;
GtkWidget *label22;
GtkObject *spinbutton21_adj;
GtkWidget *spinbutton21;
GtkWidget *hseparator3;
GtkWidget *hbox19;
GtkWidget *checkbutton1;
GtkWidget *checkbutton2;
GtkWidget *dialog_action_area1;
GtkWidget *hbox1;
GtkWidget *button1;
GtkWidget *button3;
GtkWidget *button2;
GtkTooltips *tooltips;



GtkWidget*
create_dialog1 (void)
{
  tooltips = gtk_tooltips_new ();

  dialog1 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog1", dialog1);
  gtk_widget_set_usize (dialog1, 305, 229);
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Bomns for Linux Launcher"));
  GTK_WINDOW (dialog1)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW (dialog1), 305, 229);
  gtk_window_set_policy (GTK_WINDOW (dialog1), FALSE, FALSE, FALSE);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog_vbox1", dialog_vbox1);
  gtk_widget_show (dialog_vbox1);
  gtk_widget_set_usize (dialog_vbox1, 288, 202);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox3);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "vbox3", vbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox3);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox3, TRUE, TRUE, 0);
  gtk_widget_set_usize (vbox3, 288, 158);

  vbox3_2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox3_2);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "vbox3_2", vbox3_2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox3_2);
  gtk_box_pack_start (GTK_BOX (vbox3), vbox3_2, TRUE, TRUE, 0);
  gtk_widget_set_usize (vbox3_2, 288, 134);

  hpaned1 = gtk_hpaned_new ();
  gtk_widget_ref (hpaned1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hpaned1", hpaned1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hpaned1);
  gtk_box_pack_start (GTK_BOX (vbox3_2), hpaned1, FALSE, TRUE, 0);
  gtk_widget_set_usize (hpaned1, 288, 132);
  gtk_container_set_border_width (GTK_CONTAINER (hpaned1), 6);
  gtk_paned_set_handle_size (GTK_PANED (hpaned1), 5);
  gtk_paned_set_gutter_size (GTK_PANED (hpaned1), 24);
  gtk_paned_set_position (GTK_PANED (hpaned1), 140);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox2);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "vbox2", vbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox2);
  gtk_paned_pack1 (GTK_PANED (hpaned1), vbox2, FALSE, TRUE);
  gtk_widget_set_usize (vbox2, 134, 120);

  hbox9 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox9);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox9", hbox9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox9);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox9, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox9, 77, 24);

  label11 = gtk_label_new (_("Walls"));
  gtk_widget_ref (label11);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label11", label11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label11);
  gtk_box_pack_start (GTK_BOX (hbox9), label11, TRUE, FALSE, 0);
  gtk_widget_set_usize (label11, 30, 16);

  spinbutton10_adj = gtk_adjustment_new (900, 0, 999, 1, 10, 10);
  spinbutton10 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton10_adj), 1, 0);
  gtk_widget_ref (spinbutton10);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton10", spinbutton10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton10);
  gtk_box_pack_start (GTK_BOX (hbox9), spinbutton10, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton10, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton10), GTK_UPDATE_IF_VALID);*/

  hbox11 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox11);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox11", hbox11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox11);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox11, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox11, 102, 24);

  label13 = gtk_label_new (_("Powerups"));
  gtk_widget_ref (label13);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label13", label13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label13);
  gtk_box_pack_start (GTK_BOX (hbox11), label13, TRUE, FALSE, 0);
  gtk_widget_set_usize (label13, 55, 16);

  spinbutton12_adj = gtk_adjustment_new (50, 0, 999, 1, 10, 10);
  spinbutton12 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton12_adj), 1, 0);
  gtk_widget_ref (spinbutton12);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton12", spinbutton12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton12);
  gtk_box_pack_start (GTK_BOX (hbox11), spinbutton12, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton12, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton12), GTK_UPDATE_IF_VALID); */

  hbox13 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox13);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox13", hbox13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox13);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox13, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox13, 134, 24);

  label15 = gtk_label_new (_("Invulnerabilities"));
  gtk_widget_ref (label15);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label15", label15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label15);
  gtk_box_pack_start (GTK_BOX (hbox13), label15, TRUE, FALSE, 0);
  gtk_widget_set_usize (label15, 87, 16);
  gtk_label_set_justify (GTK_LABEL (label15), GTK_JUSTIFY_FILL);

  spinbutton14_adj = gtk_adjustment_new (10, 0, 999, 1, 10, 10);
  spinbutton14 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton14_adj), 1, 0);
  gtk_widget_ref (spinbutton14);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton14", spinbutton14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton14);
  gtk_box_pack_start (GTK_BOX (hbox13), spinbutton14, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton14, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton14), GTK_UPDATE_IF_VALID); */

  hbox15 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox15);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox15", hbox15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox15);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox15, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox15, 82, 24);

  label16 = gtk_label_new (_("Warps"));
  gtk_widget_ref (label16);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label16", label16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label16);
  gtk_box_pack_start (GTK_BOX (hbox15), label16, TRUE, FALSE, 0);

  spinbutton15_adj = gtk_adjustment_new (2, 0, 999, 1, 10, 10);
  spinbutton15 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton15_adj), 1, 0);
  gtk_widget_ref (spinbutton15);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton15", spinbutton15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton15);
  gtk_box_pack_start (GTK_BOX (hbox15), spinbutton15, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton15, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton15), GTK_UPDATE_IF_VALID); */

  hbox17 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox17);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox17", hbox17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox17);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox17, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox17, 103, 24);

  label17 = gtk_label_new (_("P1 Bomns"));
  gtk_widget_ref (label17);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label17", label17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label17);
  gtk_box_pack_start (GTK_BOX (hbox17), label17, TRUE, FALSE, 0);

  spinbutton16_adj = gtk_adjustment_new (10, 0, 99, 1, 10, 10);
  spinbutton16 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton16_adj), 1, 0);
  gtk_widget_ref (spinbutton16);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton16", spinbutton16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton16);
  gtk_box_pack_start (GTK_BOX (hbox17), spinbutton16, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton16, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton16), GTK_UPDATE_IF_VALID); */

  vbox3_3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox3_3);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "vbox3_3", vbox3_3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox3_3);
  gtk_paned_pack2 (GTK_PANED (hpaned1), vbox3_3, TRUE, TRUE);
  gtk_widget_set_usize (vbox3_3, 118, 120);

  hbox10 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox10);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox10", hbox10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox10);
  gtk_box_pack_start (GTK_BOX (vbox3_3), hbox10, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox10, 84, 24);

  label12 = gtk_label_new (_("Bomns"));
  gtk_widget_ref (label12);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label12", label12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label12);
  gtk_box_pack_start (GTK_BOX (hbox10), label12, TRUE, FALSE, 0);
  gtk_widget_set_usize (label12, 37, 16);

  spinbutton11_adj = gtk_adjustment_new (20, 0, 999, 1, 10, 10);
  spinbutton11 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton11_adj), 1, 0);
  gtk_widget_ref (spinbutton11);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton11", spinbutton11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton11);
  gtk_box_pack_start (GTK_BOX (hbox10), spinbutton11, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton11, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton11), GTK_UPDATE_IF_VALID); */

  hbox12 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox12);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox12", hbox12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox12);
  gtk_box_pack_start (GTK_BOX (vbox3_3), hbox12, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox12, 118, 24);

  label14 = gtk_label_new (_("Powerdowns"));
  gtk_widget_ref (label14);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label14", label14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label14);
  gtk_box_pack_start (GTK_BOX (hbox12), label14, TRUE, FALSE, 0);
  gtk_widget_set_usize (label14, 71, 16);

  spinbutton13_adj = gtk_adjustment_new (10, 0, 999, 1, 10, 10);
  spinbutton13 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton13_adj), 1, 0);
  gtk_widget_ref (spinbutton13);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton13", spinbutton13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton13);
  gtk_box_pack_start (GTK_BOX (hbox12), spinbutton13, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton13, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton13), GTK_UPDATE_IF_VALID); */

  hbox14 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox14);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox14", hbox14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox14);
  gtk_box_pack_start (GTK_BOX (vbox3_3), hbox14, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox14, 83, 24);

  label19 = gtk_label_new (_("Health"));
  gtk_widget_ref (label19);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label19", label19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label19);
  gtk_box_pack_start (GTK_BOX (hbox14), label19, TRUE, FALSE, 0);
  gtk_widget_set_usize (label19, 36, 16);

  spinbutton18_adj = gtk_adjustment_new (10, 0, 999, 1, 10, 10);
  spinbutton18 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton18_adj), 1, 0);
  gtk_widget_ref (spinbutton18);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton18", spinbutton18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton18);
  gtk_box_pack_start (GTK_BOX (hbox14), spinbutton18, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton18, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton18), GTK_UPDATE_IF_VALID); */

  hbox16 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox16);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox16", hbox16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox16);
  gtk_box_pack_start (GTK_BOX (vbox3_3), hbox16, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox16, 76, 24);

  label20 = gtk_label_new (_("Seed"));
  gtk_widget_ref (label20);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label20", label20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label20);
  gtk_box_pack_start (GTK_BOX (hbox16), label20, TRUE, FALSE, 0);
  gtk_widget_set_usize (label20, 29, 16);

  spinbutton19_adj = gtk_adjustment_new (0, 0, 9999, 1, 10, 10);
  spinbutton19 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton19_adj), 1, 0);
  gtk_widget_ref (spinbutton19);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton19", spinbutton19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton19);
  gtk_box_pack_start (GTK_BOX (hbox16), spinbutton19, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton19, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton19), GTK_UPDATE_IF_VALID); */

  hbox18 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox18);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox18", hbox18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox18);
  gtk_box_pack_start (GTK_BOX (vbox3_3), hbox18, TRUE, TRUE, 0);
  gtk_widget_set_usize (hbox18, 103, 24);

  label21 = gtk_label_new (_("P2 Bomns"));
  gtk_widget_ref (label21);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label21", label21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label21);
  gtk_box_pack_start (GTK_BOX (hbox18), label21, TRUE, FALSE, 0);

  spinbutton20_adj = gtk_adjustment_new (10, 0, 99, 1, 10, 10);
  spinbutton20 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton20_adj), 1, 0);
  gtk_widget_ref (spinbutton20);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton20", spinbutton20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton20);
  gtk_box_pack_end (GTK_BOX (hbox18), spinbutton20, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton20, 47, 24);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton20), GTK_UPDATE_IF_VALID); */

  hseparator2 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator2);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hseparator2", hseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator2);
  gtk_box_pack_start (GTK_BOX (vbox3_2), hseparator2, FALSE, TRUE, 0);

  hbox20 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox20);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox20", hbox20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox20);
  gtk_box_pack_start (GTK_BOX (vbox3_2), hbox20, TRUE, TRUE, 0);

  label22 = gtk_label_new (_("Match Length:"));
  gtk_widget_ref (label22);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label22", label22,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label22);
  gtk_box_pack_start (GTK_BOX (hbox20), label22, FALSE, TRUE, 9);
  gtk_widget_set_usize (label22, 144, -2);
  gtk_label_set_justify (GTK_LABEL (label22), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC (label22), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label22), 59, 0);

  spinbutton21_adj = gtk_adjustment_new (100, 0, 999, 1, 10, 10);
  spinbutton21 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton21_adj), 1, 0);
  gtk_widget_ref (spinbutton21);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "spinbutton21", spinbutton21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton21);
  gtk_box_pack_start (GTK_BOX (hbox20), spinbutton21, FALSE, FALSE, 0);
  gtk_widget_set_usize (spinbutton21, 78, -2);
/*  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton21), GTK_UPDATE_IF_VALID); */

  hseparator3 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator3);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hseparator3", hseparator3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator3);
  gtk_box_pack_start (GTK_BOX (vbox3_2), hseparator3, FALSE, TRUE, 0);

  hbox19 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox19);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox19", hbox19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox19);
  gtk_box_pack_start (GTK_BOX (vbox3), hbox19, FALSE, FALSE, 0);

  checkbutton1 = gtk_check_button_new_with_label (_("Fullscreen"));
  gtk_widget_ref (checkbutton1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "checkbutton1", checkbutton1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton1);
  gtk_box_pack_start (GTK_BOX (hbox19), checkbutton1, TRUE, FALSE, 25);
  gtk_widget_set_usize (checkbutton1, 86, 24);
  gtk_tooltips_set_tip (tooltips, checkbutton1, _("Use fullscreen mode"), NULL);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton1), TRUE);

  checkbutton2 = gtk_check_button_new_with_label (_("Sound"));
  gtk_widget_ref (checkbutton2);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "checkbutton2", checkbutton2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton2);
  gtk_box_pack_start (GTK_BOX (hbox19), checkbutton2, TRUE, FALSE, 0);
  gtk_widget_set_usize (checkbutton2, 86, 24);
  gtk_tooltips_set_tip (tooltips, checkbutton2, _("Play sounds"), NULL);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton2), TRUE);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog_action_area1", dialog_action_area1);
  gtk_widget_show (dialog_action_area1);
  gtk_widget_set_usize (dialog_action_area1, 140, 42);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

  hbox1 = gtk_hbox_new (FALSE, 2);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), hbox1, FALSE, FALSE, 0);

  button1 = gtk_button_new_with_label (_("Launch Bomns"));
  gtk_widget_ref (button1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "button1", button1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_signal_connect (GTK_OBJECT (button1), "clicked",
                                 GTK_SIGNAL_FUNC (LaunchBomns), NULL);
  gtk_widget_show (button1);
  gtk_box_pack_start (GTK_BOX (hbox1), button1, FALSE, FALSE, 0);
  gtk_widget_set_usize (button1, 89, 22);
  gtk_tooltips_set_tip (tooltips, button1, _("Start the game!"), NULL);

  button3 = gtk_button_new_with_label (_("Set Defaults"));
  gtk_widget_ref (button3);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "button3", button3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_signal_connect (GTK_OBJECT (button3), "clicked",
                                  GTK_SIGNAL_FUNC (SetDefaults), NULL);
  gtk_widget_show (button3);
  gtk_box_pack_start (GTK_BOX (hbox1), button3, FALSE, FALSE, 0);
  gtk_widget_set_usize (button3, 89, 22);
  gtk_tooltips_set_tip (tooltips, button3, _("Reset to default settings"), NULL);

  button2 = gtk_button_new_with_label (_("Quit"));
  gtk_widget_ref (button2);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "button2", button2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_signal_connect (GTK_OBJECT (button2), "clicked",
                           GTK_SIGNAL_FUNC (destroy), NULL);
  gtk_widget_show (button2);
  gtk_box_pack_start (GTK_BOX (hbox1), button2, FALSE, FALSE, 0);
  gtk_widget_set_usize (button2, 89, 22);
  gtk_tooltips_set_tip (tooltips, button2, _("Get outta here"), NULL);

  gtk_object_set_data (GTK_OBJECT (dialog1), "tooltips", tooltips);

  return dialog1;
}

