#ifndef IMGUR_UPLOAD_H
#define IMGUR_UPLOAD_H 1

#include <glib-object.h>

#define IMGUR_TYPE_UPLOAD            (imgur_upload_get_type ())
#define IMGUR_UPLOAD(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), IMGUR_TYPE_UPLOAD, ImgurUpload))
#define IMGUR_IS_UPLOAD(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IMGUR_TYPE_UPLOAD))
#define IMGUR_UPLOAD_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), IMGUR_TYPE_UPLOAD, ImgurUploadClass))
#define IMGUR_IS_UPLOAD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), IMGUR_TYPE_UPLOAD))
#define IMGUR_UPLOAD_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), IMGUR_TYPE_UPLOAD, ImgurUploadClass))

typedef struct _ImgurUpload        ImgurUpload;
typedef struct _ImgurUploadClass   ImgurUploadClass;

struct _ImgurUpload
{
   GObject parent_instance;
};

struct _ImgurUploadClass
{
   GObjectClass parent_class;
   DBusGConnection *connection;
};

GType imgur_upload_get_type (void);

gboolean imgur_service_upload (ImgurUpload *upload, gchar *filename, GHashTable **result, GError **error);
gboolean imgur_service_list_records (ImgurUpload *upload, gchar ***result, GError **error);
gboolean imgur_service_get_record (ImgurUpload *upload, gchar *record, GHashTable **result, GError **error);

#endif
