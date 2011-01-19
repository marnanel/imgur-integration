libexec_PROGRAMS = imgur-service

noinst_PROGRAMS = test-recording

imgur_service_SOURCES = \
	imgur-service.c \
	imgur-upload.h \
	upload.c \
	upload.h \
	parse.c \
	parse.h \
	imgur-service-glue.h

imgur-service-glue.h: $(top_builddir)/data/imgur-service-glue.xml
	$(DBUSBINDINGTOOL) --mode=glib-server --output=$@ --prefix=imgur_service $^
BUILT_SOURCES = imgur-service-glue.h

imgur_service_LDADD = \
	$(ALL_LIBS)

AM_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	@DBUSGLIB_CFLAGS@ \
	@GLIB_CFLAGS@ \
	@LIBCURL_CFLAGS@ \
	-Wall

ALL_LIBS = \
	@DBUSGLIB_LIBS@ \
	@GLIB_LIBS@ \
	@LIBCURL_LIBS@

test_recording_SOURCES = \
	recording.c \
	test-recording.c

test_recording_LDADD = \
	$(ALL_LIBS)

