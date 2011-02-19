#include "xmlscan.h"
#include "thurman-test.h"
#include <glib-object.h>
#include <string.h>

gchar* xml =
	"<silly>"
	"<ignore>this</ignore>"
	"<test>"
	"<red>gules</red>"
	"<blue>azure</blue>"
	"<green>vert</green>"
	"<!-- this will also be ignored -->"
	"as will this"
	"</test>"
	"</silly>";

gchar *broken_xml =
	"<something>"
	"<test>"
	"<fred>bassett</fred>"
	"</something>"
	"</test>";

static gint
alpha_sort (gconstpointer a, gconstpointer b)
{
	return strcmp ((gchar*)a, (gchar*)b);
}

static gchar *
hash_to_string (GHashTable *hash)
{
	gchar *result = NULL;
	GList *keys, *keys_cursor;

	if (!hash)
		return g_strdup ("null");

	keys = g_hash_table_get_keys (hash);

	keys = g_list_sort (keys, alpha_sort);

	for (keys_cursor = keys; keys_cursor; keys_cursor = keys_cursor->next)
	{
		gchar *entry = g_strdup_printf ("%s=%s",
				(gchar*) keys_cursor->data,
				(gchar*) g_hash_table_lookup (hash, keys_cursor->data));

		if (result)
		{
			gchar *temp = result;
			result = g_strdup_printf ("%s %s", temp, entry);
			g_free (temp);
			g_free (entry);
		}
		else
		{
			result = entry;
		}
	}

	return result;
}

static gboolean
test_scan (gchar *xml, gchar *want)
{
	GHashTable *result;
	gchar *str;
	gboolean match;

	result = xml_scan (xml, "test");
	str = hash_to_string (result);
	match = strings_match (want, str);
	if (result)
		g_hash_table_unref (result);
	g_free (str);

	return match;
}

static gboolean
simple_parse (void)
{
	return test_scan (xml,
		"foo");
}
	
static gboolean
broken_parse (void)
{
	return test_scan (broken_xml,
		"null");
}

int
test_main (int argc, char **argv)
{
	g_type_init ();

	add_test ("simple-parse", THURMAN_TEST_IS_UNIT, simple_parse,
		"test XML parser with simple input");
	add_test ("broken-parse", THURMAN_TEST_IS_UNIT, broken_parse,
		"test XML parser with broken input");

	return 0;
}
