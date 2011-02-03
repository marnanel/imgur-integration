#include <glib.h>
#include <glib-object.h>
#include "fetch.h"

int
main (int argc, char **argv)
{
	GError *error = NULL;

	g_type_init ();

	g_print ("[%s]\n",
		fetch_url ("http://www.chiark.greenend.org.uk/~martinh/poems/SECRET",
			&error));
}
