AC_PREREQ([2.59])

AC_INIT([imgur], [0.3.0], [http://thomasthurman.org])

AC_CONFIG_MACRO_DIR([m4])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE([1.9 -Wno-portability])

AM_PROG_LIBTOOL
AM_CONFIG_HEADER(config.h)
AM_PROG_MKDIR_P

AC_PATH_PROG(DBUSBINDINGTOOL, dbus-binding-tool)
AC_SUBST(DBUSBINDINGTOOL)

AC_PROG_CC
AC_PROG_CC_STDC

dnl ----

dnl We don't want static libraries.
LT_INIT([disable-static])

dnl ----

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.4, gobject-2.0 >= 2.4])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(DBUSGLIB, [dbus-glib-1])
AC_SUBST(DBUSGLIB_CFLAGS)
AC_SUBST(DBUSGLIB_LIBS)

PKG_CHECK_MODULES(LIBCURL, [libcurl])
AC_SUBST(LIBCURL_CFLAGS)
AC_SUBST(LIBCURL_LIBS)

PKG_CHECK_MODULES(EOG, [eog])
AC_SUBST(EOG_CFLAGS)
AC_SUBST(EOG_LIBS)

PKG_CHECK_MODULES(LSWCLIENT, [libsocialweb-client])
AC_SUBST(LSWCLIENT_CFLAGS)
AC_SUBST(LSWCLIENT_LIBS)

AC_OUTPUT( Makefile \
	src/Makefile \
	src/dbus/Makefile \
	src/eog/Makefile \
	src/cmd/Makefile \
	data/Makefile \
	)

