#include "list-records.h"
#include <glib-object.h>
#include <string.h>
#include <stdlib.h>

typedef struct _ListEntry
{
	gchar *filename;
	long time;
} ListEntry;

static gint
compare_entries (gconstpointer a, gconstpointer b)
{
	const ListEntry* aa = (const ListEntry*) a;
	const ListEntry* bb = (const ListEntry*) b;

	if (aa->time < bb->time)
		return 1;
	else if (aa->time < bb->time)
		return -1;
	else
		return 0;
}

static gchar*
get_path (void)
{
	return g_build_filename (g_get_user_data_dir (),
			"imgur",
			NULL);
}

static GKeyFile*
get_keyfile (const gchar* path)
{
	GKeyFile *keyfile = g_key_file_new ();
	char *filename = g_build_filename (path,
			"uploads.conf",
			NULL);

	if (g_key_file_load_from_file (keyfile,
				filename,
				G_KEY_FILE_NONE,
				NULL)==FALSE)
	{
		g_key_file_free (keyfile);
		keyfile = NULL;
	}

	g_free (filename);

	return keyfile;
}

gchar**
imgur_list_records (void)
{
	gchar **result = NULL, **result_cursor;
	GKeyFile *keyfile;
	gchar **entries, **cursor;
	gchar *path;
	GList *candidates = NULL, *candidate_cursor;
	long count = 0;

	path = get_path ();
	keyfile = get_keyfile (path);

	if (!keyfile)
	{
		g_free (path);
		result = g_malloc (sizeof (gchar* ));
		*result = NULL;
		return result;
	}

	entries = g_key_file_get_groups (keyfile, NULL);

	for (cursor=entries; *cursor; cursor++)
	{
		gchar *thumbnail,
			*thumbnail_extension,
			*time,
			*our_thumbnail;
		long timestamp;
		ListEntry *entry;

		thumbnail = g_key_file_get_string (keyfile,
			*cursor,
			"small_thumbnail",
			NULL);

		if (!thumbnail)
		{
			g_warning ("Group %s has no thumbnail",
				*cursor);
			continue;
		}

		thumbnail_extension = strrchr (thumbnail, '.');

		if (!thumbnail_extension)
		{
			g_warning ("Group %s has an invalid thumbnail",
				*cursor);
			continue;
		}

		time = g_key_file_get_string (keyfile,
			*cursor,
			"time",
			NULL);

		if (time)
		{
			timestamp = atol (time);
		}
		else
		{
			/* well, it won't kill us */
			timestamp = 0;
		}

		our_thumbnail = g_strdup_printf (
			"%s/%s%s",
			path,
			*cursor,
			thumbnail_extension);

		if (!g_file_test (our_thumbnail,
			G_FILE_TEST_IS_REGULAR))
		{
			g_free (time);
			g_free (thumbnail);
			g_free (our_thumbnail);
			continue;
		}

		entry = g_malloc (sizeof (ListEntry));
		entry->time = timestamp;
		entry->filename = our_thumbnail;

		count++;
		candidates = g_list_prepend (candidates,
			entry);

		g_free (time);
		g_free (thumbnail);
	}

	g_strfreev (entries);
	g_free (path);

	candidates = g_list_sort (candidates,
		compare_entries);

	result = g_malloc (sizeof (gchar*) * (count+1));
	result[count] = NULL;
	result_cursor = result;

	/* Now go through and pick out the names */

	for (candidate_cursor = candidates;
		candidate_cursor;
		candidate_cursor = candidate_cursor->next)
	{
		ListEntry *entry = (ListEntry*) candidate_cursor->data;

		*result_cursor = entry->filename;
		g_free (entry);

		result_cursor++;
	}

	g_list_free (candidates);

	return result;
}

/* A very similar function appears in imgur-service.c;
 * we should merge them.
 */
static void
add_hash_entry (GHashTable *hash,
	const gchar *key, const gchar *value)
{
	GValue *v = g_malloc0 (sizeof (GValue) );
	g_value_init (v, G_TYPE_STRING);
	g_value_set_string (v, value);
	g_hash_table_insert (hash, g_strdup(key), v);
}

GHashTable*
imgur_get_record (const gchar* record_name)
{
	gchar *path = get_path ();
	GKeyFile *keyfile = get_keyfile (path);
	GHashTable *result = g_hash_table_new (g_str_hash, g_str_equal);
	gchar **keys, **cursor;
	gchar *temp, *extension = NULL;

	if (!keyfile)
	{
		g_free (path);
		return result;
	}

	keys = g_key_file_get_keys (keyfile,
		record_name,
		NULL, NULL);

	for (cursor = keys; *cursor; cursor++)
	{
		gchar *value = g_key_file_get_string (keyfile,
				record_name,
				*cursor,
				NULL);

		add_hash_entry (result,
			*cursor,
			value);

		if (strcmp (*cursor, "small_thumbnail")==0)
		{
			extension = strrchr (value, '.');
		}
	}

	if (!extension)
	{
		/* ugh */
		extension = ".jpg";
	}

	temp = g_strdup_printf ("%s/%s%s",
		path,
		record_name,
		extension);

	if (g_file_test (temp,
		G_FILE_TEST_IS_REGULAR))
	{
		add_hash_entry (result,
			"local_thumbnail",
			temp);
	}

	/* Since we took it out before storage: */
	add_hash_entry (result,
		"image_hash",
		record_name);

	g_strfreev (keys);
	g_free (temp);
	g_free (path);

	return result;
}

