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

AC_ARG_ENABLE(libsocialweb,
  AC_HELP_STRING([--enable-libsocialweb],
                 [(attempt to) use libsocialweb for posting image links]),,
  enable_libsocialweb=no)

AC_ARG_ENABLE(eog,
  AC_HELP_STRING([--disable-eog],
                 [don't build the Eye of Gnome plugin]),,
  enable_eog=yes)

dnl ----

dnl We don't want static libraries.
LT_INIT([disable-static])

dnl ----

AM_CONDITIONAL(WANT_EOG, test x$enable_eog = xyes)

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.4, gobject-2.0 >= 2.4])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(DBUSGLIB, [dbus-glib-1, dbus-1])
AC_SUBST(DBUSGLIB_CFLAGS)
AC_SUBST(DBUSGLIB_LIBS)

PKG_CHECK_MODULES(LIBCURL, [libcurl])
AC_SUBST(LIBCURL_CFLAGS)
AC_SUBST(LIBCURL_LIBS)

if test x$enable_eog = xyes; then
    AC_DEFINE(WANT_EOG,1,[Build with eog support])

    PKG_CHECK_MODULES(EOG, [eog])
    AC_SUBST(EOG_CFLAGS)
    AC_SUBST(EOG_LIBS)
fi

if test x$enable_libsocialweb = xyes; then
	AC_DEFINE(HAVE_LIBSOCIALWEB,1,[Build with libsocialweb support])

	PKG_CHECK_MODULES(LSWCLIENT, [libsocialweb-client])
	AC_SUBST(LSWCLIENT_CFLAGS)
	AC_SUBST(LSWCLIENT_LIBS)
fi

AC_OUTPUT( Makefile \
	src/Makefile \
	src/dbus/Makefile \
	src/eog/Makefile \
	src/cmd/Makefile \
	data/Makefile \
	)

