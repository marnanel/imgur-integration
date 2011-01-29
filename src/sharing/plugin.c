/*
 * This file is part of sharing-plugin-template
 *
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the root
 * directory.
 *
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <sharing-plugin-interface.h>
#include <sharing-transfer.h>
#include <conicconnection.h>
#include <osso-log.h>

#include <stdio.h>
#include <sharing-account.h>
#include <sharing-http.h>
#include <osso-log.h>

SharingPluginInterfaceAccountValidateResult
validate (SharingAccount* account,
          ConIcConnection* con, gboolean *cont, gboolean* dead_mans_switch)
{
	/* there are no accounts; it's always valid */
	return TRUE;
}

SharingPluginInterfaceSendResult
send (SharingTransfer* transfer,
      ConIcConnection* con, gboolean* dead_mans_switch)
{
    SharingEntry *entry = sharing_transfer_get_entry (transfer);

    for (GSList* p = sharing_entry_get_media (entry); p; p=g_slist_next(p))
    {
      SharingEntryMedia* media = p->data;

      if (!sharing_entry_media_get_sent (media))
      {
	/* send it */
	if (0)
	{
	  sharing_entry_media_set_sent (media, TRUE);
	}
      }

      *dead_mans_switch = 0; /* keepalive */
    }

    return SHARING_SEND_SUCCESS;
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
    SharingPluginInterfaceSendResult ret_val = SHARING_SEND_ERROR_UNKNOWN;
    ret_val = send (transfer, con, dead_mans_switch);
    return ret_val;
}

SharingPluginInterfaceAccountSetupResult
sharing_plugin_interface_account_setup (GtkWindow* parent,
		SharingService* service,
		SharingAccount** worked_on,
		osso_context_t* osso)
{
    return 0;
}

SharingPluginInterfaceAccountValidateResult
sharing_plugin_interface_account_validate (SharingAccount* account, 
    ConIcConnection* con, gboolean *cont, gboolean* dead_mans_switch)
{
    SharingPluginInterfaceAccountValidateResult ret_val = 0;
    ret_val = validate (account, con, cont, dead_mans_switch);
    return ret_val;
}

SharingPluginInterfaceEditAccountResult
sharing_plugin_interface_edit_account (GtkWindow* parent,
    SharingAccount* account, ConIcConnection* con, gboolean* dead_mans_switch)
{
    SharingPluginInterfaceEditAccountResult ret = 0;
    return ret;
}

