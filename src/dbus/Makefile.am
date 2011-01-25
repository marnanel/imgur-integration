libexec_PROGRAMS = imgur-service

noinst_PROGRAMS = \
	test-parse \
	test-upload \
	test-recording \
	test-list-records \
	test-prefs

imgur_service_SOURCES = \
	imgur-service.c \
	imgur-upload.h \
	upload.c \
	upload.h \
	parse.c \
	parse.h \
	prefs.h \
	prefs.c \
	recording.h \
	recording.c \
	imgur-service-glue.h

test_parse_SOURCES = \
	parse.c \
	parse.h \
	test-parse.c

test_upload_SOURCES = \
	upload.c \
	upload.h \
	test-upload.c

test_prefs_SOURCES = \
	prefs.c \
	prefs.h \
	test-prefs.c

test_list_records_SOURCES = \
	list-records.c \
	list-records.h \
	test-list-records.c

imgur-service-glue.h: $(top_builddir)/data/imgur-service-glue.xml
	$(DBUSBINDINGTOOL) --mode=glib-server --output=$@ --prefix=imgur_service $^
BUILT_SOURCES = imgur-service-glue.h

imgur_service_LDADD = \
	$(ALL_LIBS)

test_parse_LDADD = \
	$(ALL_LIBS)

test_upload_LDADD = \
	$(ALL_LIBS)

test_prefs_LDADD = \
	$(ALL_LIBS)

test_list_records_LDADD = \
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

