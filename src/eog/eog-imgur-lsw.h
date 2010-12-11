#ifndef EOG_IMGUR_LSW_H
#define EOG_IMGUR_LSW_H 1

#include <glib.h>

typedef void (LswListServicesCallback) (const GList *services);

void lsw_list_services (LswListServicesCallback *cb);

void lsw_post_to_service (gchar *service_name,
	gchar *message);

#endif

