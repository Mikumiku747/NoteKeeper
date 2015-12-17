/*  callbacks.c - Event callbacks for the main program
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

#ifndef CALLBACKS_C
#define CALLBACKS_C

#include "callbacks.h"

gint topWindow_delete_event(GtkWidget *widget, GdkEvent *event, 
	gpointer data)
/* Exits the Application */
{
	gtk_main_quit();
	return FALSE;
}

gint fileMenuOpenCallback(GtkWidget *widget, GdkEvent *event, 
	gpointer calldata)
/* Opens a notebook for the user, builds XML tree, creates and binds
 * widgets, etc. */
{
	GtkWidget *openNotebookDialog;
	char *filename;
	GtkWidget **importantWidgets = (GtkWidget**)calldata;
	
	/* File dialog to select a notebook. */
	openNotebookDialog = gtk_file_chooser_dialog_new("Open Notebook...", 
		GTK_WINDOW(importantWidgets[0]), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, 
		GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);
	if (gtk_dialog_run (GTK_DIALOG (openNotebookDialog)) == GTK_RESPONSE_ACCEPT) {
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (openNotebookDialog));
		/* Debug print the filename */
		g_printf("Opening notebook: %s\n", filename);
		/* Check that it's at least NAMED *.notebook (even though it 
		 * might not really be a notebook file. */
		for (int i = 0; filename[i] != 0; i++) {
			if (strcmp(filename+i, ".notebook") == 0) {
				g_printf("File extension is .notebook, loading into XML tree.");
			}
		}
		g_printf("\n");
		/* Free the filename now that we're done with it. */
		g_free(filename);
	}
	/* Get rid of the Dialog Widget */
	gtk_widget_destroy(openNotebookDialog);
	return FALSE;
	
	
}

#endif
