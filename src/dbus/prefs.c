#include "prefs.h"

#define IMGUR_DEFAULT_URL "http://imgur.com/api/upload.xml"

/* STUB. FIXME.
 * This should read the settings out of a file,
 * and only if that file doesn't exist should it
 * use the defaults.
 */
ImgurPrefs*
imgur_prefs_new(void)
{
	ImgurPrefs *result = g_malloc (sizeof(ImgurPrefs));

	result->api = g_strdup (IMGUR_DEFAULT_URL);
	result->recording = 1;
	result->username = NULL;
	result->password = NULL;

	return result;
}

void
imgur_prefs_free(ImgurPrefs *prefs)
{
	g_free (prefs->api);
	g_free (prefs->username);
	g_free (prefs->password);
	g_free (prefs);
}

