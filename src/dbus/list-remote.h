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

#ifndef LIST_REMOTE_H
#define LIST_REMOTE_H 1

#include <glib.h>

/**
 * Lists some images on the remote server.
 *
 * \param popular  TRUE if you want the popular images;
 *                 FALSE if you want the most recent images.
 * \return A null-terminated list of GPtrArray*s, which the
 *         caller should free. If there are no images
 *         to be listed, returns a pointer to NULL.
 */
GPtrArray**
list_remote (gboolean popular);

#endif
