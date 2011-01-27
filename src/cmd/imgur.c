#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <dbus/dbus-glib-bindings.h>
#include "../../data/imgur-client-glue.h"

#define UPLOAD_METHOD "Upload"
#define TIMEOUT_MS 3000

enum {
	EXIT_OK = 0,
	EXIT_ARGUMENT_PARSE_ERROR,
	EXIT_BUS_ERROR,
	EXIT_NO_ARGUMENT,
	EXIT_NO_RECORD_ID,
	EXIT_IMGUR_ERROR = 255
};

/*
 * FIXME: the "time" column needs strftime()ing.
 */

/*
 * FIXME: show_browser SHOULD work when
 * we are using list_some_records as well.
 */
gboolean show_browser = FALSE;
gboolean list_all_records = FALSE;
gboolean list_some_records = FALSE;
gchar *filename = NULL;

DBusGConnection *connection = NULL;
DBusGProxy *uploader = NULL;

static GOptionEntry entries[] =
{
	{ "browser", 'b', 0, G_OPTION_ARG_NONE, &show_browser,
		"Launch browser if successful", NULL },
	{ "list", 'l', 0, G_OPTION_ARG_NONE, &list_all_records,
		"List previous uploads", NULL },
	{ "records", 'r', 0, G_OPTION_ARG_NONE, &list_some_records,
		"List details of a previous upload", NULL },
	{ NULL }
};

static void
parse_commandline (int argc, char **argv)
{
	GError *error = NULL;
	GOptionContext *context =
		g_option_context_new ("[PATH] - post images to the web");

	g_option_context_add_main_entries (context,
		entries, "imgur");

	if (!g_option_context_parse (context,
		&argc, &argv, &error))
	  {
	    g_print ("%s\n", error->message);
	    g_error_free (error);
	    exit (EXIT_ARGUMENT_PARSE_ERROR);
	  }

	if (argc!=0)
	  {
	    filename = argv[1];
	  }

	if (filename && strncmp (filename, "file:", 5)==0)
	  {
	    filename += 5;
	  }

	if (!filename && !list_all_records)
	  {
	    gchar *help = g_option_context_get_help (context,
		FALSE, NULL);

	    g_print ("%s", help);
	    g_free (help);

	    exit (EXIT_NO_ARGUMENT);
	  }

	if (filename && !list_some_records)
	{
		/*
		 * At this point, "filename" is non-NULL and
		 * contains the filename we need. It's statically
		 * allocated. It may be relative. We need it
		 * to be allocated on the heap, and absolute.
		 */

		if (g_path_is_absolute (filename))
		{
			/*
			 * lovely, just what we want.
			 * Take a copy.
			 */
			filename = g_strdup (filename);
		}
		else
		{
			/* Attempt to canonicalise. */

			gchar *cwd = g_get_current_dir ();

			filename = g_build_filename (cwd,
					filename, NULL);

			g_free (cwd);
		}
	}
}

static void
get_proxy (void)
{
	GError *error = NULL;

	connection = dbus_g_bus_get (DBUS_BUS_SESSION,
		&error);

	if (!connection)
	  {
	    g_print ("Could not connect to bus: %s\n",
		error->message);
	    g_error_free (error);
	    exit (EXIT_BUS_ERROR);
	  }

	uploader = dbus_g_proxy_new_for_name (connection,
		"com.imgur",
		"/com/imgur",
		"com.imgur");
}

static void
launch_browser (const char* url)
{
	GError *error = NULL;
	gchar *command_line = g_strdup_printf ("xdg-open %s",
			url);

	if (!g_spawn_command_line_async (command_line, &error))
	{
		g_warning ("Failed to spawn browser: %s", error->message);
		g_error_free (error);
		error = NULL;
	}

}

static void
print_one_line (gpointer key, gpointer value, gpointer identifier)
{
	GValue *v = (GValue*) value;

	g_print ("%s\t%s\t%s\n",
	  identifier,
	  (gchar*) key,
	  g_value_get_string (v));
}

static void
print_hash_table (GHashTable *hash,
	gchar *title)
{
	GError *error = NULL;
	if (!title)
	{
		/*
 		 * Oh dear. I suppose we'd better
 		 * look it up, then.
 		 */

		GValue *v = g_hash_table_lookup (hash,
			"image_hash");

		if (v)
			title = (gchar*) g_value_get_string(v);
		else
			title = "???";
	}

	/*
 	 * Now actually print stuff.
 	 */

	g_hash_table_foreach (hash,
			print_one_line,
			title);

	if (g_hash_table_size (hash)==0)
	{
		/*
 		 * No such record.  But
		 * output a dummy record
		 * so that the user knows
		 * we did actually look.
		 */

		g_print ("%s\tfound\t0\n",
			title);
	}
}

static void
perform_upload (void)
{
	GError *error = NULL;
	GHashTable *result = NULL;

	if (com_imgur_upload (uploader, filename, &result, &error))
	  {
	    if (show_browser)
	      {
	        GValue *url = g_hash_table_lookup (result, "imgur_page");

		if (url)
	          {
			launch_browser (g_value_get_string (url));
	          }
	      }

            print_hash_table (result, NULL);

	    exit (EXIT_OK);
	  }
	else
	  {
	    g_print ("error\t%s\n",
		error->message);
	    g_error_free (error);

	    exit (EXIT_IMGUR_ERROR);
	  }
}

static void
show_all_records (void)
{
	GError *error = NULL;
	gchar **list = NULL;

	if (com_imgur_list_records (uploader, &list, &error))
	{
		gchar **cursor;

		for (cursor=list; *cursor; cursor++)
		{
			g_print ("%s\n", *cursor);
		}

		if (!*list)
		{
			fprintf (stderr, "(no records to list)\n");
		}

		g_strfreev (list);
	}
	else
	{
		fprintf (stderr, "imgur: error in listing records: %s\n",
			error->message);
		g_error_free (error);
		exit (EXIT_IMGUR_ERROR);
	}
	
}

static void
show_some_records (void)
{
	GError *error = NULL;
	GHashTable *hash = NULL;

	if (com_imgur_get_record (uploader, filename, &hash, &error))
	{
		print_hash_table (hash, filename);

		exit (EXIT_OK);
	}
	else
	{
		fprintf (stderr, "imgur: error in listing records: %s\n",
			error->message);
		g_error_free (error);
		exit (EXIT_IMGUR_ERROR);
	}
}
	
int
main (int argc, char **argv)
{
	g_type_init ();

	parse_commandline (argc, argv);

	get_proxy ();

	if (list_all_records)
		show_all_records ();

	if (filename)
	{
		if (list_some_records)
		{
			show_some_records ();
		}
		else
		{
			perform_upload ();
		}
	}
}

/* EOF imgur.c */

