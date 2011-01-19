if WANT_EOG

pluginsdir = $(libdir)/eog/plugins

EXTRA_DIST = imgur.eog-plugin

INCLUDES = \
	$(GLIB_CFLAGS) \
	$(EOG_CFLAGS) \
	$(DBUSGLIB_CFLAGS) \
	$(LSWCLIENT_CFLAGS)

libimgur_la_SOURCES = \
	imgur-client-glue.h \
	eog-imgur-plugin.c \
	eog-imgur-plugin.h \
	eog-imgur-lsw.c \
	eog-imgur-lsw.h \
	eog-imgur-ui.c \
	eog-imgur-ui.h \
	eog-imgur-post.c \
	eog-imgur-post.h

libimgur_la_LIBADD = \
	$(EOG_LIBS) \
	$(LSWCLIENT_LIBS)

plugins_DATA = \
	imgur.eog-plugin

AM_LDFLAGS = -avoid-version -module

plugins_LTLIBRARIES = \
	libimgur.la

endif
