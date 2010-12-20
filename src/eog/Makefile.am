pluginsdir = $(libdir)/eog/plugins

INCLUDES = \
	$(GLIB_CFLAGS) \
	$(EOG_CFLAGS) \
	$(LSWCLIENT_CFLAGS)

libimgur_la_SOURCES = \
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

