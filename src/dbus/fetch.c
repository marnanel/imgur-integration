#include "fetch.h"

CURL *our_curl = NULL;

CURL *
fetch_our_curl (void)
{
	if (!our_curl)
	{
		our_curl = curl_easy_init ();
	}

	return our_curl;
}

static size_t
fetch_cb (void *ptr, size_t size, size_t nmemb, void *data)
{
	char **so_far = (char **) data;

	if (*so_far)
	{
		char *temp = *so_far;

		*so_far = g_strdup_printf ("%s%*s",
			*so_far, size*nmemb, (gchar*) ptr);

		g_free (temp);
	}
	else
	{
		*so_far = g_strdup_printf ("%*s",
			size*nmemb, (gchar*) ptr);
	}

	return nmemb;
}

gchar *fetch_url (const gchar *url,
	GError **error)
{
	CURL *curl = fetch_our_curl ();
	CURLcode res;
	gchar *so_far = NULL;

	/* FIXME: No error checking yet! */

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
		fetch_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &so_far);

	res = curl_easy_perform(curl);

	/* FIXME check res */

	return so_far;
}
