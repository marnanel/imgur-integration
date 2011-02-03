#include <gtk/gtk.h>

int
main (int argc, char **argv)
{
	GtkClipboard *clipboard;

	if (!gtk_init_check (&argc, &argv))
	{
		g_print ("X was not found. Cannot paste.\n");
		return 255;
	}

	if (argc!=2)
	{
		g_print ("Please give a single argument, which "
			"will be copied to the clipboard.\n");
		return 1;
	}

	clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);

	if (!clipboard)
	{
		g_print ("Clipboard could not be found. Cannot paste.\n");
		return 255;
	}

	gtk_clipboard_set_text (clipboard,
		argv[1],
		-1);

	gtk_clipboard_store (clipboard);

	return 0;
}
