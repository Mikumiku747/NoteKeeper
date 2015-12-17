/*  xml.h - XML related functions (Saving and loading the notebooks)
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

#ifndef XML_H
#define XML_H

/* Includes */
#include <libxml/parser.h>
#include <libxml/tree.h>

int initXML();
/* Initialises the XML library */

xmlDoc *loadNotebook(char *filename);
/* Loads an XML document into memory from a file */

#endif
