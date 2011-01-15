#ifndef EOG_IMGUR_UI_H
#define EOG_IMGUR_UI_H 1

#include <glib.h>
#include <gtk/gtk.h>

typedef void (EogImgurUiMessageCallback) (GtkWindow *parent,
	gchar *status_message,
	const gchar *service);

void eog_imgur_ui_display (GtkWindow *parent,
	const gchar *message,
	gboolean is_error);

void eog_imgur_ui_get_message (GtkWindow *parent,
	const gchar *service,
	gint character_limit,
	EogImgurUiMessageCallback *callback);

void eog_imgur_ui_launch_browser (GtkWindow *parent,
	const gchar *url);

#endif

