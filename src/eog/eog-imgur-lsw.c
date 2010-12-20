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

static void
lsw_post_to_service_cb (SwClientService *service,
	const GError *error,
	gpointer userdata)
{
	LswPostToServiceCallback *cb = userdata;

	cb (error);

	/* FIXME do we free the service or what */
}

void
lsw_post_to_service (gchar *service_name,
	gchar *status,
	LswPostToServiceCallback *cb)
{
	SwClient *client = sw_client_new ();
	SwClientService *service = sw_client_get_service (client,
                                        service_name);

	sw_client_service_update_status (service,
                                 lsw_post_to_service_cb,
                                 status,
                                 cb);

}

/* EOF eog-imgur-lsw.c */

