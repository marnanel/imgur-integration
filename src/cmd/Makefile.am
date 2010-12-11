bin_PROGRAMS = imgur

INCLUDES = \
	$(DBUSGLIB_CFLAGS) \
	$(GLIB_CFLAGS)

imgur_SOURCES = \
	imgur-client-glue.h \
	imgur.c

imgur_LDADD = \
	$(DBUSGLIB_LIBS) \
	$(GLIB_LIBS)

imgur-client-glue.h: $(top_builddir)/data/imgur-service-glue.xml
        $(DBUSBINDINGTOOL) --mode=glib-client --output=$@ --prefix=imgur_service $^
BUILT_SOURCES = imgur-client-glue.h

