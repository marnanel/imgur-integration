#include <glib-object.h>
#include "recording.h"

static void
add_to_hash (GHashTable *hash,
	const char* key,
	const char* value)
{
	GValue *temp = g_malloc0 (sizeof (GValue));

	g_value_init (temp, G_TYPE_STRING);
	g_value_set_static_string (temp, value);

	g_hash_table_insert (hash, (char*) key, temp);
}

int
main (int argc, char **argv)
{
	GHashTable *hash;

	g_type_init ();

	hash = g_hash_table_new (g_str_hash,
				g_str_equal);

	add_to_hash (hash, "image_hash", "10YnP");
	add_to_hash (hash, "small_thumbnail", "http://i.imgur.com/10YnPs.jpg");
	add_to_hash (hash, "original_image", "http://i.imgur.com/10YnP.jpg");
	add_to_hash (hash, "large_thumbnail", "http://i.imgur.com/10YnPl.jpg");
	add_to_hash (hash, "delete_hash", "inQAUnHqeypbFs0");
	add_to_hash (hash, "delete_page", "http://imgur.com/delete/inQAUnHqeypbFs0");
	add_to_hash (hash, "imgur_page", "http://imgur.com/10YnP");

	eog_recording_store (hash, "/tmp/lsm.jpg");
}

/* EOF test-recording.c */

