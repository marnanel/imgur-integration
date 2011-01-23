#include "prefs.h"

#define IMGUR_DEFAULT_URL "http://imgur.com/api/upload.xml"

/**
 * The key we use to identify ourselves as a client
 * to imgur.com. For other APIs, use a different key.
 * You should possibly get your own imgur API key,
 * which you can do at http://imgur.com/register/api_anon .
 */
#define IMGUR_KEY "5a2d59b29160b55090e6bd9cd539519f"

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
	result->key = g_strdup (IMGUR_KEY);
	result->recording = TRUE;
	result->username = NULL;
	result->password = NULL;

	return result;
}

void
imgur_prefs_free(ImgurPrefs *prefs)
{
	g_free (prefs->api);
	g_free (prefs->key);
	g_free (prefs->username);
	g_free (prefs->password);
	g_free (prefs);
}

