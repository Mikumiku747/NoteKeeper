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

gint topWindow_delete_event(GtkWidget *widget, gpointer data)
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
	xmlNode *sectionNameNode = NULL;
	xmlNode *page = NULL;
	xmlNode *field = NULL;
	xmlNode *pageNameNode = NULL;
	xmlNode *pageContentNode = NULL;
	GtkWidget *pageNotebook;
	GtkWidget *pageBox;
	GtkWidget *pageNameField;
	GtkWidget *pageScrollWindow;
	GtkEntryBuffer *pageNameBuffer;
	GtkWidget *pageContentTextView;
	GtkTextBuffer *pageContentBuffer;
	int isValidNotebook = FALSE;
	GtkWidget *loadErrorDialog;
	GtkWidget **callbackRefs;
	GtkFileFilter *notebookFilter;
	
	/* File dialog to select a notebook. */
	openNotebookDialog = gtk_file_chooser_dialog_new("Open Notebook...", 
		GTK_WINDOW(importantWidgets[0]), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, 
		GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);
	notebookFilter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(notebookFilter, "*.notebook");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(openNotebookDialog), notebookFilter);
	if (gtk_dialog_run (GTK_DIALOG (openNotebookDialog)) == GTK_RESPONSE_ACCEPT) {
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (openNotebookDialog));
		/* Debug print the filename */
		printf("Opening notebook: %s\n", filename);
		/* Check that it's at least NAMED *.notebook (even though it 
		 * might not really be a notebook file. */
		for (int i = 0; filename[i] != 0; i++) {
			if (strcmp(filename+i, ".notebook") == 0) {
				isValidNotebook = TRUE;
				printf("File extension is .notebook, loading into XML tree.");
				notebookDoc = xmlReadFile(filename, NULL, 0);
				if (notebookDoc != NULL) {
					/* Free an existing xmldoc if one exists. */
					if (g_object_get_data(G_OBJECT(importantWidgets[1]), "xmlDocument") != NULL) {
						xmlFreeDoc((xmlDocPtr)g_object_get_data(G_OBJECT(importantWidgets[1]), "xmlDocument"));
					}
					/* Attach the doc to the section notebook */
					g_object_set_data(G_OBJECT(importantWidgets[1]), "xmlDocument", (gpointer)notebookDoc);
					/* Attach the current filename to the topwindow. */
					g_object_set_data(G_OBJECT(importantWidgets[0]), "currentFile", (gpointer)filename);
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
							/* Keep a reference to the current section node. */
							g_object_set_data(G_OBJECT(pageNotebook), "xmlNodeSection", (gpointer)section);
							gtk_notebook_set_tab_pos(GTK_NOTEBOOK(pageNotebook), GTK_POS_LEFT);
							gtk_widget_show(pageNotebook);
							for (page = section->children; page; page = page->next) {
								if (strcmp(page->name, "Name") == 0) {
									/* Set the section Name */
									sectionName = xmlNodeGetContent(page);
									/* Keep a reference to the section name node */
									g_object_set_data(G_OBJECT(pageNotebook), "xmlNodeName", (gpointer)page);
								}
								pageName = NULL;
								pageContent = NULL;
								if (strcmp(page->name, "Page") == 0) {
									/* Build the page and add it to the
									 * section. */
									for (field = page->children; field; field = field->next) {
										if (strcmp(field->name, "Name") == 0) {
											/* Set the Page Name */
											pageName = xmlNodeGetContent(field);
											/* Keep a reference to the page name node. */
											pageNameNode = field;
										}
										if (strcmp(field->name, "Content") == 0) {
											/* Set the Page Content */
											pageContent = xmlNodeGetContent(field);
											/* Keep a reference to the page content node. */
											pageContentNode = field;
										}
									}
									/* Create the page widgets */
									pageBox = gtk_vbox_new(FALSE, 0);
									g_object_set_data(G_OBJECT(pageBox), "xmlNodePage", (gpointer)page);
									pageNameBuffer = gtk_entry_buffer_new(pageName, -1);
									pageNameField = gtk_entry_new_with_buffer(GTK_ENTRY_BUFFER(pageNameBuffer));
									gtk_widget_modify_font(pageNameField, pango_font_description_from_string("Sans 16"));
									g_object_set_data(G_OBJECT(pageNameField), "xmlNodeName", (gpointer)pageNameNode);
									pageContentBuffer = gtk_text_buffer_new(NULL);
									gtk_text_buffer_set_text(GTK_TEXT_BUFFER(pageContentBuffer), pageContent, -1);
									pageContentTextView = gtk_text_view_new_with_buffer(pageContentBuffer);
									gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(pageContentTextView), GTK_WRAP_WORD_CHAR);
									g_object_set_data(G_OBJECT(pageContentBuffer), "xmlNodeContent", (gpointer)pageContentNode);
									pageScrollWindow = gtk_scrolled_window_new(NULL, NULL);
									gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pageScrollWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
									gtk_container_add(GTK_CONTAINER(pageScrollWindow), pageContentTextView);
									gtk_widget_show(pageScrollWindow);
									/* Connect them to callbacks. */
									callbackRefs = (GtkWidget **)malloc(sizeof(GtkWidget *)*2);
									callbackRefs[0] = pageBox;
									callbackRefs[1] = pageNotebook;
									g_signal_connect(G_OBJECT(pageNameField), "activate", G_CALLBACK(pageNameChangedCallback), (gpointer)callbackRefs);
									g_signal_connect(G_OBJECT(pageContentBuffer), "changed", G_CALLBACK(pageContentChangedCallback), NULL);
									/* Finalise */
									gtk_widget_show(pageNameField);
									gtk_widget_show(pageContentTextView);
									gtk_box_pack_start(GTK_BOX(pageBox), pageNameField, FALSE, FALSE, 0);
									gtk_box_pack_start(GTK_BOX(pageBox), pageScrollWindow, TRUE, TRUE, 0);
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
				GTK_WINDOW(importantWidgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
				GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
				"Failed to load notebook file from [%s]. Check that it \
exists and is a valid notebook file.", 
				filename);
			gtk_dialog_run (GTK_DIALOG (loadErrorDialog));
			gtk_widget_destroy (loadErrorDialog);
		}
		printf("\n");
		/* Free the filename now that we're done with it. */
		g_free(filename);
	}
	/* Get rid of the Dialog Widget */
	gtk_widget_destroy(openNotebookDialog);
	return FALSE;
}

gint pageNameChangedCallback (GtkEntry *entry, gpointer data)
/* Renames the page when the user edits the page name. */
{
	/* Get a reference to the page(0) and notebook(1). */
	GtkWidget **widgets = (GtkWidget **)data;
	/* Get a reference to the node. */
	xmlNode *pageNameNode = g_object_get_data(G_OBJECT(entry), "xmlNodeName");
	
	/* Update the node */
	xmlNodeSetContent(pageNameNode, gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(gtk_entry_get_buffer(GTK_ENTRY(entry)))));
	
	/* Update the page Label */
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(widgets[1]), widgets[0], gtk_label_new(gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(gtk_entry_get_buffer(GTK_ENTRY(entry))))));
}

