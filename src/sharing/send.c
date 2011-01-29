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

#include <stdio.h>
#include <glib.h>
#include <osso-log.h>
#include <sharing-http.h>
#include "send.h"
#include "common.h"

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

