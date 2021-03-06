/*  menu.c - Sets up the menu for the main program
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

#include "menu.h"
#include "callbacks.h"

GtkWidget *setupMenu(GtkWidget **importantWidgets) {
	/* Packs the main menu into the given destination widget. Returns a
	 * reference to the menu bar, so you can pack it how you want. */
	
	GtkWidget *MenuBar;
	GtkWidget *MenuBarItems[5];
	GtkWidget *FileMenu;
	GtkWidget *FileMenuItems[3];
	GtkWidget *EditMenu;
	GtkWidget *EditMenuItems[3];
	GtkWidget *PageMenu;
	GtkWidget *PageMenuItems[2];
	GtkWidget *SectionMenu;
	GtkWidget *SectionMenuItems[3];
	GtkWidget *HelpMenu;
	GtkWidget *HelpMenuItems[2];
	
	/* Create and set up the menubar */
	MenuBar = gtk_menu_bar_new();
	gtk_widget_show(MenuBar);
	/* Set up the File Menu */
	FileMenu = gtk_menu_new();
	FileMenuItems[0] = gtk_menu_item_new_with_mnemonic("_Open");
	g_signal_connect(FileMenuItems[0], "activate", 
		G_CALLBACK(fileMenuOpenCallback), 
		(gpointer)importantWidgets);
	FileMenuItems[1] = gtk_menu_item_new_with_mnemonic("_Save");
	g_signal_connect(FileMenuItems[1], "activate", 
		G_CALLBACK(fileMenuSaveCallback), 
		(gpointer)importantWidgets);
	FileMenuItems[2] = gtk_menu_item_new_with_mnemonic("_Quit");
	g_signal_connect(FileMenuItems[2], "activate", 
		G_CALLBACK(topWindow_delete_event), NULL);
	for (int i = 0; i < 3; i++) {
		gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), 
			FileMenuItems[i]);
		gtk_widget_show(FileMenuItems[i]);
	}
	/* Set up the Edit Menu */
	EditMenu = gtk_menu_new();
	EditMenuItems[0] = gtk_menu_item_new_with_label("Copy");
	EditMenuItems[1] = gtk_menu_item_new_with_label("Paste");
	EditMenuItems[2] = gtk_menu_item_new_with_label("Select All");
	for (int i = 0; i < 3; i++) {
		gtk_menu_shell_append(GTK_MENU_SHELL(EditMenu), 
			EditMenuItems[i]);
		gtk_widget_show(EditMenuItems[i]);
	}
	/* Set up the page menu. */
	PageMenu = gtk_menu_new();
	PageMenuItems[0] = gtk_menu_item_new_with_mnemonic("_New page");
	g_signal_connect(PageMenuItems[0], "activate", 
		G_CALLBACK(pageMenuNewCallback), (gpointer)importantWidgets);
	PageMenuItems[1] = gtk_menu_item_new_with_mnemonic("_Remove page");
	g_signal_connect(PageMenuItems[1], "activate",
		G_CALLBACK(pageMenuRemoveCallback), (gpointer)importantWidgets);
	for (int i = 0; i < 2; i++) {
		gtk_menu_shell_append(GTK_MENU_SHELL(PageMenu), 
			PageMenuItems[i]);
		gtk_widget_show(PageMenuItems[i]);
	}
	/* Set up the section menu. */
	SectionMenu = gtk_menu_new();
	SectionMenuItems[0] = gtk_menu_item_new_with_mnemonic("_New section");
	g_signal_connect(SectionMenuItems[0], "activate",
		G_CALLBACK(sectionMenuNewCallback), (gpointer)importantWidgets);
	SectionMenuItems[1] = gtk_menu_item_new_with_mnemonic("_Remove section");
	g_signal_connect(SectionMenuItems[1], "activate",
		G_CALLBACK(sectionMenuRemoveCallback), (gpointer)importantWidgets);
	SectionMenuItems[2] = gtk_menu_item_new_with_mnemonic("R_ename section");
	for (int i = 0; i < 3; i++) {
		gtk_menu_shell_append(GTK_MENU_SHELL(SectionMenu), 
			SectionMenuItems[i]);
		gtk_widget_show(SectionMenuItems[i]);
	}
	/* Set up the help menu. */
	HelpMenu = gtk_menu_new();
	HelpMenuItems[0] = gtk_menu_item_new_with_mnemonic("_Documentation");
	HelpMenuItems[1] = gtk_menu_item_new_with_mnemonic("_About");
	g_signal_connect(HelpMenuItems[1], "activate",
		G_CALLBACK(helpMenuAboutCallback), NULL);
	for (int i = 0; i < 2; i++) {
		gtk_menu_shell_append(GTK_MENU_SHELL(HelpMenu), 
			HelpMenuItems[i]);
		gtk_widget_show(HelpMenuItems[i]);
	}
	/* Connect the submenus to the main menu */
	MenuBarItems[0] = gtk_menu_item_new_with_mnemonic("_File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuBarItems[0]), 
		FileMenu);
	MenuBarItems[1] = gtk_menu_item_new_with_mnemonic("_Edit");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuBarItems[1]), 
		EditMenu);
	MenuBarItems[2] = gtk_menu_item_new_with_mnemonic("_Page");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuBarItems[2]),
		PageMenu);
	MenuBarItems[3] = gtk_menu_item_new_with_mnemonic("_Section");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuBarItems[3]),
		SectionMenu);
	MenuBarItems[4] = gtk_menu_item_new_with_mnemonic("_Help");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuBarItems[4]),
		HelpMenu);
	for (int i = 0; i < 5; i++) {
		gtk_menu_bar_append(GTK_MENU_BAR(MenuBar), 
			MenuBarItems[i]);
		gtk_widget_show(MenuBarItems[i]);
	}
	return MenuBar;
}
