#include "prefs.h"
#include <stdarg.h>

#define IMGUR_DEFAULT_URL "http://imgur.com/api/upload.xml"

#define IMGUR_DEFAULT_V2_URL "http://api.imgur.com/2"

/**
 * The key we use to identify ourselves as a client
 * to imgur.com. For other APIs, use a different key.
 * You should possibly get your own imgur API key,
 * which you can do at http://imgur.com/register/api_anon .
 */
#define IMGUR_KEY "5a2d59b29160b55090e6bd9cd539519f"

/**
 * Looks up a specific value in the configuration
 * file. If the value is found, returns a copy of
 * it. If the value is not found, returns a copy
 * of the default, unless the default is NULL,
 * when it returns NULL.
 *
 * \param ini  The configuration file. May be NULL.
 * \param key  The key to look for (in the
 *             "general" section).
 * \param default_value  The default value; may be
 *                 NULL.
 * \returns  The value of the key. User must free.
 */
static gchar*
get_value (GKeyFile *ini,
	gchar *key,
	gchar *default_value)
{
	if (ini)
	{
		gchar *result = g_key_file_get_string (ini,
			"general",
			key,
			NULL);

		if (result)
		{
		  return result;
		}
	}
	
	if (default_value)
		return g_strdup (default_value);
	else
		return NULL;
}

ImgurPrefs*
imgur_prefs_new(void)
{
	ImgurPrefs *result = g_malloc (sizeof(ImgurPrefs));
	gchar *temp;
	GKeyFile *ini = g_key_file_new ();
	gchar *path;

	/* FIXME: this is wrong; we should do it by directories */
	path = g_build_filename (g_get_user_config_dir (),
		"imgur",
		"imgur.conf",
		NULL);

	if (g_key_file_load_from_file (ini,
		path,
		G_KEY_FILE_NONE,
		NULL)==FALSE)
	{
		/* it's clearly not available */
		g_key_file_free (ini);
		ini = NULL;
	}

	g_free (path);

	result->api = get_value (ini, "api", IMGUR_DEFAULT_V2_URL);
	result->key = get_value (ini, "key", IMGUR_KEY);
	result->username = get_value (ini, "username", NULL);
	result->password = get_value (ini, "password", NULL);

	temp = get_value (ini, "action", "browser");
	result->action = action_from_string (temp);
	g_free (temp);

	temp = get_value (ini, "recording", "1");
	result->recording = (temp && temp[0]=='1');
	g_free (temp);

	if (ini)
	{
		g_key_file_free (ini);
	}

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

gchar *
imgur_prefs_get_url (ImgurPrefs *prefs,
        gchar *method,
        ...)
{
	va_list ap;
	gboolean first = TRUE;
	gboolean odd = TRUE;
	gchar *cursor;
	gchar *result =
		g_strdup_printf ("%s/%s.xml",
			prefs->api,
			method);

	va_start (ap, method);

	while ((cursor = va_arg (ap, gchar*)))
	{
		gchar *temp = result;
		result = g_strdup_printf("%s%c%s",
			result,
			odd? (first? '?': '&'): '=',
			cursor);
		g_free (temp);
		odd = !odd;
		first = FALSE;
	}

	va_end (ap);

	return result;
}

