#include "thurman-test.h"
#include <string.h>

gboolean verbose = FALSE;
gboolean run_tests = FALSE;
gboolean count_tests = FALSE;
gchar *run_specific_test = NULL;

extern int test_main (int, char**);

int test_count = 0;
int pass_count = 0;

gboolean
is_verbose (void)
{
	return verbose;
}

gboolean
strings_match (gchar *want, gchar *got)
{
	gboolean result = strcmp (want, got)==0;

	if (!result && is_verbose())
	{
		g_print ("Want: %s\nGot :%s\n",
			want, got);
	}

	return result;
}

void
add_test (gchar *test_name,
        ThurmanTestFlags flags,
        ThurmanTest the_test,
	gchar *description)
{
	if (count_tests)
	{
		gchar *letters = "UIRF";
		gint i = 0;

		/* FIXME should probably run these tests on description too */

		if (index (test_name, ',') ||
			index (test_name, '\n'))
		{
			g_error ("Forbidden characters in test name: %s\n",
				test_name);
		}

		g_print ("TEST,%s,", test_name);

		while (letters[i])
		{
			if (flags & 1<<i)
				g_print ("%c", letters[i]);
			i++;
		}

		g_print (",%s\n",
			description? description: "test");
	}

	if (run_tests &&
		(!run_specific_test ||
		strcmp (run_specific_test, test_name)==0))
	{
		gboolean result = the_test ();

		g_print ("%s,%s\n",
			result? "PASS": "FAIL",
			test_name);

		if (flags && THURMAN_TEST_EXPECT_FAIL)
			result = !result;

		if (result)
			pass_count++;

		test_count++;
	}
}

static GOptionEntry entries[] =
{
	{ "count", 'c', 0, G_OPTION_ARG_NONE, &count_tests, "Print details of tests, but don't run them", NULL },
	{ "run", 'r', 0, G_OPTION_ARG_NONE, &run_tests, "Run the tests", NULL },
	{ "test", 't', 0, G_OPTION_ARG_STRING, &run_specific_test, "Run a specific test", "TESTNAME" },
	{ "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Print more detail", NULL },
	{ NULL }
};
GOptionContext *context;

static void
show_usage (void)
{
	gchar *options;
	g_print ("This program uses a common testing library.\n");
	g_print ("You should probably be running it using the test harness\n");
	g_print ("(when one is written).\n");
	g_print ("\n");

	options = g_option_context_get_help (context, FALSE, NULL);
	g_print ("Options are:\n%s\n",
		options);
	g_free (options);
}

int
main (int argc, char** argv)
{
	GError *error = NULL;

	context = g_option_context_new ("- a test program");
	g_option_context_add_main_entries (context, entries, "");
	if (!g_option_context_parse (context, &argc, &argv, &error))
	{
		g_print ("option parsing failed: %s\n", error->message);
		show_usage ();
		return 1;
	}

	if (!run_tests && !count_tests)
	{
		show_usage ();
		return 1;
	}

	/* Should we do something with the result of test_main? */
	test_main (argc, argv);

	if (run_tests)
	{
		if (test_count==0)
		{
			g_print ("You asked to run tests, but none were run\n");
		}
		else if (verbose)
		{
			g_print ("Run: %d.  Passed: %d.\n",
				test_count, pass_count);
		}
	}
	return pass_count;
}

