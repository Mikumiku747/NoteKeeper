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

gint fileMenuOpenCallback(GtkWidget *widget, gpointer calldata)
/* Opens a notebook for the user, builds XML tree, creates and binds
 * widgets, etc. */
{
	GtkWidget *openNotebookDialog;
	char *filename;
	GtkWidget **importantWidgets = (GtkWidget**)calldata;
	xmlDocPtr notebookDoc = NULL;
	xmlChar *sectionName;
	xmlChar *pageName;
	xmlChar *pageContent;
	xmlNode *notebook = NULL;
	xmlNode *section = NULL;
	xmlNode *page = NULL;
	xmlNode *field = NULL;
	GtkWidget *pageNotebook;
	GtkWidget *pageBox;
	GtkWidget *pageNameField;
	GtkEntryBuffer *pageNameBuffer;
	GtkWidget *pageContentTextView;
	GtkTextBuffer *pageContentBuffer;
	int isValidNotebook = FALSE;
	GtkWidget *loadErrorDialog;
	
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
				isValidNotebook = TRUE;
				g_printf("File extension is .notebook, loading into XML tree.");
				notebookDoc = xmlReadFile(filename, NULL, 0);
				if (notebookDoc != NULL) {
					/* Get the root node */
					notebook = xmlDocGetRootElement(notebookDoc);
					/* Get first section */
					section = NULL;
					/* Clear off the old pages */
					while (gtk_notebook_get_n_pages(GTK_NOTEBOOK(importantWidgets[1])) > 0) {
						gtk_notebook_remove_page(GTK_NOTEBOOK(importantWidgets[1]), 0);
					}
					/* Iterate through the section */
					for (section = notebook->children; section; section = section->next) {
						/* Build Each Section */
						if (strcmp(section->name, "Section") == 0) {
							/* Iterate through the pages for setup. */
							sectionName = NULL;
							pageNotebook = gtk_notebook_new();
							gtk_notebook_set_tab_pos(GTK_NOTEBOOK(pageNotebook), GTK_POS_LEFT);
							gtk_widget_show(pageNotebook);
							for (page = section->children; page; page = page->next) {
								if (strcmp(page->name, "Name") == 0) {
									/* Set the section Name */
									sectionName = xmlNodeGetContent(page);
								}
								pageName = NULL;
								pageContent = NULL;
								if (strcmp(page->name, "Page") == 0) {
									/* Build the page and add it to the
									 * section. */
									pageBox = gtk_vbox_new(FALSE, 0);
									for (field = page->children; field; field = field->next) {
										if (strcmp(field->name, "Name") == 0) {
											/* Set the Page Name */
											pageName = xmlNodeGetContent(field);
										}
										if (strcmp(field->name, "Content") == 0) {
											/* Set the Page Content */
											pageContent = xmlNodeGetContent(field);
										}
									}
									pageNameBuffer = gtk_entry_buffer_new(pageName, -1);
									pageNameField = gtk_entry_new_with_buffer(GTK_ENTRY_BUFFER(pageNameBuffer));
									pageContentBuffer = gtk_text_buffer_new(NULL);
									gtk_text_buffer_set_text(GTK_TEXT_BUFFER(pageContentBuffer), pageContent, -1);
									pageContentTextView = gtk_text_view_new_with_buffer(pageContentBuffer);
									gtk_widget_show(pageNameField);
									gtk_widget_show(pageContentTextView);
									gtk_box_pack_start(GTK_BOX(pageBox), pageNameField, FALSE, FALSE, 0);
									gtk_box_pack_start(GTK_BOX(pageBox), pageContentTextView, TRUE, TRUE, 0);
									gtk_widget_show(pageBox);
									gtk_notebook_append_page(GTK_NOTEBOOK(pageNotebook), pageBox, gtk_label_new(pageName));
									if (pageName != NULL) {xmlFree(pageName);}
									if (pageContent != NULL) {xmlFree(pageContent);}
								}
							}
							gtk_notebook_append_page(GTK_NOTEBOOK(importantWidgets[1]), pageNotebook, gtk_label_new(sectionName));
							if (sectionName != NULL) {xmlFree(sectionName);}
						} 
					}
					gtk_notebook_set_show_tabs(GTK_NOTEBOOK(importantWidgets[1]), TRUE);
					gtk_widget_show(importantWidgets[1]);
				}
				filename[i] = 0;
			}
		}
		if (isValidNotebook == FALSE) {
			loadErrorDialog = gtk_message_dialog_new (
				importantWidgets[0], GTK_DIALOG_DESTROY_WITH_PARENT, 
				GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
				"Failed to load notebook file from [%s]. Check that it \
exists and is a valid notebook file.", 
				filename);
			gtk_dialog_run (GTK_DIALOG (loadErrorDialog));
			gtk_widget_destroy (loadErrorDialog);
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
