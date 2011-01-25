#include "list-records.h"

typedef struct _ListEntry
{
	gchar *filename;
	long time;
} ListEntry;

gchar**
imgur_list_records (void)
{
	gchar **result = NULL;

	g_print ("Stub.\n");
	return result;
}
