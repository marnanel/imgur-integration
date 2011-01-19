libexec_PROGRAMS = imgur-service

noinst_PROGRAMS = test-parse

imgur_service_SOURCES = \
	imgur-service.c \
	imgur-upload.h \
	upload.c \
	upload.h \
	parse.c \
	parse.h \
	imgur-service-glue.h

test_parse_SOURCES = \
	parse.c \
	parse.h \
	test-parse.c

imgur-service-glue.h: $(top_builddir)/data/imgur-service-glue.xml
	$(DBUSBINDINGTOOL) --mode=glib-server --output=$@ --prefix=imgur_service $^
BUILT_SOURCES = imgur-service-glue.h

imgur_service_LDADD = \
	$(ALL_LIBS)

test_parse_LDADD = \
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

