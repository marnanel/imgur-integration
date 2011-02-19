#include <glib.h>
#include <string.h>
#include "xmlscan.h"

typedef struct _XmlScanPrivate {
	guint depth;
	gchar *wrapper;
	GHashTable *result;
	gchar *catcher;
	gboolean problem;
} XmlScanPrivate;

static void
xml_scan_start (GMarkupParseContext *context,
		const gchar         *element_name,
		const gchar        **attribute_names,
		const gchar        **attribute_values,
		gpointer             user_data,
		GError             **error)
{
	XmlScanPrivate *private = user_data;

	switch (private->depth)
	{
	case 0:
		if (strcmp (private->wrapper, element_name)==0)
		{
			/* Only pay attention if it's our wrapper */
			private->depth++;
		}
		break;

	case 1:
		/* Okay, start capturing */
		private->catcher = g_strdup ("");
		private->depth++;
		break;

	default:
		private->depth++;
		break;
	}
}

static void
xml_scan_end (GMarkupParseContext *context,
		const gchar         *element_name,
		gpointer             user_data,
		GError             **error)
{
	XmlScanPrivate *private = user_data;
	g_print ("Tag is </%s>, captured %s\n", element_name, private->catcher);

	if (private->catcher)
	{
		g_hash_table_insert (private->result,
				g_strdup (element_name), private->catcher);
		private->catcher = NULL; /* but don't free it! */
	}

	if (private->depth)
		private->depth--;
}

static void
xml_scan_text (GMarkupParseContext *context,
		const gchar         *text,
		gsize                text_len,
		gpointer             user_data,
		GError             **error)
{
	XmlScanPrivate *private = user_data;
	gchar *temp = private->catcher;

	if (temp)
	{
		private->catcher = g_strdup_printf ("%s%*s",
			temp, text_len, text);
		g_free (temp);
	}
}

GMarkupParser xml_scan_parser = {
	xml_scan_start,
	xml_scan_end,
	xml_scan_text,
	NULL, /* ignore passthrough */
	NULL, /* no error handler, rely on the boolean result */
};

GHashTable*
xml_scan (const gchar *xml,
        gchar *wrapper)
{
	XmlScanPrivate private;
	GMarkupParseContext *context;
	gboolean parse_result;

	private.depth = 0;
	private.wrapper = wrapper;
	private.catcher = NULL;
	private.result = g_hash_table_new_full (g_str_hash,
		g_str_equal,
		g_free,
		g_free);

	context = g_markup_parse_context_new (&xml_scan_parser,
			G_MARKUP_TREAT_CDATA_AS_TEXT,
			&private,
			NULL);

	parse_result = g_markup_parse_context_parse (context,
		xml,
		-1,
		NULL);

	g_markup_parse_context_free (context);

	if (parse_result)
	{
		/* it all worked; return the hash */
		return private.result;
	}
	else
	{
		/* it failed */
		g_hash_table_unref (private.result);
		return NULL;
	}
}

