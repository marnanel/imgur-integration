/* There is some amount of code reuse between this file
 * and src/cmd/imgur.c, which is a bad thing.
 */
#include "eog-imgur-post.h"
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <dbus/dbus-glib-bindings.h>
#include "imgur-client-glue.h"

#define UPLOAD_METHOD "Upload"
#define TIMEOUT_MS 3000

void
eog_imgur_post (gchar *filename,
		EogImgurPostCallback *callback,
		gpointer user_data)
{
	DBusGConnection *connection = NULL;
	DBusGProxy *uploader = NULL;
	GError *error = NULL;
	GHashTable *result = NULL;

	/* FIXME: PROBABLY WRONG.
	Can we get a connection from elsewhere?
	*/
	connection = dbus_g_bus_get (DBUS_BUS_SESSION,
		&error);

	if (!connection)
	  {
	    g_print ("Could not connect to bus: %s\n",
		error->message);
	    g_error_free (error);
	    return;
	  }

	uploader = dbus_g_proxy_new_for_name (connection,
		"com.imgur",
		"/com/imgur",
		"com.imgur");

	if (com_imgur_upload (uploader, filename, &result, &error))
	  {
	        GValue *url = g_hash_table_lookup (result, "imgur_page");

		if (url)
	          {
			callback (TRUE,
				g_value_get_string (url),
				user_data);

			g_value_unset (url);
			g_free (url);	
	          }
	        else
	          {
			callback (FALSE,
				"Failed to parse response from server.",
				user_data);
	          }
	   }
	else
	  {
		  callback (FALSE, error->message, user_data);
		  g_error_free (error);
	  }

	/* FIXME: free result? */
	/* FIXME: free uploader? */
}


