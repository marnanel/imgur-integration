/* 
 * Copyright (C) 2010 Thomas Thurman <tthurman@gnome.org>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <glib.h>
#include <curl/curl.h>
#include "upload.h"

/*
 * you should possibly get your own
 */
char *api_key = "5a2d59b29160b55090e6bd9cd539519f";

#define IMGUR_DEFAULT_URL "http://imgur.com/api/upload.xml"

static size_t
save_data (void *ptr, size_t size, size_t nmemb, void *data)
{
  char **details = (char **) data;
  size_t bytes = size*nmemb;
  char *old_details = *details;

  if (old_details)
    {
      *details = g_strdup_printf ("%s%*s",
				  old_details,
				  bytes,
				  (char*) ptr);
    }
  else
    {
      *details = g_strdup_printf ("%*s",
				  bytes,
				  (char*) ptr);
    }

  if (old_details)
    {
      g_free (old_details);
    }

  return bytes;
}

#if 0
/* Temporarily not in use. */
static int
report_progress (gboolean (*progress)(char),
		 double dltotal,
		 double dlnow,
		 double ultotal,
		 double ulnow)
{
  char percent = ulnow*100.0/ultotal;

  if (progress (percent))
    return 0; /* it's all good */
  else
    return 1; /* abort */
}
#endif

void
upload (gchar *filename,
	gboolean *success,
	gchar **result,
	gboolean (*progress)(char))
{
  static const char* imgur_url = NULL;
  CURL *curl;
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;

  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "image",
               CURLFORM_FILE, filename,
               CURLFORM_END);
 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
               CURLFORM_COPYCONTENTS, api_key,
               CURLFORM_END);
 
  curl = curl_easy_init();

  if (!curl)
    {
      *success = FALSE;
      *result = g_strdup_printf ("Could not initialise HTTP library.");
      return;
    }

  if (!imgur_url)
    {
      imgur_url = g_getenv ("IMGUR_URL");

      if (!imgur_url)
        imgur_url = IMGUR_DEFAULT_URL;
    }

  *result = NULL;

  curl_easy_setopt (curl, CURLOPT_URL,
		   imgur_url);
  curl_easy_setopt (curl, CURLOPT_HTTPPOST,
		   formpost);
  curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION,
		   save_data);
  curl_easy_setopt (curl, CURLOPT_WRITEDATA,
		   (void*) result); 

  if (g_getenv ("IMGUR_VERBOSE"))
    {
      curl_easy_setopt (curl, CURLOPT_VERBOSE,
		   TRUE);
    }

#if 0
  /* We were using this, but not at present */
  curl_easy_setopt (curl, CURLOPT_PROGRESSFUNCTION,
		    report_progress);
  curl_easy_setopt (curl, CURLOPT_PROGRESSDATA,
		    progress);
#endif

  curl_easy_perform (curl);

  /* tidy up */
  curl_easy_cleanup(curl);
  curl_formfree(formpost);

  if (g_getenv ("IMGUR_VERBOSE"))
    {
      g_print ("%s\n", *result);
    }

  if (result==NULL)
    {
      *success = FALSE;
      *result = g_strdup_printf ("Could not upload to the server.");
      return;
    }

  *success = TRUE;
}

#ifdef TEST
static gboolean
progress (char percent)
{
  g_warning("Progress: %d%%", percent);
  return TRUE;
}

int
main()
{
  gchar *filename = "lsm.jpg";
  gchar *result;
  gboolean success;

  upload (filename,
	  &success,
	  &result,
	  progress);

  g_warning ("Upload: %d %s", success, result);
}
#endif
