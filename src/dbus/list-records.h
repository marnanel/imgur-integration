#ifndef LIST_RECORDS_H
#define LIST_RECORDS_H 1

#include <glib.h>

/**
 * Returns a list of images we have previously
 * uploaded.
 *
 * \return  The list, terminated by NULL.
 *          Entries may be in the form of filenames;
 *          if so, the path and extension are irrelevant.
 */
GPtrArray* imgur_list_records (void);

/**
 * Returns the details of a given record.
 *
 * \param record  The identity of the record.
 *          It may be in the form of a filename;
 *          if so, the path and extension are irrelevant.
 * \return  The contents of the record, as a hash
 *          table mapping gchar*s to GValue*s.
 *          All GValue*s are strings.
 */
GHashTable* imgur_get_record (const gchar *record);

#endif /* !LIST_RECORDS_H */
