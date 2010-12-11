#include "eog-imgur-lsw.h"

#include <glib.h>
#include <libsocialweb-client/sw-client.h>

static void
lsw_list_services_cb (SwClient *client,
	const GList *stuff,
	gpointer userdata)
{
	LswListServicesCallback *callback = userdata;

	callback(stuff);
	g_object_unref (client);
}

void
lsw_list_services (LswListServicesCallback *callback)
{
	SwClient *client = sw_client_new ();

	sw_client_get_services (client,
		lsw_list_services_cb,
		callback);
}

/* EOF eog-imgur-lsw.c */

