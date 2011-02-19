#ifndef LIST_RECORDS_H
#define LIST_RECORDS_H 1

#include <glib.h>

/**
 * Returns a list of images we have previously
 * uploaded.
 *
 * \return  The list.
 */
GPtrArray* imgur_list_records (void);

/**
 * Returns the details of a given record.
 *
 * \param record  The identity of the record.
 * \return  The contents of the record, as a hash
 *          table mapping gchar*s to GValue*s.
 *          All GValue*s are strings.
 */
GHashTable* imgur_get_record (const gchar *record);

/**
 * Forgets the details of an existing record.
 * This does *not* delete it from the web.
 *
 * \param record  The identity of the record.
 * \return  True iff the forgetting was successful.
 */
gboolean imgur_forget_record (const gchar *record);

#endif /* !LIST_RECORDS_H */
