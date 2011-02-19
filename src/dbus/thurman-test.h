/*
 * This is a small part of a test framework I'm working on.
 * Documentation is also being worked on.
 */

#ifndef THURMAN_TEST
#define THURMAN_TEST 1

#include <glib.h>

typedef enum _ThurmanTestFlags {
	THURMAN_TEST_IS_UNIT = 1 << 0,
	THURMAN_TEST_IS_INTEGRATION = 1 << 1,
	THURMAN_TEST_IS_REGRESSION = 1 << 2,

	THURMAN_TEST_EXPECT_FAIL = 1 << 3
} ThurmanTestFlags;

typedef gboolean (ThurmanTest) (void);

gboolean is_verbose (void);
gboolean strings_match (gchar *want, gchar *got);

void add_test (gchar *test_name,
	ThurmanTestFlags flags,
	ThurmanTest the_test,
	gchar *description);

#endif
