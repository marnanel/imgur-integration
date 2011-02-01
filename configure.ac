AC_PREREQ([2.59])

AC_INIT([imgur], [0.5.1], [http://thomasthurman.org])

AC_CONFIG_MACRO_DIR([m4])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE([1.8 -Wno-portability])

dnl ----

dnl We don't want static libraries.
AC_DISABLE_STATIC()

dnl ----

dnl Is this Maemo?
dnl FIXME: This should be a separate m4 macro, really.

AC_MSG_CHECKING([for Maemo])
if test -f /etc/maemo_version; then

	dnl Yes, so don't build the eog plugin
	dnl and do build the libsharing plugin.

	AC_MSG_RESULT([yes])
	AC_DEFINE(MAEMO,1,[Running under Maemo])
	AC_SUBST([enable_maemo], [yes])

	AC_SUBST([default_eog], [no])
	AC_SUBST([default_libsharing], [yes])

else
	dnl No, so do build the eog plugin
	dnl and don't build the libsharing plugin.

	AC_MSG_RESULT([no])
	AC_SUBST([enable_maemo], [no])

	AC_SUBST([default_eog], [yes])
	AC_SUBST([default_libsharing], [no])

fi

dnl ----

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
  enable_eog=$default_eog)

AC_ARG_ENABLE(libsharing,
  AC_HELP_STRING([--enable-libsharing],
                 [build Maemo libsharing plugin]),,
  enable_libsharing=$default_libsharing)

dnl ----

AM_CONDITIONAL(WANT_EOG, test x$enable_eog = xyes)
AM_CONDITIONAL(WANT_LIBSHARING, test x$enable_libsharing != xno)
AM_CONDITIONAL(MAEMO, test x$enable_maemo = xyes)

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

if test x$enable_libsharing = xyes; then
    AC_DEFINE(WANT_LIBSHARING,1,[Build with libsharing support])

    PKG_CHECK_MODULES(LIBSHARING, [libsharing conic])
    AC_SUBST(LIBSHARING_CFLAGS)
    AC_SUBST(LIBSHARING_LIBS)

    PKG_CHECK_MODULES(HILDON, [hildon-1])
    AC_SUBST(HILDON_CFLAGS)
    AC_SUBST(HILDON_LIBS)

    dnl Note to my future self:
    dnl When we add the GUI client, GTK will need
    dnl to be included when we have EITHER that client
    dnl OR libsharing turned on.

    PKG_CHECK_MODULES(GTK, [gtk+-2.0])
    AC_SUBST(GTK_CFLAGS)
    AC_SUBST(GTK_LIBS)

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
	src/sharing/Makefile \
	data/Makefile \
	)

