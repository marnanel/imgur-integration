/* 
 * Copyright (C) 2010 Thomas Thurman <tthurman@gnome.org>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <glib.h>
#include <curl/curl.h>
#include "list-remote.h"
#include "prefs.h"

GPtrArray**
list_remote (gboolean popular)
{
	gint count = 0;
	GPtrArray **result;

	/* STUB */

	count = 1;
	result = g_malloc(sizeof(GPtrArray*)*(count+1));
	result[count] = NULL;

	return result;
}