gint pageContentChangedCallback (GtkTextBuffer *buffer, gpointer data)
/* Applies changes in the text to memory */
{
	/* Get a reference to the node. */
	xmlNode *pageContentNode = (xmlNode *)g_object_get_data(G_OBJECT(buffer), "xmlNodeContent");
	
	/* Save the text to the node's content */
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	xmlNodeSetContent(pageContentNode, gtk_text_buffer_get_text(buffer, &start, &end, TRUE));
}

gint fileMenuSaveCallback(GtkWidget *widget, gpointer data)
/* Saves the document currently in memory to a file. */
{
	GtkWidget *saveErrorDialog;
	GtkWidget *saveChooserDialog;
	GtkFileFilter *notebookFilter;
	char *filename;
	int magic;
	FILE *savefile = NULL;
	
	/* Get a reference to the window(0) and secion notebook(1). */
	GtkWidget** widgets = (GtkWidget **)data;
	/* Get a reference to the xml document. */
	xmlDocPtr notebookDoc = (xmlDocPtr)g_object_get_data(G_OBJECT(widgets[1]), "xmlDocument");
	/* Check the document loaded properly. */
	if (notebookDoc != NULL) {
		/* Open a file dialog so we can save. */
		saveChooserDialog = gtk_file_chooser_dialog_new("Save Notebook...", 
			GTK_WINDOW(widgets[0]), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, 
			GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL);
		notebookFilter = gtk_file_filter_new();
		gtk_file_filter_add_pattern(notebookFilter, "*.notebook");
		gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(saveChooserDialog), notebookFilter);
		/* Check to see if there's a filename we should default to. */
		char *defaultName = g_object_get_data(G_OBJECT(widgets[0]), "currentFile");
		if (defaultName != NULL && defaultName[0] == '/') {
			gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(saveChooserDialog), defaultName);
		}
		if (gtk_dialog_run (GTK_DIALOG (saveChooserDialog)) == GTK_RESPONSE_ACCEPT) {
			filename = (char *)gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (saveChooserDialog));
			savefile = fopen(filename, "w");
			magic = (int)savefile;
			if (savefile =! NULL) {
				/*printf("File pointer: %d\n", (FILE *)magic);*/
				xmlDocDump((FILE *)magic, notebookDoc);
			} else {
				saveErrorDialog = gtk_message_dialog_new (
					GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
					GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
					"Unable to save notebook: Failed to open file [%s]", 
					filename);
				gtk_dialog_run (GTK_DIALOG (saveErrorDialog));
				gtk_widget_destroy (saveErrorDialog);
			}
		}
		gtk_widget_destroy(saveChooserDialog);
		g_free(filename);
	} else {
		saveErrorDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"Failed to save notebook: Error loading in-memory document. \
			Current Value: %d", 
			notebookDoc);
		gtk_dialog_run (GTK_DIALOG (saveErrorDialog));
		gtk_widget_destroy (saveErrorDialog);
	}
}

