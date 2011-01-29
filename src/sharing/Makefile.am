sharingdir = `$(PKG_CONFIG) libsharing --variable=pluginsdir`

sharing_LTLIBRARIES = libimgur.la

libimgur_la_SOURCES = plugin.c

libimgur_la_CFLAGS = $(DBUSGLIB_CFLAGS) $(GLIB_CFLAGS) $(LIBSHARING_CFLAGS) $(GTK_CFLAGS)
libimgur_la_LDFLAGS = -module -avoid-version -export-dynamic
libimgur_la_LIBADD = $(DBUSGLIB_LIBS) $(GLIB_LIBS) $(LIBSHARING_LIBS) $(GTK_LIBS)


