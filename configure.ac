dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta - will be overwritten
dnl If you don't want it to overwrite it,
dnl 	Please disable it in the Anjuta project configuration

AC_INIT(configure.ac)
AM_INIT_AUTOMAKE(berusky2, 0.10)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

AC_PROG_CPP
AC_PROG_CXX

GETTEXT_PACKAGE=berusky2
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Package name for gettext])

dnl Languages which your application supports
ALL_LINGUAS=""
AM_GLIB_GNU_GETTEXT

AM_PROG_LIBTOOL

PKG_CHECK_MODULES(SDL, [sdl SDL_image])
AC_SUBST(SDL_CFLAGS)
AC_SUBST(SDL_LIBS)

PKG_CHECK_MODULES(OPENGL, [gl glu])
AC_SUBST(OPENGL_CFLAGS)
AC_SUBST(OPENGL_LIBS)

PKG_CHECK_MODULES(X11, [x11])
AC_SUBST(X11_CFLAGS)
AC_SUBST(X11_LIBS)

PKG_CHECK_MODULES(ZLIB, [zlib])
AC_SUBST(ZLIB_CFLAGS)
AC_SUBST(ZLIB_LIBS)

PKG_CHECK_MODULES(SOUND, [openal freealut vorbisfile])
AC_SUBST(SOUND_CFLAGS)
AC_SUBST(SOUND_LIBS)

CFLAGS="$CFLAGS $SDL_CFLAGS $OPENGL_CFLAGS $X11_CFLAGS $SOUND_CFLAGS $ZLIB_CFLAGS"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS $OPENGL_CFLAGS $X11_CFLAGS $SOUND_CFLAGS $ZLIB_CFLAGS"
LDFLAGS="$LDFLAGS $SDL_LIBS $OPENGL_LIBS $X11_LIBS $SOUND_LIBS $ZLIB_LIBS"

# https://github.com/stransky/berusky2/issues/23
LDFLAGS="$LDFLAGS -lpthread"

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)

AC_ARG_WITH([user_directory],
AS_HELP_STRING([--with-user-directory=DIR], [User directory location (default is ~/.berusky2)]))
if test "x$with_user_directory" != x; then
  AC_DEFINE_UNQUOTED([INI_USER_DIRECTORY], [$with_user_directory], [Default user directory location.])
else
  AC_DEFINE_UNQUOTED([INI_USER_DIRECTORY], ["~/.berusky2"], [Default user directory location.])
fi

AC_ARG_WITH([saves_directory],
AS_HELP_STRING([--with-saves-directory=DIR], [Game saves dir (default is ~/.berusky2/Save)]))
if test "x$with_saves_directory" != x; then
  AC_DEFINE_UNQUOTED([INI_SAVES_DIRECTORY], [$with_saves_directory], [Game saves dir])
else
  AC_DEFINE_UNQUOTED([INI_SAVES_DIRECTORY], ["~/.berusky2/Save"], [Game saves dir])
fi

AC_ARG_WITH([profile_directory],
AS_HELP_STRING([--with-profile-directory=DIR], [User profiles dir (default is ~/.berusky2/Save/profiles)]))
if test "x$with_profile_directory" != x; then
  AC_DEFINE_UNQUOTED([INI_PROFILE_DIRECTORY], [$with_profile_directory], [User profile dir.])
else
  AC_DEFINE_UNQUOTED([INI_PROFILE_DIRECTORY], ["~/.berusky2/Save/profiles"], [User profile dir.])
fi

AC_ARG_WITH([ini_file],
AS_HELP_STRING([--with-ini-file=NAME], [Define ini file name (default is berusky3d.ini)]))
if test "x$with_ini_file" != x; then
  AC_DEFINE_UNQUOTED([INI_FILE_NAME], [$with_ini_file], [Game ini file])  
else
  AC_DEFINE_UNQUOTED([INI_FILE_NAME], ["berusky3d.ini"], [Game ini file])
fi
  