void helpMenuAboutCallback(GtkWidget *widget, gpointer data)
/* Shows the about box, which has information about the program. */
{
	gtk_show_about_dialog (NULL,
		"program-name", "Notekeeper",
		"title", "About Notekeeper",
		"copyright", "Copyright (C) 2015  Daniel Selmes", 
		"comments", "Notekeeper is a notebook document editor, inspired\
 by Microsoft OneNote. It's for keeping a series of text documents\
 organised and together in a single file. Really, I just wanted to\
 learn a widget library in linux, and this seems like something I could\
 make that would be useful.",
		NULL);
}

void pageMenuNewCallback(GtkWidget *widget, gpointer data)
/* Creates a new page and adds it to the current section. */
{
	xmlNode *sectionNode = NULL;
	GtkWidget **widgets;
	GtkWidget *noDocumentErrorDialog;
	GtkWidget *noSectionErrorDialog;
	GtkWidget *pageNotebook;
	xmlNode *pageNode = NULL;
	xmlNode *pageNameNode = NULL;
	xmlNode *pageContentNode = NULL;
	GtkWidget *pageBox;
	GtkEntryBuffer *pageNameBuffer;
	GtkWidget *pageNameField;
	GtkTextBuffer *pageContentBuffer;
	GtkWidget *pageContentTextView;
	GtkWidget *pageScrollWindow;
	GtkWidget **callbackRefs;
	
	/* Get a reference to the window(0), section notebook(1). */
	widgets = (GtkWidget **)data;
	/* Check if there's a loaded document. */
	if (g_object_get_data(G_OBJECT(widgets[1]), "xmlDocument") == NULL) {
		noDocumentErrorDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"You need to have a document open to add a page.");
		gtk_dialog_run (GTK_DIALOG (noDocumentErrorDialog));
		gtk_widget_destroy (noDocumentErrorDialog);
		return; 
	}
	/* Make sure there's a section to put it into! */
	if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(widgets[1])) < 1) {
		noSectionErrorDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"There's no section to put a page into, you need to create \
a section first.");
		gtk_dialog_run (GTK_DIALOG (noSectionErrorDialog));
		gtk_widget_destroy (noSectionErrorDialog);
		return;
	}
 	/* Get a reference to the currently open section. */
	pageNotebook = gtk_notebook_get_nth_page(
		GTK_NOTEBOOK(widgets[1]), 
		gtk_notebook_get_current_page(GTK_NOTEBOOK(widgets[1])));
	/* Get a reference to the section node for the current section. */
	sectionNode = (xmlNode *)g_object_get_data(G_OBJECT(pageNotebook), "xmlNodeSection");
	/* Create the new XML page node and children. */
	pageNode = xmlNewChild(sectionNode, NULL, "Page", NULL);
	pageNameNode = xmlNewChild(pageNode, NULL, "Name", "Untitled Page");
	pageContentNode = xmlNewChild(pageNode, NULL, "Content", "Start by entering some text here!");
	/* Create the new widgets to hold the page. */
	pageBox = gtk_vbox_new(FALSE, 0);
	pageNameBuffer = gtk_entry_buffer_new("Untitled Page", -1);
	pageNameField = gtk_entry_new_with_buffer(GTK_ENTRY_BUFFER(pageNameBuffer));
	gtk_widget_modify_font(pageNameField, pango_font_description_from_string("Sans 16"));
	g_object_set_data(G_OBJECT(pageNameField), "xmlNodeName", (gpointer)pageNameNode);
	pageContentBuffer = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(pageContentBuffer), "Start by entering some text here!", -1);
	pageContentTextView = gtk_text_view_new_with_buffer(pageContentBuffer);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(pageContentTextView), GTK_WRAP_WORD_CHAR);
	g_object_set_data(G_OBJECT(pageContentBuffer), "xmlNodeContent", (gpointer)pageContentNode);
	pageScrollWindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pageScrollWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(pageScrollWindow), pageContentTextView);
	gtk_widget_show(pageScrollWindow);
	/* Connect callbacks for the widgets. */
	callbackRefs = (GtkWidget **)malloc(sizeof(GtkWidget *)*2);
	callbackRefs[0] = pageBox;
	callbackRefs[1] = pageNotebook;
	g_signal_connect(G_OBJECT(pageNameField), "activate", G_CALLBACK(pageNameChangedCallback), (gpointer)callbackRefs);
	g_signal_connect(G_OBJECT(pageContentBuffer), "changed", G_CALLBACK(pageContentChangedCallback), NULL);
	/* Finalize widget creation. */
	gtk_widget_show(pageNameField);
	gtk_widget_show(pageContentTextView);
	gtk_box_pack_start(GTK_BOX(pageBox), pageNameField, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(pageBox), pageScrollWindow, TRUE, TRUE, 0);
	gtk_widget_show(pageBox);
	/* Add the new page to the page notebook. */
	gtk_notebook_append_page(GTK_NOTEBOOK(pageNotebook), pageBox, gtk_label_new("Untitled Page"));

}

