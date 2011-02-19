#ifndef PREFS_H
#define PREFS_H 1

#include <glib.h>
#include "action.h"

/**
 * Preferences for the imgur service, as might
 * be stored in a configuration file.
 */
typedef struct _ImgurPrefs {
/**
 * Address of the API endpoint.
 * (Note: the environment variable IMGUR_URL
 * overrides this.)
 */
 gchar *api;

/**
 * A string we use to identify ourselves
 * as a client to the API.
 */
 gchar *key;

/**
 * Whether we should store details of the
 * images we upload, so the user can come
 * back to them later.
 */
 gboolean recording;

/**
 * Username to log into imgur.com with, for galleries.
 * Null if none should be used.
 * Not currently in use.
 */
 gchar *username;

/**
 * Password to log into imgur.com with, for galleries.
 * Null if none should be used.
 * Not currently in use.
 */
 gchar *password;

/**
 * Action to take after we successfully upload a picture.
 */
 AfterwardsAction action;
} ImgurPrefs;

/**
 * Creates an ImgurPrefs object, either from the
 * settings file on disc or, if that does not
 * exist, from defaults.
 *
 * \return  The new ImgurPrefs object.
 */
ImgurPrefs* imgur_prefs_new(void);

/**
 * Disposes of an ImgurPrefs object.
 *
 * \param prefs  The object to dispose of.
 */
void imgur_prefs_free(ImgurPrefs *prefs);

/**
 * Returns a URL for version 2 of the Imgur API.
 * The basic path may be overridden from the prefs.
 * You may also supply a sequence of pairs of strings,
 * which become a query string appended to the URL.
 * This is over-simple; there will be no escaping.
 * Be careful.
 * End the list with a NULL.
 *
 * \param prefs   The preferences.
 * \param method  The method we're calling.
 * \return  The URL. The caller should free it with g_free().
 */
G_GNUC_NULL_TERMINATED
gchar *imgur_prefs_get_url (ImgurPrefs *prefs,
	gchar *method,
	...);

#endif /* !PREFS_H */
