/* callbacks.h
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


#include <gtk/gtk.h>


void
on_load_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_play_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_editor_button_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_defaults_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_quit_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancel_button_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_open_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);