void pageMenuRemoveCallback(GtkWidget *widget, gpointer data)
/* Deletes the current page, from both the notebook and XML doc. */
{
	GtkWidget **widgets;
	GtkWidget *pageNotebook;
	GtkWidget *pageBox;
	GtkWidget *noDocumentErrorDialog;
	xmlNode *pageNode = NULL;
	
	/* Get a reference to the window(0) and section notebook(1). */
	widgets = (GtkWidget **)data;
	/* Check if there's a loaded document. */
	if (g_object_get_data(G_OBJECT(widgets[1]), "xmlDocument") == NULL) {
		noDocumentErrorDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"You need to have a document open to delete a page.");
		gtk_dialog_run (GTK_DIALOG (noDocumentErrorDialog));
		gtk_widget_destroy (noDocumentErrorDialog);
		return; 
	}
	/* Get a reference to the current section. */
	pageNotebook = gtk_notebook_get_nth_page(
		GTK_NOTEBOOK(widgets[1]), 
		gtk_notebook_get_current_page(GTK_NOTEBOOK(widgets[1])));
	/* Make sure there's a page to delete. */
	if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(pageNotebook)) < 1) {
		noDocumentErrorDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"There are no more pages to delete.");
		gtk_dialog_run (GTK_DIALOG (noDocumentErrorDialog));
		gtk_widget_destroy (noDocumentErrorDialog);
		return; 
	}
	/* Get a reference to the current page. */
	pageBox = gtk_notebook_get_nth_page(
		GTK_NOTEBOOK(pageNotebook), 
		gtk_notebook_get_current_page(GTK_NOTEBOOK(pageNotebook)));
	/* Get a reference to the page's XML node. */
	pageNode = g_object_get_data(G_OBJECT(pageBox), "xmlNodePage");
	/* Remove the page, destroy the widget. */
	gtk_notebook_remove_page(GTK_NOTEBOOK(pageNotebook), 
		gtk_notebook_get_current_page(GTK_NOTEBOOK(pageNotebook)));
	/* Unlink the XML node from the document, and free (destroy) it. */
	xmlUnlinkNode(pageNode);
	xmlFreeNode(pageNode);
		
}

