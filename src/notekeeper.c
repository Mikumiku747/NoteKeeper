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

/* Source Includes */
#include "callbacks.h"

/* Main */
int main(int argc, char *argv)
{
	/* Storage variables for some important widgets */
	GtkWidget *topWindow;
	GtkWidget *layoutTable;
	GtkWidget *menuBar;
	GtkWidget *sectionNotebook;
	
	GtkWidget *exampleTextView;
	GtkTextBuffer *exampleTextBuffer;
	GtkWidget *exampleSection;
	
	
	/* Initialise GTK */
	gtk_init(&argc, &argv);	
	
	/* Set up the main window */
	topWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(topWindow), "delete_event", 
		G_CALLBACK(topWindow_delete_event), NULL);
	gtk_window_set_title(GTK_WINDOW(topWindow), "Notekeeper");
	
	/* Set up the table for UI layout */
	layoutTable = gtk_table_new(2, 1, FALSE);
	gtk_container_add(GTK_CONTAINER(topWindow), layoutTable);
	gtk_widget_show(layoutTable);
	
	/* Set up the menu */
	menuBar = setupMenu();
	gtk_table_attach_defaults(GTK_TABLE(layoutTable), menuBar, 
		0, 1, 0, 1);
	
	/* Create an example text view */
	exampleTextView = gtk_text_view_new();
	exampleTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(exampleTextView));
	gtk_widget_show(exampleTextView);
	
	/* Create an example section notebook */
	exampleSection = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(exampleSection), 
		GTK_POS_RIGHT);
	gtk_widget_show(exampleSection);
		
	/* Add the section notebook */
	sectionNotebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(sectionNotebook), 
		GTK_POS_TOP);
	gtk_table_attach_defaults(GTK_TABLE(layoutTable), sectionNotebook,
		0, 1, 1, 2);
	gtk_widget_show(sectionNotebook);
	
	/* Add the example text view to the example section */
	gtk_notebook_append_page(GTK_NOTEBOOK(exampleSection), exampleTextView, gtk_label_new("Example Page"));
	
	/* Add the example section to the section notebook */
	gtk_notebook_append_page(GTK_NOTEBOOK(sectionNotebook), exampleSection, gtk_label_new("Example Section"));
	
	
	
	
	/* UI is ready, render the window */
	gtk_widget_show(topWindow);
	
	/* Enter main event loop */
	gtk_main();
	
	return 0;
}
