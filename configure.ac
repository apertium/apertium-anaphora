AC_PREREQ(2.61)

AC_INIT([apertium-anaphora], [1.1.1], [khanna.tanmai@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

CFLAGS="-Wall -Wextra $CFLAGS"
CXXFLAGS="-Wall -Wextra $CXXFLAGS"

PKG_CHECK_MODULES([LTTOOLBOX], [lttoolbox >= 3.7.1])
AC_SUBST(LTTOOLBOX_CFLAGS)
AC_SUBST(LTTOOLBOX_LIBS)

PKG_CHECK_MODULES([LIBXML], [libxml-2.0])
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

PKG_CHECK_MODULES([ICU], [icu-i18n icu-io icu-uc])
AC_SUBST(ICU_CFLAGS)
AC_SUBST(ICU_LIBS)

_found_utf8=no
for ipath in /usr /usr/local /opt /opt/local; do
  for upath in utf8cpp utfcpp utf8; do
    if test -f "$ipath/include/$upath/utf8.h"; then
      echo "found utfcpp at $ipath/include/$upath/utf8.h"
      CPPFLAGS="-I$ipath/include/$upath $CPPFLAGS"
      _found_utf8=yes
    fi
  done
done
if test $_found_utf8 = no
then
  AC_CHECK_HEADER([utf8cpp/utf8.h], [CPPFLAGS="-I/usr/include/utf8cpp/ $CPPFLAGS"], [
    AC_CHECK_HEADER([utf8.h], [], [AC_MSG_ERROR([Could not find utf8.h from utf8cpp / utfcpp])])
  ])
fi

CPPFLAGS="$CPPFLAGS $CFLAGS $LTTOOLBOX_CFLAGS $LIBXML_CFLAGS $ICU_CFLAGS"
LIBS="$LIBS $LTTOOLBOX_LIBS $LIBXML_LIBS $ICU_LIBS"

# Checks for highest supported C++ standard
AC_LANG(C++)
for version in 26 2c 23 2b 20 2a 17; do
  version_flag="-std=c++${version}"
  AX_CHECK_COMPILE_FLAG([${version_flag}], [break], [version_flag=none])
done
AS_IF([test "$version_flag" == none], [
  AC_MSG_ERROR([Could not enable at least C++17 - upgrade your compiler])
])
CXXFLAGS="$CXXFLAGS ${version_flag}"

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
