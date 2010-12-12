#include "eog-imgur-ui.h"

void
eog_imgur_ui_display (GtkWindow *parent,
	const gchar *message,
	gboolean is_error)
{
	GtkWidget *dialogue = gtk_message_dialog_new (parent,
		GTK_DIALOG_DESTROY_WITH_PARENT,
		is_error? GTK_MESSAGE_ERROR: GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		"%s", message);

	g_signal_connect_swapped (dialogue,
		"response",
		G_CALLBACK (gtk_widget_destroy),
		dialogue);

	gtk_widget_show_all (dialogue);
}

void
eog_imgur_ui_get_message (GtkWindow *parent,
	gchar *service,
	gint character_limit,
	EogImgurUiMessageCallback *callback)
{
	/* stub */
	eog_imgur_ui_display (parent, service, FALSE);

#if 0
	GtkWidget *dialogue;
	GtkWidget *content;

	dialogue = gtk_dialog_new_with_buttons ("Eye of GNOME",
		parent,
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_STOCK_OK,
		GTK_RESPONSE_ACCEPT,
		NULL);

	content = gtk_dialog_get_content_area (GTK_DIALOG (dialogue));
#endif
}

void eog_imgur_ui_launch_browser (GtkWindow *parent,
	gchar *url)
{
	/* stub */
	eog_imgur_ui_display (parent, url, FALSE);
}


