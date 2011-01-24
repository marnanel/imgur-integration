#include <glib-object.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include "recording.h"

static gchar*
get_field (GHashTable *fields,
	const gchar *field_name)
{
	GValue *value = g_hash_table_lookup (fields,
				field_name);

	if (!value)
	{
		return NULL;
	}

	return g_strdup (g_value_get_string (value));
}

static int
fwrite_handler (void *buffer, size_t size, size_t nmemb, void *stream)
{
	return fwrite (buffer, size, nmemb, stream);
}

static void
download_thumbnail (gchar *url,
	gchar *target_filename)
{
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *target;

	if (!curl)
	{
		g_warning ("Could not download.");
		return;
	}

	target = fopen (target_filename, "wb");

	if (!target)
	{
		g_warning ("Could not create target file.");
		return;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
		(curl_write_callback) fwrite_handler);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, target);
 
	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	fclose (target);
}

void
imgur_recording_store(GHashTable *fields,
		gchar *filename)
{
	gchar *group = get_field (fields, "image_hash");
	GKeyFile *keyfile;
	gchar *path;
	gchar *inifile;
	gchar *temp;
	gchar *thumbnail = NULL;
	GList *keys, *cursor;

	if (!group)
	{
		g_warning ("Received fields table with no image_hash");
		return;
	}

	keyfile = g_key_file_new ();
	path = g_build_filename (g_get_user_data_dir (),
		"imgur",
		NULL);
	inifile = g_build_filename (path,
		"uploads.conf",
		NULL);

	g_key_file_load_from_file (keyfile,
		inifile,
		G_KEY_FILE_NONE,
		NULL);

	g_key_file_set_string (keyfile,
		group,
		"filename",
		filename);

	temp = g_strdup_printf ("%ld",
		time (NULL));
	g_key_file_set_string (keyfile,
		group,
		"time",
		temp);
	g_free (temp);

	keys = cursor = g_hash_table_get_keys (fields);
	while (cursor)
	{
		gchar *key = (gchar*) cursor->data;
		gchar *value = get_field (fields, key);

		if (strcmp (key, "image_hash") != 0)
		{
			g_key_file_set_string (keyfile,
					group,
					key,
					value);
		}

		if (strcmp (key, "small_thumbnail") == 0)
		{
			thumbnail = value;
		}
		else
		{
			g_free (value);
		}

		cursor = cursor->next;
	}
	g_list_free (keys);

	temp = g_key_file_to_data (keyfile,
		NULL, NULL);

	g_mkdir_with_parents (path, 0700);

	/* ignore the result */
	g_file_set_contents (inifile,
		temp,
		-1, NULL);

	g_free (temp);

	if (thumbnail)
	{
		gchar *final_component = strrchr (thumbnail, '/');
		gchar *thumbnail_target =
			g_build_filename (path,
				final_component,
				NULL);
		download_thumbnail (thumbnail,
			thumbnail_target);
		g_free (thumbnail);
	}

	g_free (inifile);
	g_key_file_free (keyfile);
	g_free (path);
	g_free (group);
}

/* EOF recording.c */

