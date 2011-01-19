#ifndef RECORDING_H
#define RECORDING_H 1

#include <glib.h>

/**
 * Records information about the thumbnail so
 * that we can find it later. If the storage
 * directory doesn't exist, does nothing (since
 * this allows users to turn it off easily).
 *
 * \param fields  A hash table mapping strings to
 *                GValues. Each will be put into
 *                the store. May not be NULL.
 *                If one key is "small_thumbnail",
 *                this thumbnail will also be
 *                downloaded.
 * \param filename The source filename. May be
 *                 NULL, in which case no source
 *                 filename will be stored.
 */
void eog_recording_store(GHashTable *fields,
			gchar *filename);

#endif /* !RECORDING_H */
