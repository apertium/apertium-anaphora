AC_PREREQ(2.61)

AC_INIT([apertium-anaphora], [1.0.1], [khanna.tanmai@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

CFLAGS="-Wall -Wextra $CFLAGS"
CXXFLAGS="-Wall -Wextra $CXXFLAGS"

PKG_CHECK_MODULES([LTTOOLBOX], [lttoolbox >= 3.5.3])
AC_SUBST(LTTOOLBOX_CFLAGS)
AC_SUBST(LTTOOLBOX_LIBS)

PKG_CHECK_MODULES([LIBXML], [libxml-2.0])
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

CPPFLAGS="$CPPFLAGS $CFLAGS $LTTOOLBOX_CFLAGS $LIBXML_CFLAGS"
LIBS="$LIBS $LTTOOLBOX_LIBS $LIBXML_LIBS"

# Checks for highest supported C++ standard
AC_LANG(C++)
AX_CHECK_COMPILE_FLAG([-std=c++20], [CXXFLAGS="$CXXFLAGS -std=c++20"], [
 AX_CHECK_COMPILE_FLAG([-std=c++2a], [CXXFLAGS="$CXXFLAGS -std=c++2a"], [
  AX_CHECK_COMPILE_FLAG([-std=c++17], [CXXFLAGS="$CXXFLAGS -std=c++17"], [
   AX_CHECK_COMPILE_FLAG([-std=c++1z], [CXXFLAGS="$CXXFLAGS -std=c++1z"], [
    AX_CHECK_COMPILE_FLAG([-std=c++14], [CXXFLAGS="$CXXFLAGS -std=c++14"], [
     AX_CHECK_COMPILE_FLAG([-std=c++1y], [CXXFLAGS="$CXXFLAGS -std=c++1y"], [])
    ])
   ])
  ])
 ])
])

AC_PATH_PROG(BASH, bash, no)
if test x$ac_cv_path_BASH = x
then
  AC_MSG_ERROR([You don't have bash installed.])
fi
if test x$ac_cv_path_BASH = xno
then
  AC_MSG_ERROR([You don't have bash installed.])
fi

AC_CONFIG_FILES([
                 apertium-anaphora.pc
                 Makefile
                 src/Makefile
                 ])
AC_OUTPUT
