#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "list-records.h"

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
	gchar *imgurdir, *temp;
	int fd;
	gchar *images[] = {
		"aa.jpg",
		"bb.png",
		"cc.jpg",
		"dd.jpg",
		NULL
	};
	gchar **cursor;

	fd = mkstemp (tempdir);

	if (fd==-1)
	{
		g_warning ("Could not create temporary file!");
		exit (255);
	}

	close (fd);
	unlink (tempdir);
	mkdir (tempdir, 0700);

	imgurdir = g_build_filename (tempdir,
			"imgur",
			NULL);
	mkdir (imgurdir, 0700);

	if (!g_setenv ("XDG_DATA_HOME", tempdir, TRUE))
	{
		g_warning ("Could not prepare environment!");
		exit (255);
	}

	for (cursor=images; *cursor; cursor++)
	{
		temp = g_build_filename (imgurdir,
			*cursor,
			NULL);

		if (!g_file_set_contents (temp,
			"(dummy)",
			-1, NULL))
		{
                	g_warning ("Could not create test file!");
                	exit (255);
		}

		g_free (temp);
	}

	temp = g_build_filename (imgurdir,
		"uploads.conf",
		NULL);
	if (!g_file_set_contents (temp,
		"[aa]\n"
		"time=100\n"
		"small_thumbnail=http://example.com/aa.jpg\n"
		"\n"
		"[dd]\n"
		"time=400\n"
		"small_thumbnail=http://example.com/dd.jpg\n"
		"\n"
		"[bb]\n"
		"time=200\n"
		"small_thumbnail=http://example.com/bb.png\n"
		"\n"
		"[cc]\n"
		"time=350\n"
		"small_thumbnail=http://example.com/cc.jpg\n"
		"\n",
		-1, NULL))
		{
                	g_warning ("Could not create test file!");
                	exit (255);
		}
	g_free (temp);

	g_print ("%s\n", tempdir);

	return tempdir;
}

static void
test1 (void)
{
	gchar **records = imgur_list_records();
	gchar *got = g_strdup("");
	gchar **cursor;

	for (cursor=records; *cursor; cursor++)
	{
		gchar *temp = got;
		gchar *last_slash = strrchr (*cursor, '/');

		if (last_slash)
		{
			last_slash++;
		}
		else
		{
			last_slash = *cursor;
		}

		got = g_strdup_printf ("%s[%s]",
			got,
			last_slash);
		g_free (temp);
	}

	test (got, "[dd.jpg][cc.jpg][bb.png][aa.jpg]", "Sort thumbnails");
	g_free (got);
}

int
main (int argc, char** argv)
{
	gchar *temp_dir = make_temp_dir ();
	
	test1 ();

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

