if MAEMO
# Under Maemo, also install the placeholder app.
#
optdir = /opt/maemo
opt_DATA = \
	   imgur-maemo.html \
	   sharing-button.jpg
apphildondir = $(datadir)/applications/hildon
apphildon_DATA = \
		 imgur-maemo.desktop
servicesdir = `$(PKG_CONFIG) libsharing --variable=servicesdir`
services_DATA = imgur.service.xml

icondir = $(datadir)/icons/hicolor/48x48/hildon
icon_DATA = imgur.png

else
# Man pages. Don't attempt to install them under Maemo.
#
man_MANS = \
	imgur.1
endif

BUILT_FILES=com.imgur.service imgur.service.xml
EXTRA_DIST= \
	com.imgur.service.in \
	imgur-service-glue.xml \
	imgur.conf \
	imgur-maemo.html \
	sharing-button.jpg \
	imgur-maemo.desktop \
	$(man_MANS)

servicedir = $(datadir)/dbus-1/services
service_in_files = com.imgur.service.in
service_DATA = com.imgur.service
CLEANFILES = com.imgur.service

egconfdir = $(sysconfdir)/xdg/imgur
egconf_DATA = imgur.conf

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = imgur.png

com.imgur.service: com.imgur.service.in
	@sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@

imgur.service.xml: imgur.service.xml.in
	@sed -e "s|\@pixmapdir\@|$(pixmapdir)|" $< > $@

imgur-client-glue.h: imgur-service-glue.xml
	$(DBUSBINDINGTOOL) --mode=glib-client --output=$@ --prefix=imgur_service $^
BUILT_SOURCES = imgur-client-glue.h

clean-local:
	rm -f imgur-client-glue.h