void sectionMenuNewCallback(GtkWidget *widget, gpointer data)
/* Create a new section and append it to the document. */
{
	GtkWidget **widgets;
	GtkWidget *noDocumentErrorDialog;
	xmlDocPtr notebookDoc = NULL;
	xmlNode *notebook = NULL;
	xmlNode *sectionNode = NULL;
	xmlNode *sectionNameNode = NULL;
	GtkWidget *pageNotebook;
	xmlNode *pageNode = NULL;
	xmlNode *pageNameNode = NULL;
	xmlNode *pageContentNode = NULL;
	GtkWidget *pageBox;
	GtkEntryBuffer *pageNameBuffer;
	GtkWidget *pageNameField;
	GtkTextBuffer *pageContentBuffer;
	GtkWidget *pageContentTextView;
	GtkWidget *pageScrollWindow;
	GtkWidget **callbackRefs;
	
	/* Get a pointer to the window(0) and section notebook(1). */
	widgets = (GtkWidget **)data;
	/* Check if there's a loaded document. */
	if (g_object_get_data(G_OBJECT(widgets[1]), "xmlDocument") == NULL) {
		noDocumentErrorDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"You need to have a document open to create a new section.");
		gtk_dialog_run (GTK_DIALOG (noDocumentErrorDialog));
		gtk_widget_destroy (noDocumentErrorDialog);
		return; 
	}
	/* Get a reference to the XML doc. */
	notebookDoc = g_object_get_data(G_OBJECT(widgets[1]), "xmlDocument");
	/* Get a reference to the root element. */
	notebook = xmlDocGetRootElement(notebookDoc);
	/* Create the new section, with a name node and child page node. */
	sectionNode = xmlNewChild(notebook, NULL, "Section", NULL);
	sectionNameNode = xmlNewChild(sectionNode, NULL, "Name", "Untitled Section");
	pageNode = xmlNewChild(sectionNode, NULL, "Page", NULL);
	pageNameNode = xmlNewChild(pageNode, NULL, "Name", "Untitled Page");
	pageContentNode = xmlNewChild(pageNode, NULL, "Content", "Start by entering some text here!");
	/* Create the widgets, set them up, and link in node references. */
	pageBox = gtk_vbox_new(FALSE, 0);
	pageNameBuffer = gtk_entry_buffer_new("Untitled Page", -1);
	pageNameField = gtk_entry_new_with_buffer(GTK_ENTRY_BUFFER(pageNameBuffer));
	gtk_widget_modify_font(pageNameField, pango_font_description_from_string("Sans 16"));
	g_object_set_data(G_OBJECT(pageNameField), "xmlNodeName", (gpointer)pageNameNode);
	pageContentBuffer = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(pageContentBuffer), "Start by entering some text here!", -1);
	pageContentTextView = gtk_text_view_new_with_buffer(pageContentBuffer);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(pageContentTextView), GTK_WRAP_WORD_CHAR);
	g_object_set_data(G_OBJECT(pageContentBuffer), "xmlNodeContent", (gpointer)pageContentNode);
	pageScrollWindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pageScrollWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(pageScrollWindow), pageContentTextView);
	gtk_widget_show(pageScrollWindow);
	/* Connect callbacks for the widgets. */
	callbackRefs = (GtkWidget **)malloc(sizeof(GtkWidget *)*2);
	callbackRefs[0] = pageBox;
	callbackRefs[1] = pageNotebook;
	g_signal_connect(G_OBJECT(pageNameField), "activate", G_CALLBACK(pageNameChangedCallback), (gpointer)callbackRefs);
	g_signal_connect(G_OBJECT(pageContentBuffer), "changed", G_CALLBACK(pageContentChangedCallback), NULL);
	/* Finalize widget creation. */
	gtk_widget_show(pageNameField);
	gtk_widget_show(pageContentTextView);
	gtk_box_pack_start(GTK_BOX(pageBox), pageNameField, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(pageBox), pageScrollWindow, TRUE, TRUE, 0);
	gtk_widget_show(pageBox);
	/* Create the section widgets. */
	pageNotebook = gtk_notebook_new();
	/* Keep a reference to the current section node. */
	g_object_set_data(G_OBJECT(pageNotebook), "xmlNodeSection", (gpointer)sectionNode);
	/* Keep a reference to the section's name node. */
	g_object_set_data(G_OBJECT(pageNotebook), "xmlNodeName", (gpointer)sectionNameNode);
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(pageNotebook), GTK_POS_LEFT);
	gtk_widget_show(pageNotebook);
	/* Add the page to the notebook. */
	gtk_notebook_append_page(GTK_NOTEBOOK(pageNotebook), pageBox, gtk_label_new("Untitled Page"));
	/* Add the section to the notebook. */
	gtk_notebook_append_page(GTK_NOTEBOOK(widgets[1]), pageNotebook, gtk_label_new("Untitled Section"));
	
}

