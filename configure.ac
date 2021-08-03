AC_PREREQ(2.61)

AC_INIT([apertium-anaphora], [1.1.0], [khanna.tanmai@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

CFLAGS="-Wall -Wextra $CFLAGS"
CXXFLAGS="-Wall -Wextra $CXXFLAGS"

PKG_CHECK_MODULES([LTTOOLBOX], [lttoolbox >= 3.6.0])
AC_SUBST(LTTOOLBOX_CFLAGS)
AC_SUBST(LTTOOLBOX_LIBS)

PKG_CHECK_MODULES([LIBXML], [libxml-2.0])
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

PKG_CHECK_MODULES([ICU], [icu-i18n icu-io icu-uc])
AC_SUBST(ICU_CFLAGS)
AC_SUBST(ICU_LIBS)

AC_CHECK_HEADERS([string_view])

CPPFLAGS="$CPPFLAGS $CFLAGS $LTTOOLBOX_CFLAGS $LIBXML_CFLAGS $ICU_CFLAGS"
LIBS="$LIBS $LTTOOLBOX_LIBS $LIBXML_LIBS $ICU_LIBS"

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

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <unicode/umachine.h>
#include <type_traits>
]],[[
using namespace std;
static_assert(!is_same<UChar,char16_t>::value, "UChar == char16_t");
   ]])], [AC_DEFINE([UCHAR_NOT_CHAR16_T], [1], [UChar != char16_t])])

AC_CONFIG_FILES([
                 apertium-anaphora.pc
                 Makefile
                 src/Makefile
                 ])
AC_OUTPUT
