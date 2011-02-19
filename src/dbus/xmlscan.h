#ifndef XMLSCAN_H
#define XMLSCAN_H 1

#include <glib.h>

/**
 * Scans a piece of XML for key-value pairs. All the pairs
 * must be wrapped in an outer tag; the keys are tags, the
 * text inside the tags is the value.
 *
 * This uses GMarkup, not a full XML parser, so some
 * invalid features may be accepted (but no valid features
 * should be rejected).
 *
 * \param xml        The XML text.
 * \param wrapper    The outermost wrapper tag.
 * \param user_data  User data to pass to the callback.
 * \return           A hash table mapping strings to strings.
 *                   Free it with g_hash_table_unref() when done.
 */
GHashTable *xml_scan (const gchar *xml,
	gchar *wrapper);

#endif

