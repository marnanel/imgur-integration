#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <dbus/dbus-glib-bindings.h>

#include "imgur-upload.h"
#include "imgur-service-glue.h"
#include "upload.h"
#include "parse.h"
#include "prefs.h"
#include "recording.h"
#include "list-records.h"

enum
{
	LIST_CHANGED_SIGNAL,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

#define G_IMGUR_ERROR g_imgur_error_quark ()
GQuark
g_imgur_error_quark (void)
{
  return g_quark_from_static_string ("g-imgur-error-quark");
}

static void
imgur_upload_class_init (ImgurUploadClass *klass)
{
  GError *error = NULL;

  klass->connection = dbus_g_bus_get (DBUS_BUS_SESSION, &error);

  if (klass->connection==NULL)
    {
      g_warning ("Unable to connect to DBus: %s", error->message);
      g_error_free (error);
      return;
    }

  dbus_g_object_type_install_info (IMGUR_TYPE_UPLOAD,
     &dbus_glib_imgur_service_object_info);

  signals[LIST_CHANGED_SIGNAL] =
	  g_signal_new ("list_changed",
			  G_OBJECT_CLASS_TYPE (klass),
			  G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
			  0,
			  NULL, NULL,
			  g_cclosure_marshal_VOID__VOID,
			  G_TYPE_NONE, 0);

}

static void
imgur_upload_init (ImgurUpload *iu)
{
  GError *error = NULL;
  DBusGProxy *proxy;
  ImgurUploadClass *klass = IMGUR_UPLOAD_GET_CLASS (iu);
  guint ret;

  dbus_g_connection_register_g_object (klass->connection,
    "/com/imgur",
    G_OBJECT (iu));

  proxy = dbus_g_proxy_new_for_name (klass->connection,
    DBUS_SERVICE_DBUS,
    DBUS_PATH_DBUS,
    DBUS_INTERFACE_DBUS);

  if (!org_freedesktop_DBus_request_name (proxy,
    "com.imgur",
    0, &ret,
    &error))
    {
      g_warning ("Unable to register service: %s", error->message);
      g_error_free (error);
    }

  if (!dbus_g_proxy_call (proxy, "RequestName", &error,
        G_TYPE_STRING, "com.imgur", G_TYPE_UINT, 0,
        G_TYPE_INVALID,
        G_TYPE_UINT, &ret, G_TYPE_INVALID))
    g_error ("Failed to request name: %s", error->message);
 
  g_object_unref (proxy);

}

GType
imgur_upload_get_type (void)
{
  static GType object_type = 0;

  if (!object_type)
    {
      static const GTypeInfo object_info =
        {
          sizeof (ImgurUploadClass),
          (GBaseInitFunc) NULL,
          (GBaseFinalizeFunc) NULL,
          (GClassInitFunc) imgur_upload_class_init,
          NULL, NULL,
          sizeof (ImgurUpload),
          0,
          (GInstanceInitFunc) imgur_upload_init
        };

      object_type = g_type_register_static (G_TYPE_OBJECT, "ImgurUpload", &object_info, 0);
    }

  return object_type;
}

gboolean imgur_service_emit_list_changed_signal (ImgurUpload *iu, GError **error)
{
	g_signal_emit (iu, signals[LIST_CHANGED_SIGNAL], 0);
	return TRUE;
}

static void
hash_add_entry (GHashTable *hash, gchar *key, gchar *value)
{
  GValue *v = g_malloc0 (sizeof(GValue));
  g_value_init (v, G_TYPE_STRING);
  g_value_set_string (v, value);
  g_hash_table_insert (hash, g_strdup(key), v);
}

gboolean
imgur_service_upload (ImgurUpload *iu, gchar *filename, GHashTable **result,
	gchar **action_ret, GError **error)
{
  gboolean success;
  gchar *message = NULL;
  GList *parsed, *cursor;
  gchar *temp = NULL;
  ImgurPrefs *prefs = imgur_prefs_new ();

  *error = NULL;

  if (strncmp (filename, "file:", 5)==0)
    {
      filename += 5;
    }

  upload (prefs, filename, &success, &message, NULL);

  if (!success)
    {
       if (!message)
         {
            message = g_strdup ("Error in uploading.");
         }

       g_set_error (error,
                    G_IMGUR_ERROR,
                    1,
                    "%s", message);

       g_free (message);
       g_free (prefs);

       return FALSE;
    }

  parsed = parse_imgur_response (message, &success);

  if (success)
    {
       *result = g_hash_table_new (g_str_hash, g_str_equal);

       for (cursor=parsed; cursor; cursor = cursor->next)
         {
           if (temp)
             {
               hash_add_entry (*result, temp, cursor->data);
               g_free (cursor->data);
               g_free (temp);
               temp = NULL;
             }
           else
             {
               temp = cursor->data;
             }
          }

       if (prefs->recording)
       {
         imgur_recording_store (*result,
	        filename);
       }

       action_perform (prefs->action, filename, action_ret);

       g_list_free (parsed);
       g_free (message);
       g_free (prefs);

       return TRUE;
    }
  else
    {
       /* imgur sent us an error code */
       int code = 2;
       char *message = NULL;
       char *temp = NULL;

       for (cursor=parsed; cursor; cursor = cursor->next)
         {
           if (temp)
             {
               if (strcmp (temp, "error_msg")==0)
                 {
                   message = cursor->data;
                 }
               else if (strcmp (temp, "error_code")==0)
                 {
                   code = atoi (cursor->data);
                   g_free (cursor->data);
                 }
               else
                 {
                   g_free (cursor->data);
                 }

               g_free (temp);
               temp = NULL;
             }
           else
             {
               temp = cursor->data;
             }
         }

       g_free (temp);

       if (!message)
         {
           message = g_strdup_printf ("Error in hosting: %d.",
		code);
         }

       g_set_error (error,
                    G_IMGUR_ERROR,
                    code,
                    "%s", message);

       g_list_free (parsed);
       g_free (message);
       g_free (prefs);

       return FALSE;
    }
}

gboolean
imgur_service_list_records (ImgurUpload *iu, GPtrArray **result, GError **error)
{
	*result = imgur_list_records ();
	return TRUE;
}

gboolean
imgur_service_list_remote (ImgurUpload *iu, gboolean popular, GPtrArray **result, GError **error)
{
	/* STUB */
	*error = g_error_new (g_imgur_error_quark (),
		4,
		"Not yet implemented.");
	*result = NULL;
	return FALSE;
}

gboolean
imgur_service_get_record (ImgurUpload *iu, gchar *record, GHashTable **result, GError **error)
{
	gchar *last_dot = strrchr (record, '.');
	gchar *last_slash = strrchr (record, '/');
	gchar *temp = NULL;

	/*
 	 * Record IDs can be represented as filenames;
	 * if we get one of these, it must be canonicalised
	 * by removing the path and the extension.
	 */

	if (last_slash)
		record = last_slash+1;

	if (last_dot && last_dot > last_slash)
	{
		temp = g_strndup (record,
			last_dot - record);
	}

	*result = imgur_get_record (temp? temp: record);

	g_free (temp);

	return TRUE;
}

gboolean
imgur_service_forget_record (ImgurUpload *iu, gchar *record, GError **error)
{
	/* STUB */
	*error = g_error_new (g_imgur_error_quark (),
		4,
		"Not yet implemented.");
	return FALSE;
}

gboolean
imgur_service_delete_record (ImgurUpload *iu, gchar *record, GError **error)
{
	/* STUB */
	*error = g_error_new (g_imgur_error_quark (),
		4,
		"Not yet implemented.");
	return FALSE;
}

gboolean
imgur_service_stats_for_record (ImgurUpload *iu, gchar *record, GHashTable **result, GError **error)
{
	/* STUB */
	*error = g_error_new (g_imgur_error_quark (),
		4,
		"Not yet implemented.");
	*result = NULL;
	return FALSE;
}

gboolean
imgur_service_show_credits (ImgurUpload *iu, GHashTable **result, GError **error)
{
	/* STUB */
	*error = g_error_new (g_imgur_error_quark (),
		4,
		"Not yet implemented.");
	*result = NULL;
	return FALSE;
}

gboolean
imgur_service_log_in (ImgurUpload *iu, gboolean auth, GError **error)
{
	/* STUB */
	*error = g_error_new (g_imgur_error_quark (),
		4,
		"Not yet implemented.");
	return FALSE;
}

int
main (int argc, char **argv)
{
  GMainLoop *mainloop;
  GError *error = NULL;
  DBusGConnection *bus;
  DBusGProxy *bus_proxy;
  ImgurUpload *uploader;

  g_type_init ();

  dbus_g_object_type_install_info (IMGUR_TYPE_UPLOAD,
      &dbus_glib_imgur_service_object_info);

  mainloop = g_main_loop_new (NULL, FALSE);

  bus = dbus_g_bus_get (DBUS_BUS_SESSION, &error);

  if (!bus)
    {
      g_error ("Couldn't connect to session bus: %s", error->message);
    }

  bus_proxy = dbus_g_proxy_new_for_name (bus, "org.freedesktop.DBus",
                "/org/freedesktop/DBus", "org.freedesktop.DBus");

  uploader = g_object_new (IMGUR_TYPE_UPLOAD,
                           NULL);

  g_main_loop_run (mainloop);

}

/* EOF imgur-service.c */

