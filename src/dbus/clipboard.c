#include <gtk/gtk.h>

/*
 * FIXME: This needs to return an error value
 * for all possible error conditions.
 */

int
main (int argc, char **argv)
{
	GtkClipboard *clipboard;

	gtk_init (&argc, &argv);

	if (argc!=2)
	{
		g_print ("Please give a single argument, which "
			"will be copied to the clipboard.\n");
		return 1;
	}

	clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);

	g_print ("Clipboard is %p\n", clipboard);

	gtk_clipboard_set_text (clipboard,
		argv[1],
		-1);

	gtk_clipboard_store (clipboard);

	return 0;
}
