#include <string.h>
#include "action.h"

AfterwardsAction
action_from_string (const gchar *str)
{
	if (strcmp (str, "none")==0)
	{
		return ACTION_NONE;
	}
	else if (strcmp (str, "browser")==0)
	{
		return ACTION_BROWSER;
	}
	else if (strcmp (str, "gimgur")==0)
	{
		return ACTION_GIMGUR;
	}
	else if (strcmp (str, "copy")==0)
	{
		return ACTION_COPY;
	}
	else
	{
		/* go with the default */
		return ACTION_BROWSER;
	}
}

gchar*
action_to_string (AfterwardsAction action)
{
	switch (action)
	{
	case ACTION_NONE:
		return "none";
	case ACTION_GIMGUR:
		return "gimgur";
	case ACTION_COPY:
		return "copy";
	case ACTION_BROWSER:
	default:
		return "browser";
	}
}

void
action_perform (AfterwardsAction action,
	const gchar *url,
	gchar **response)
{
	gboolean success;
	gchar *command_line = NULL;

	switch (action)
	{
	case ACTION_NONE:
		/* that was easy! */
		success = TRUE;
		break;

	case ACTION_COPY:
		/* FIXME this should be taken from config.h */
		command_line =
			g_strdup_printf ("/usr/libexec/imgur-clipboard \"%s\"", url);

		success = g_spawn_command_line_async (command_line,
				NULL);

		/* FIXME also need to check its errorlevel */
		/* FIXME therefore needs to be synchronous */
		break;

	case ACTION_GIMGUR:
		/*
 		 * Until gimgur is implemented,
 		 * this is the same as "browser".
 		 */

		/* FALLTHROUGH */

	case ACTION_BROWSER:
	default:
		command_line =
			g_strdup_printf ("xdg-open %s", url);

		success = g_spawn_command_line_async (command_line,
				NULL);
		break;
	}
	
	g_free (command_line);

	if (response)
	{
		*response = g_strdup_printf (success? "%s": "no-%s",
			action_to_string (action));
	}
}

