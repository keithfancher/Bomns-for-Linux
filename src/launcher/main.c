/* main.c
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


#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"
#include "callbacks.h"


int main (int argc, char * argv[])
{
  GtkWidget * dialog1;

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  dialog1 = create_dialog1 ();
  gtk_widget_show (dialog1);

  LoadSettings();

  gtk_signal_connect (GTK_OBJECT (dialog1), "delete_event", GTK_SIGNAL_FUNC (delete_event), NULL);
  gtk_signal_connect (GTK_OBJECT (dialog1), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
     
  gtk_main ();
  return 0;
}


