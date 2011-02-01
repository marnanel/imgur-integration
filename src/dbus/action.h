#ifndef ACTION_H
#define ACTION_H 1

#include <glib.h>

/**
 * Actions to carry out after we successfully
 * upload a picture.
 */
typedef enum _AfterwardsAction {

/**
 * Do nothing.
 */
 ACTION_NONE,

/**
 * Launch the browser.
 */
 ACTION_BROWSER,

/**
 * Launch gimgur (the graphical imgur client).
 */
 ACTION_GIMGUR,

/**
 * Attempt to xdg-open the file
 * (or the local equivalent).
 */
 ACTION_OPEN,

/**
 * Copy the URL to the clipboard.
 */
 ACTION_COPY,

 ACTION_LAST
} AfterwardsAction;

AfterwardsAction action_from_string (const gchar *str);
gchar* action_to_string (AfterwardsAction action);
void action_perform (AfterwardsAction action, const gchar *filename);

#endif
