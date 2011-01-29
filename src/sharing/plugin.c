/*
 * This was originally based on Maemo example code that was
 * copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 * That code was licensed under an MIT-style licence.
 */

/* FIXME not all of these are necessary */
#include <gtk/gtk.h>
#include <glib.h>
#include <sharing-plugin-interface.h>
#include <sharing-transfer.h>
#include <conicconnection.h>
#include <osso-log.h>
#include <hildon/hildon.h>
#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <sharing-account.h>
#include <sharing-http.h>
#include <osso-log.h>
#include "../../data/imgur-client-glue.h"

static gboolean
launch_browser (DBusGConnection *connection,
		const gchar *url)
{
	GError *error = NULL;
	DBusGProxy *proxy = dbus_g_proxy_new_for_name (connection,
			"com.nokia.osso_browser",
			"/com/nokia/osso_browser/request",
			"com.nokia.osso_browser");

	if (!dbus_g_proxy_call (proxy, "load_url", &error,
				G_TYPE_STRING, url,
				G_TYPE_INVALID,
				G_TYPE_INVALID))
	{
		/* do something to complain about error->message */
		g_error_free (error);
		return FALSE;
	}

	return TRUE;
}

static DBusGConnection*
get_connection (void)
{
	DBusGConnection *connection;
	GError *error = NULL;

	DBusGProxy *proxy;

	connection = dbus_g_bus_get (DBUS_BUS_SESSION,
			&error);
	if (connection == NULL)
	{
		g_warning ("Error in getting connection: %s",
				error->message);
		g_error_free (error);
		return NULL;
	}

	return connection;
}

guint
sharing_plugin_interface_init (gboolean* dead_mans_switch)
{
    return 0;
}

guint
sharing_plugin_interface_uninit (gboolean* dead_mans_switch)
{
    return 0;
}

SharingPluginInterfaceSendResult
sharing_plugin_interface_send (SharingTransfer* transfer, ConIcConnection* con,
    gboolean* dead_mans_switch)
{
	SharingEntry *entry = sharing_transfer_get_entry (transfer);
	DBusGConnection* connection = get_connection ();
	GSList* p;
	DBusGProxy *uploader = NULL;
	GError *error = NULL;

	if (!connection)
	{
		return SHARING_SEND_ERROR_UNKNOWN;
	}

	uploader = dbus_g_proxy_new_for_name (connection,
			"com.imgur", 
			"/com/imgur",
			"com.imgur");
	
	for (p=sharing_entry_get_media (entry); p; p=g_slist_next (p))
	{
		SharingEntryMedia* media = p->data;
		gchar *url = NULL;
		GHashTable *result = NULL;
		const gchar *filename =
			sharing_entry_media_get_localpath (media);
	
		if (sharing_entry_media_get_sent (media))
			continue; /* it was already sent */

		/* Great, so let's send it. */

		if (com_imgur_upload (uploader, filename, &result, &error))
		{
			GValue *url_v = g_hash_table_lookup (result, "imgur_page");

			if (url_v)
				url = g_strdup (g_value_get_string (url_v));

			if (result)
				g_hash_table_unref (result);
		}
		else
		{
			g_warning ("Error in upload: %s", error->message);
			g_error_free (error);
			return SHARING_SEND_ERROR_UNKNOWN;
		}

		if (url)
		{
			/* looks like a successful operation */
	
			/*
			 * At some point we may want a configuration
			 * setting that means that the libsharing
			 * plugin does *not* launch the browser.
			 * But this will only become useful when
			 * there is a GUI app to look at existing
			 * uploads.
			 */
	
			if (!launch_browser (connection, url))
			{
				g_free (url);
				return SHARING_SEND_ERROR_UNKNOWN;
			}

			g_free (url);
			sharing_entry_media_set_sent (media, TRUE);
		}
		else
		{
			return SHARING_SEND_ERROR_UNKNOWN;
		}

		*dead_mans_switch = 0; /* keepalive */
	}

	/* FIXME: don't we want to unref the connection? */
	
	return SHARING_SEND_SUCCESS;
}

static void
set_account_name (SharingAccount *sa)
{
	/*
	 * Of course we don't really have usernames
	 * (at least, not in the default case), but
	 * it makes sense to set something so that
	 * it appears in dialogues.
	 */
	sharing_account_set_username (sa,
			"imgur.com anonymous upload");
}

/*
 * This is called when they first add us.
 */
SharingPluginInterfaceAccountSetupResult
sharing_plugin_interface_account_setup (GtkWindow* parent,
		SharingService* service,
		SharingAccount** worked_on,
		osso_context_t* osso)
{
	/*
	 * Unfortunately, there doesn't seem to be
	 * a way of telling Maemo not to put us
	 * through the validation step.
	 */

	set_account_name (worked_on);

	return SHARING_ACCOUNT_SETUP_SUCCESS;
}

SharingPluginInterfaceAccountValidateResult
sharing_plugin_interface_account_validate (SharingAccount* account, 
    ConIcConnection* con, gboolean *cont, gboolean* dead_mans_switch)
{
	/* there are no accounts; it's always valid */
	*cont = TRUE;
	return SHARING_ACCOUNT_VALIDATE_SUCCESS;
}

SharingPluginInterfaceEditAccountResult
sharing_plugin_interface_edit_account (GtkWindow* parent,
    SharingAccount* account, ConIcConnection* con, gboolean* dead_mans_switch)
{
	set_account_name (account);
	return SHARING_EDIT_ACCOUNT_SUCCESS;
}

gboolean
sharing_plugin_interface_update_options (SharingAccount *account,
		ConIcConnection *con,
		gboolean *cont,
		gboolean *dead_mans_switch,
		UpdateOptionsCallback cb_func,
		gpointer cb_data)
{
	*cont = TRUE;
	if (cb_func)
	{
		cb_func (SHARING_UPDATE_OPTIONS_SUCCESS, cb_data);
	}
	return TRUE;
}

/* EOF plugin.c */

