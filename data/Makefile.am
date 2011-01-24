BUILT_FILES=com.imgur.service
EXTRA_DIST=com.imgur.service.in imgur-service-glue.xml

servicedir = $(datadir)/dbus-1/services
service_in_files = com.imgur.service.in
service_DATA = com.imgur.service
CLEANFILES = com.imgur.service

egconfdir = $(sysconfdir)/xdg/imgur
egconf_DATA = imgur.conf

com.imgur.service: com.imgur.service.in
	@sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@

imgur-client-glue.h: imgur-service-glue.xml
	$(DBUSBINDINGTOOL) --mode=glib-client --output=$@ --prefix=imgur_service $^
BUILT_SOURCES = imgur-client-glue.h

clean-local:
	rm -f imgur-client-glue.h