void sectionMenuRemoveCallback(GtkWidget *widget, gpointer data)
/* Removes a section from the notebook. */
{
	GtkWidget **widgets; 
	GtkWidget *noDocumentErrorDialog;
	GtkWidget *noSectionsErrorDialog;
	xmlDocPtr notebookDoc = NULL;
	xmlNode *notebook = NULL;
	GtkWidget *pageNotebook;
	GtkWidget *setionHasPagesDialog;
	xmlNode *section = NULL;
	
	/* Get a reference to the window(0) and sectionNotebook(1). */
	widgets = (GtkWidget **)data;
	/* Check if there's a loaded document. */
	if (g_object_get_data(G_OBJECT(widgets[1]), "xmlDocument") == NULL) {
		noDocumentErrorDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"You need to have a document open to delete a section.");
		gtk_dialog_run (GTK_DIALOG (noDocumentErrorDialog));
		gtk_widget_destroy (noDocumentErrorDialog);
		return; 
	}
	/* Make sure there's a section to delete. */
	if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(widgets[1])) < 1) {
		noSectionsErrorDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"There are no more sections to delete.");
		gtk_dialog_run (GTK_DIALOG (noSectionsErrorDialog));
		gtk_widget_destroy (noSectionsErrorDialog);
		return; 
	}
	/* Get a reference to the XML doc. */
	notebookDoc = g_object_get_data(G_OBJECT(widgets[1]), "xmlDocument");
	/* Get a reference to the root element. */
	notebook = xmlDocGetRootElement(notebookDoc);
	/* Get a reference to the current section. */
	pageNotebook = gtk_notebook_get_nth_page(GTK_NOTEBOOK(widgets[1]), 
		gtk_notebook_get_current_page(GTK_NOTEBOOK(widgets[1])));
	/* Check there aren't any pages left in the section. */
	if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(pageNotebook)) > 0) {
		setionHasPagesDialog = gtk_message_dialog_new (
			GTK_WINDOW(widgets[0]), GTK_DIALOG_DESTROY_WITH_PARENT, 
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
			"You need to remove all the pages from a section before removing the section.");
		gtk_dialog_run (GTK_DIALOG (setionHasPagesDialog));
		gtk_widget_destroy (setionHasPagesDialog);
		return;
	}
	/* Remove the XML Node for the section. */
	section = g_object_get_data(G_OBJECT(pageNotebook), "xmlNodeSection");
	xmlUnlinkNode(section);
	xmlFree(section);
	/* Delete the widgets. */
	gtk_notebook_remove_page(GTK_NOTEBOOK(widgets[1]), 
		gtk_notebook_get_current_page(GTK_NOTEBOOK(widgets[1])));
}

#endif
