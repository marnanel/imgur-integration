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

struct parseresults {
  gboolean passed;
  gboolean take_text;
  GList *details;
};

static void
start_element (GMarkupParseContext *context,
	       const gchar         *element_name,
	       const gchar        **attribute_names,
	       const gchar        **attribute_values,
	       gpointer             user_data,
	       GError             **error)
{
  struct parseresults *results = user_data;

  if (strcmp(element_name, "rsp")==0) {
	  results->passed = TRUE;
  } else if (strcmp(element_name, "error")==0) {
	  results->passed = FALSE;
  } else {
    results->details = g_list_prepend (results->details,
                                      g_strdup(element_name));

    results->details = g_list_prepend (results->details,
                                      g_strdup(""));

    results->take_text = TRUE;
  }
}

static void
end_element (GMarkupParseContext *context,
	     const gchar         *element_name,
	     gpointer             user_data,
	     GError             **error)
{
  struct parseresults *results = user_data;
  results->take_text = FALSE;
}

static void
text (GMarkupParseContext *context,
      const gchar         *text,
      gsize                text_len,  
      gpointer             user_data,
      GError             **error)
{
  struct parseresults *results = user_data;
  char *old, *result;
  
  if (!results->take_text)
    return;

  old = results->details->data;

  result = g_strdup_printf("%s%*s",
			   old,
			   text_len,
			   text);

  results->details->data = result;
  
  g_free (old);
}


/**
 * Parses a string of XML returned from imgur.
 *
 * \param xml  The XML.
 * \param details  If non-NULL: if this is a success,
 *                 set to the image hash; if failure,
 *                 set to an error message.  Either way
 *                 the caller must g_free().  If NULL,
 *                 it is not used.
 * \return TRUE if this is a success; FALSE otherwise.
 */
GList *
parse_imgur_response(char *xml, gboolean *success)
{
  struct parseresults results = { FALSE, FALSE, NULL };

  GMarkupParser parser = {
    start_element,
    end_element,
    text,
    NULL,
    NULL
  };
  GMarkupParseContext *context =
    g_markup_parse_context_new (&parser,
				0,
				&results,
				NULL);

  g_markup_parse_context_parse (context,
				xml, -1, NULL);

  g_markup_parse_context_free (context);

  if (success)
    {
      *success = results.passed;
    }

  return g_list_reverse (results.details);
}

