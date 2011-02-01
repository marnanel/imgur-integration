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
	const gchar *filename)
{
	g_warning ("Not implemented.");
}

