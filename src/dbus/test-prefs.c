#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "prefs.h"

int failed = 0;
int passed = 0;

static void
test (gchar *got, gchar *want, gchar *name)
{
	if (strcmp (got, want)==0)
	{
		g_print ("PASS: %s\n", name);
		passed++;
	}
	else
	{
		g_print ("FAIL: %s\nGot : %s\nWant: %s\n",
			name, got, want);
		failed++;
	}
}

static gchar *
make_temp_dir (void)
{
	gchar *tempdir = g_strdup_printf ("%s/imgurXXXXXX",
			g_get_tmp_dir ()
			);
	int fd;

	fd = mkstemp (tempdir);

	if (fd==-1)
	{
		g_warning ("Could not create temporary file!");
		exit (255);
	}

	close (fd);
	unlink (tempdir);
	mkdir (tempdir, 0700);
	if (!g_setenv ("XDG_CONFIG_HOME", tempdir, TRUE))
	{
		g_warning ("Could not prepare environment!");
		exit (255);
	}

	return tempdir;
}

static gchar*
prefs_as_string (ImgurPrefs *prefs)
{
	if (!prefs)
		return g_strdup ("NULL");

	return g_strdup_printf ("API=%s / Key = %s / Record = %d / "
		"Username = %s / Password = %s",
		prefs->api, prefs->key,
		prefs->recording,
		prefs->username, prefs->password);

}

static void
test1 (void)
{
	ImgurPrefs *prefs;
	gchar *temp;

	prefs = imgur_prefs_new ();

	temp = prefs_as_string (prefs);
	test (temp,
		"API=http://imgur.com/api/upload.xml / "
		"Key = 5a2d59b29160b55090e6bd9cd539519f / "
		"Record = 1 / Username = (null) / Password = (null)",
		"All defaults");
	g_free (temp);

	imgur_prefs_free (prefs);
}

static void
test2 (void)
{
	ImgurPrefs *prefs;
	gchar *temp;

	gchar *path = g_build_filename (g_getenv ("XDG_CONFIG_HOME"),
		"imgur",
		NULL);
	gchar *filename = g_build_filename (path,
		"imgur.conf",
		NULL);

	mkdir (path, 0700);

	if (!g_file_set_contents (filename,
		"# This is a comment.\n"
		"[general]\n"
		"api=A\n"
		"key=B\n"
		"recording=0\n"
		"username=C\n"
		"password=D\n",
		-1, NULL))
	{
		g_warning ("Could not create test file!");
		exit (255);
	}

	prefs = imgur_prefs_new ();

	temp = prefs_as_string (prefs);
	test (temp,
		"API=A / Key = B / Record = 0 / Username = C / Password = D",
		"Specific settings");
	g_free (temp);

	imgur_prefs_free (prefs);

	unlink (filename);
	g_free (filename);
	rmdir (path);
	g_free (path);
}

/* not pass/fail yet */
static void
test3 (void)
{
	ImgurPrefs *prefs;
	gchar *got;
	gchar *path = g_build_filename (g_getenv ("XDG_CONFIG_HOME"),
		"imgur",
		NULL);
	gchar *filename = g_build_filename (path,
		"imgur.conf",
		NULL);

	mkdir (path, 0700);

	if (!g_file_set_contents (filename,
		"# This is a comment.\n"
		"[general]\n"
		"api=http://wombat.example.com/foo/\n",
		-1, NULL))
	{
		g_warning ("Could not create test file!");
		exit (255);
	}

	prefs = imgur_prefs_new ();

	got = imgur_prefs_get_url (prefs,
		"badger",
		"fred", "bassett",
		"jim", "jam",
		"sheila", "bloke",
		NULL);

	g_print ("Got: %s\n", got);

	imgur_prefs_free (prefs);

	unlink (filename);
	g_free (filename);
	rmdir (path);
	g_free (path);
}

int
main (int argc, char** argv)
{
	gchar *temp_dir = make_temp_dir ();
	
	test1 ();
	test2 ();
	test3 ();

	rmdir (temp_dir);
	g_free (temp_dir);

	g_print ("Passed: %d. Failed: %d.\n",
		passed, failed);

	if (passed!=0 && failed==0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/* EOF test-prefs.c */

