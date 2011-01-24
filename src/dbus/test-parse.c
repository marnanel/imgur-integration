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
#include <stdio.h>
#include <string.h>
#include "parse.h"

/* test data */

char *fail = "<rsp stat=\"fail\">\n"
  "<error_code>1000</error_code>\n"
  "<error_msg>No image selected</error_msg>\n"
  "</rsp>\n";

char *pass = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
  "<rsp stat=\"ok\">\n"
  "<image_hash>cxmHM</image_hash>\n"
  "<delete_hash>NNy6VNpiAA</delete_hash>\n"
  "<original_image>http://imgur.com/cxmHM.png</original_image>\n"
  "<large_thumbnail>http://imgur.com/cxmHMl.png</large_thumbnail>\n"
  "<small_thumbnail>http://imgur.com/cxmHMs.png</small_thumbnail>\n"
  "<imgur_page>http://imgur.com/cxmHM</imgur_page>\n"
  "<delete_page>http://imgur.com/delete/NNy6VNpiAA</delete_page>\n"
  "</rsp>\n";

static gchar *
join(GList *list)
{
   if (list==NULL)
     {
       return g_strdup("");
     }
   else
     {
       char *rest = join(list->next);
       char *current = g_strdup_printf("%s%s%s",
                         (gchar*) list->data,
                         *rest ? "," : "",
                         rest);

       g_free (rest);

       return current;
     }
}

int
main()
{
  GList *result;
  gboolean success;

  result = parse_imgur_response(fail, &success);
  printf("FAIL: Result=%d, message=%s\n", success, join(result));

  result = parse_imgur_response(pass, &success);
  printf("PASS: Result=%d, message=%s\n", success, join(result));
}
