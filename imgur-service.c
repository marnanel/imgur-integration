#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <dbus/dbus-glib-bindings.h>

#include "imgur-upload.h"
#include "imgur-service-glue.h"
#include "upload.h"
#include "parse.h"

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
}

static void
imgur_upload_init (ImgurUpload *iu)
{
  GError *error = NULL;
  DBusGProxy *proxy;
  ImgurUploadClass *klass = IMGUR_UPLOAD_GET_CLASS (iu);
  int ret;

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
imgur_service_upload (ImgurUpload *iu, gchar *filename, GHashTable **result, GError **error)
{
  gboolean success;
  gchar *message = NULL;
  GList *parsed, *cursor;
  gchar *temp = NULL;
  GValue *success_value;

  if (strncmp (filename, "file:", 5)==0)
    {
      filename += 5;
    }

  upload (filename, &success, &message, NULL);

  parsed = parse_imgur_response (message, &success);

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

  g_list_free (parsed);
  g_free (message);

  /* FIXME: This is wrong.  success==FALSE means error. */
  success_value = g_malloc0 (sizeof(GValue));
  g_value_init (success_value, G_TYPE_BOOLEAN);
  g_value_set_boolean (success_value, success);
  g_hash_table_insert (*result, g_strdup("success"), success_value);

  return TRUE;
}

int
main (int argc, char **argv)
{
  GMainLoop *mainloop;
  GError *error = NULL;
  DBusGConnection *bus;
  DBusGProxy *bus_proxy;
  ImgurUpload *uploader;
  int request_name_result;

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

  if (!dbus_g_proxy_call (bus_proxy, "RequestName", &error,
        G_TYPE_STRING, "com.imgur", G_TYPE_UINT, 0,
        G_TYPE_INVALID,
        G_TYPE_UINT, &request_name_result, G_TYPE_INVALID))
    g_error ("Failed to request name: %s", error->message);

  uploader = g_object_new (IMGUR_TYPE_UPLOAD,
                           NULL);

  dbus_g_connection_register_g_object (bus,
                                       "/com/imgur",
                                       G_OBJECT (uploader));

  g_main_loop_run (mainloop);

}

/* EOF imgur-service.c */

