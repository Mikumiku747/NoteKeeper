/*  notekeeper.c - A program for taking notes, similar to Microsoft's
	OneNote. 
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

/* Library Includes */
#include <gtk/gtk.h>
#include <stdio.h>

/* Source Includes */
#include "callbacks.h"
#include "menu.h"
#include "xml.h"

/* Main */
int main(int argc, char *argv)
{
	/* Storage variables for some important widgets */
	GtkWidget *topWindow;
	GtkWidget *layoutBox;
	GtkWidget *menuBar;
	GtkWidget *sectionNotebook;
	GtkWidget *notOpenLabel;
	
	GtkWidget *important[2];
	
	/* Initialise GTK */
	gtk_init(&argc, &argv);	
	
	/* Initialise libxml */
	initXML();
	
	/* Set up the main window */
	topWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(topWindow), "delete_event", 
		G_CALLBACK(topWindow_delete_event), NULL);
	gtk_window_set_title(GTK_WINDOW(topWindow), "Notekeeper");
	
	/* Set up the vbox for UI layout */
	layoutBox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(topWindow), layoutBox);
	gtk_widget_show(layoutBox);
	
	/* Add the section notebook */
	sectionNotebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(sectionNotebook), 
		GTK_POS_TOP);
	gtk_box_pack_end(GTK_BOX(layoutBox), sectionNotebook, TRUE, TRUE, 0);
	gtk_widget_show(sectionNotebook);
	
	/* Add a default label in the notebook to inform the user they need
	 * to open one. Also, hide section labels for now. */
	notOpenLabel = gtk_label_new(
		"No notebook currently open, use the [File > Open] command to o\
pen a notebook."
		);
	gtk_label_set_line_wrap(GTK_LABEL(notOpenLabel), TRUE);
	gtk_widget_set_size_request(notOpenLabel, 400, 300);
	gtk_widget_show(notOpenLabel);
	gtk_notebook_append_page(GTK_NOTEBOOK(sectionNotebook), 
		notOpenLabel, NULL);
	gtk_notebook_set_show_tabs(GTK_NOTEBOOK(sectionNotebook), FALSE);
	
	/* Set up the menu */
	important[0] = topWindow;
	important[1] = sectionNotebook;
	menuBar = setupMenu(important);
	gtk_box_pack_start(GTK_BOX(layoutBox), menuBar, FALSE, FALSE, 0);
	
	/* UI is ready, render the window */
	gtk_widget_show(topWindow);
	
	/* Enter main event loop */
	gtk_main();
	
	/* Clean up the XML library */
	xmlCleanupParser();
	
	return 0;
}
