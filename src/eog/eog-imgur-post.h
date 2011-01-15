#ifndef EOG_IMGUR_POST
#define EOG_IMGUR_POST 1

#include <glib.h>

/**
 * Callback from eog_imgur_post().
 *
 * \param success   TRUE if it worked; FALSE if it failed.
 * \param result    The URL if success==TRUE; an error message
 *                  to display if success==FALSE.
 * \param user_data Arbitrary data as passed to eog_imgur_post().
 */
typedef void (EogImgurPostCallback) (gboolean success,
	const gchar *result,
	gpointer user_data);

/**
 * Posts an image to imgur.com.
 *
 * \param filename  Path to the image on disk.
 * \param callback  A callback to call when we succeed (or fail).
 * \param user_data Arbitrary data.
 */
void eog_imgur_post (gchar *filename,
	EogImgurPostCallback *callback,
	gpointer user_data);

#endif

