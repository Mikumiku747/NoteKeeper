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
#include <stdio.h>
#include <string.h>

gint topWindow_delete_event(GtkWidget *widget, gpointer data);

gint fileMenuOpenCallback(GtkWidget *widget, gpointer calldata);
/* Opens a notebook for the user, builds XML tree, creates and binds
 * widgets, etc. */
 
gint pageNameChangedCallback (GtkEntry *entry, gpointer data);
/* Renames the page when the user edits the page name. */

gint pageContentChangedCallback (GtkTextBuffer *buffer, gpointer data);
/* Applies changes in the text to memory */

gint fileMenuSaveCallback(GtkWidget *widget, gpointer data);
/* Saves the document currently in memory to a file. */

void helpMenuAboutCallback(GtkWidget *widget, gpointer data);
/* Shows the about box, which has information about the program. */

void pageMenuNewCallback(GtkWidget *widget, gpointer data);
/* Creates a new page and adds it to the current section. */

void pageMenuRemoveCallback(GtkWidget *widget, gpointer data);
/* Deletes the current page, from both the notebook and XML doc. */

void sectionMenuNewCallback(GtkWidget *widget, gpointer data);
/* Create a new section and append it to the document. */

void sectionMenuRemoveCallback(GtkWidget *widget, gpointer data);
/* Removes a section from the notebook. */

#endif
