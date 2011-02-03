#ifndef FETCH_H
#define FETCH_H 1

#include <glib.h>
#include <curl/curl.h>

/**
 * Returns a properly-initialised CURL object.
 */
CURL *fetch_our_curl (void);

/**
 * Returns whatever text is found at the given URL.
 * (This performs whatever initialisation is needed
 * as a side-effect.)
 *
 * \param url    The URL.
 * \param error  Set to details of the error if one occurs.
 *               May be NULL.
 * \return  The text found at the given URL, or NULL on error.
 *          The caller must free the text.
 */
gchar *fetch_url (const gchar *url,
	GError **error);

#endif
