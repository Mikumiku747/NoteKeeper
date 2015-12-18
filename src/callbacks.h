/*  callbacks.h - Event callbacks for the main program
    Copyright (C) 2015  Daniel Selmes

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALLBACKS_H
#define CALLBACKS_H

/* Includes */
#include <gtk/gtk.h>
#include "xml.h"
#include <stdlib.h>

gint topWindow_delete_event(GtkWidget *widget, GdkEvent *event, 
	gpointer data);

gint fileMenuOpenCallback(GtkWidget *widget, gpointer calldata);
/* Opens a notebook for the user, builds XML tree, creates and binds
 * widgets, etc. */
 
 gint pageNameChangedCallback (GtkEntry *entry, gpointer data);
/* Renames the page when the user edits the page name. */

gint pageContentChangedCallback (GtkTextBuffer *buffer, gpointer data);
/* Applies changes in the text to memory */

#endif
