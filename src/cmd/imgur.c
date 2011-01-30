#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <time.h>
#include <dbus/dbus-glib-bindings.h>
#include "../../data/imgur-client-glue.h"
#include <config.h>

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

/**
 * Parses the commandline.
 *
 * \param argc  The number of commandline arguments, plus one.
 * \param argv  The name of the program, followed by the
 *              commandline arguments.
 */
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

/**
 * Sets up the global variables concerned with DBus.
 */
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

/**
 * Launches the web browser. If that fails, prints an error
 * to stderr.
 *
 * \param url  The address to send the browser to.
 */
static void
launch_browser (const char* url)
{
	GError *error = NULL;

#ifdef MAEMO
	/*
	 * Maemo has its own rather weird way of doing this.
	 */
	DBusGProxy *proxy = dbus_g_proxy_new_for_name (connection,
			"com.nokia.osso_browser",
			"/com/nokia/osso_browser/request",
			"com.nokia.osso_browser");

	if (!dbus_g_proxy_call (proxy, "load_url", &error,
				G_TYPE_STRING, url,
				G_TYPE_INVALID,
				G_TYPE_INVALID))
	{
		g_warning ("Failed to spawn browser: %s", error->message);
		g_error_free (error);
	}
#else
	gchar *command_line = g_strdup_printf ("xdg-open %s",
			url);

	if (!g_spawn_command_line_async (command_line, &error))
	{
		g_warning ("Failed to spawn browser: %s", error->message);
		g_error_free (error);
		error = NULL;
	}
#endif
}

/**
 * Returns a time in human-readable format.
 *
 * \param clock  A decimal number of seconds since the epoch.
 * \return  The same time, expressed in human-readable format;
 *          or a copy of the same string if that string was invalid.
 *          Either way, it's the user's responsibility to free it.
 */
static gchar*
format_time (const gchar* clock)
{
	time_t time_int = atol (clock);
	struct tm time_struct;
	gchar buffer[2048];

	if (time_int==0)
	{
		return g_strdup (clock);
	}

	localtime_r (&time_int, &time_struct);

	strftime (buffer, sizeof (buffer),
		"%Y-%m-%d %H:%M:%S",
                &time_struct);

	return g_strdup (buffer);
}

/**
 * Prints one line of a hash table. Ancillary to print_hash_table().
 *
 * \param key  The key of the hash; a cast char*.
 *             If this is "imgur_page", and the global variable
 *             show_browser is TRUE, this has the side-effect of
 *             launching the browser on the URL given in value.
 * \param value  The value of the hash; a cast GValue containing
 *               a string.
 * \param identifier  An identifier for the whole hash.
 */
static void
print_one_line (gpointer key, gpointer value, gpointer identifier)
{
	GValue *v = (GValue*) value;
	const gchar *key_str = (gchar*) key;
	const gchar *value_str = g_value_get_string (value);
	gchar *temp = NULL;

	if (strcmp (key_str, "time")==0)
	{
		temp = format_time (value_str);
	}

	g_print ("%s\t%s\t%s\n",
	  identifier,
	  key_str,
	  temp? temp: value_str);

	g_free (temp);

	if (show_browser &&
		strcmp (key_str, "imgur_page")==0)
	{
		launch_browser (value_str);
	}
}

/**
 * Prints the contents of a hash table mapping
 * strings to GValues containing strings.
 * See print_one_line() for special cases.
 * If the hash is null, a dummy record mapping
 * "found" to "0" is printed.
 *
 * \param hash   The hash whose contents should
 *               be printed.
 * \param title  A title for the hash, to be
 *               printed in the first column.
 *               If this is NULL, it's guessed
 *               from the contents of the hash.
 */
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

/**
 * Uploads an image, according to the commandline options.
 */
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

/**
 * Prints an identifier for every known record to standard output.
 */
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

/**
 * Prints all details of one record to standard output.
 */
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

