#include "prefs.h"

int
main (int argc, char** argv)
{
	ImgurPrefs *prefs = imgur_prefs_new ();

	g_print ("API = %s\n", prefs->api);
	g_print ("Recording = %d\n", prefs->recording);
	g_print ("Username = %s\n", prefs->username);
	g_print ("Password = %s\n", prefs->password);

	imgur_prefs_free (prefs);
}

/* EOF test-prefs.c */