AC_ARG_WITH([ini_file_global_dir],
AS_HELP_STRING([--with-ini-file-global-dir=DIR], [Location of global ini file (default is /var/games/berusky2)]))
if test "x$with_ini_file_global_dir" != x; then
  AC_DEFINE_UNQUOTED([INI_FILE_GLOBAL_DIR], [$with_ini_file_global_dir], [Global ini file dir.])
else
  AC_DEFINE_UNQUOTED([INI_FILE_GLOBAL_DIR], ["/var/games/berusky2"], [Global ini file dir.])
fi

dnl Set PACKAGE_LOCALE_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${ac_default_prefix}/${DATADIRNAME}/locale", [Package local directory])
else
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${prefix}/${DATADIRNAME}/locale", [Package local directory])
fi

dnl Set PACKAGE SOURCE DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`

dnl Set PACKAGE PREFIX
if test "x${prefix}" = "xNONE"; then
  packageprefix=${ac_default_prefix}
else
  packageprefix=${prefix}
fi

dnl Set PACKAGE DATA & DOC DIR
packagedatadir=share
packagedocdir=doc/${PACKAGE}

dnl Set PACKAGE DIRS in config.h.
packagepixmapsdir=${packagedatadir}/pixmaps
packagehelpdir=${packagedatadir}/help
packagemenudir=${packagedatadir}

dnl Subst PACKAGE_DATA_DIR.
NO_PREFIX_PACKAGE_DATA_DIR="${packagedatadir}"
AC_SUBST(NO_PREFIX_PACKAGE_DATA_DIR)
PACKAGE_DATA_DIR="${packageprefix}/${packagedatadir}"
AC_SUBST(PACKAGE_DATA_DIR)

dnl Subst PACKAGE_DOC_DIR.
NO_PREFIX_PACKAGE_DOC_DIR="${packagedocdir}"
AC_SUBST(NO_PREFIX_PACKAGE_DOC_DIR)
PACKAGE_DOC_DIR="${packageprefix}/${packagedocdir}"
AC_SUBST(PACKAGE_DOC_DIR)

dnl Subst PACKAGE_PIXMAPS_DIR.
NO_PREFIX_PACKAGE_PIXMAPS_DIR="${packagepixmapsdir}"
AC_SUBST(NO_PREFIX_PACKAGE_PIXMAPS_DIR)
PACKAGE_PIXMAPS_DIR="${packageprefix}/${packagepixmapsdir}"
AC_SUBST(PACKAGE_PIXMAPS_DIR)

dnl Subst PACKAGE_HELP_DIR.
NO_PREFIX_PACKAGE_HELP_DIR="${packagehelpdir}"
AC_SUBST(NO_PREFIX_PACKAGE_HELP_DIR)
PACKAGE_HELP_DIR="${packageprefix}/${packagehelpdir}"
AC_SUBST(PACKAGE_HELP_DIR)

dnl Subst PACKAGE_MENU_DIR.
NO_PREFIX_PACKAGE_MENU_DIR="${packagemenudir}"
AC_SUBST(NO_PREFIX_PACKAGE_MENU_DIR)
PACKAGE_MENU_DIR="${packageprefix}/${packagemenudir}"
AC_SUBST(PACKAGE_MENU_DIR)

AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${packageprefix}/${packagedatadir}")
AC_DEFINE_UNQUOTED(PACKAGE_DOC_DIR, "${packageprefix}/${packagedocdir}")
AC_DEFINE_UNQUOTED(PACKAGE_PIXMAPS_DIR, "${packageprefix}/${packagepixmapsdir}")
AC_DEFINE_UNQUOTED(PACKAGE_HELP_DIR, "${packageprefix}/${packagehelpdir}")
AC_DEFINE_UNQUOTED(PACKAGE_MENU_DIR, "${packageprefix}/${packagemenudir}")
AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}")

AC_OUTPUT([
Makefile
po/Makefile.in
src/Makefile
data/Makefile
])
