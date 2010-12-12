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

typedef struct {
	GtkWindow *parent;
	GtkWidget *textview;
	int character_limit;
	const gchar *service;
	EogImgurUiMessageCallback *callback;
} MessageDetails;

static void
message_response (GtkDialog *dialogue,
	gint arg1,
	MessageDetails *details)
{
	switch (arg1)
	  {
		case GTK_RESPONSE_OK:
		case GTK_RESPONSE_ACCEPT:
		  {
			GtkTextBuffer *buffer;
			gchar *text;
			GtkTextIter start, end;

			buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (details->textview));

			gtk_text_buffer_get_bounds (buffer,
				&start, &end);

			text = gtk_text_buffer_get_slice (buffer,
				&start, &end, TRUE);

			details->callback (
				GTK_WINDOW (details->parent),
				text,
				details->service);

			g_free (text);
			gtk_widget_destroy (GTK_WIDGET (dialogue));
		  }
		  break;

		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_REJECT:
		case GTK_RESPONSE_DELETE_EVENT:
			gtk_widget_destroy (GTK_WIDGET (dialogue));
			break;
	  }

	/* g_free (details); */
}

void
eog_imgur_ui_get_message (GtkWindow *parent,
	const gchar *service,
	gint character_limit,
	EogImgurUiMessageCallback *callback)
{
	GtkWidget *dialogue;
	GtkWidget *content;
	GtkWidget *label;
	GtkWidget *textview;
	MessageDetails *details = g_malloc (sizeof (MessageDetails));

	details->parent = parent;
	details->textview = gtk_text_view_new ();
	details->character_limit = character_limit;
	details->service = service;
	details->callback = callback;

	gtk_text_view_set_editable (GTK_TEXT_VIEW (details->textview), TRUE);
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW (details->textview), 10);
	gtk_text_view_set_right_margin (GTK_TEXT_VIEW (details->textview), 10);

	dialogue = gtk_dialog_new_with_buttons ("Eye of GNOME",
		parent,
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_STOCK_OK,
		GTK_RESPONSE_ACCEPT,
		GTK_STOCK_CANCEL,
		GTK_RESPONSE_REJECT,
		NULL);

	content = gtk_dialog_get_content_area (GTK_DIALOG (dialogue));

	label = gtk_label_new ("Enter any text you'd like to add beside the link.  "
		"(You may leave this blank and simply press OK.)");

	gtk_container_add (GTK_CONTAINER (content), label);
	gtk_container_add (GTK_CONTAINER (content), details->textview);

	g_signal_connect (dialogue,
		"response",
		G_CALLBACK (message_response),
		details);

	gtk_widget_show_all (dialogue);
}

void eog_imgur_ui_launch_browser (GtkWindow *parent,
	gchar *url)
{
	/* stub */
	eog_imgur_ui_display (parent, url, FALSE);
}


