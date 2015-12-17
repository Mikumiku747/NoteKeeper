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
	GtkWidget *MenuBarItems[2];
	GtkWidget *FileMenu;
	GtkWidget *FileMenuItems[3];
	GtkWidget *EditMenu;
	GtkWidget *EditMenuItems[3];
	
	/* Create and set up the menubar */
	MenuBar = gtk_menu_bar_new();
	gtk_widget_show(MenuBar);
	/* Set up the File Menu */
	FileMenu = gtk_menu_new();
	FileMenuItems[0] = gtk_menu_item_new_with_mnemonic("Open");
	g_signal_connect(FileMenuItems[0], "activate", 
		G_CALLBACK(fileMenuOpenCallback), 
		(gpointer)12345);
	FileMenuItems[1] = gtk_menu_item_new_with_label("Save");
	FileMenuItems[2] = gtk_menu_item_new_with_label("Quit");
	g_signal_connect(FileMenuItems[2], "activate", 
		G_CALLBACK(topWindow_delete_event), (gpointer)"Hello");
	gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), 
		FileMenuItems[0]);
	gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), 
		FileMenuItems[1]);
	gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), 
		FileMenuItems[2]);
	for (int i = 0; i < 3; i++) {
		gtk_widget_show(FileMenuItems[i]);
	}
	/* Set up the Edit Menu */
	EditMenu = gtk_menu_new();
	EditMenuItems[0] = gtk_menu_item_new_with_label("Copy");
	EditMenuItems[1] = gtk_menu_item_new_with_label("Paste");
	EditMenuItems[2] = gtk_menu_item_new_with_label("Select All");
	gtk_menu_shell_append(GTK_MENU_SHELL(EditMenu), 
		EditMenuItems[0]);
	gtk_menu_shell_append(GTK_MENU_SHELL(EditMenu), 
		EditMenuItems[1]);
	gtk_menu_shell_append(GTK_MENU_SHELL(EditMenu), 
		EditMenuItems[2]);
	for (int i = 0; i < 3; i++) {
		gtk_widget_show(EditMenuItems[i]);
	}
	/* Connect the submenus to the main menu */
	MenuBarItems[0] = gtk_menu_item_new_with_label("File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuBarItems[0]), 
		FileMenu);
	MenuBarItems[1] = gtk_menu_item_new_with_label("Edit");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuBarItems[1]), 
		EditMenu);
	for (int i = 0; i < 2; i++) {
		gtk_menu_bar_append(GTK_MENU_BAR(MenuBar), 
			MenuBarItems[i]);
		gtk_widget_show(MenuBarItems[i]);
	}
	
	return MenuBar;
}
