AC_INIT([rfsniffer], [0.1], [avp@avp.name])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
<<<<<<< HEAD

AC_LANG([C++])
AM_PROG_AR
#AC_PROG_CXX
#AM_PROG_LIBTOOL
LT_INIT
AC_CONFIG_MACRO_DIR([m4])

AC_CHECK_HEADERS([json/json.h])
AC_CHECK_HEADERS([jsoncpp/json/json.h])
AM_CONFIG_HEADER(haconfig.inc)
=======
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

AC_LANG([C++])
LT_INIT

#AC_LANG_CPLUSPLUS
#AC_PROG_CXX
#AM_PROG_LIBTOOL
#AM_PROG_AR

AC_DISABLE_SHARED

AC_CHECK_HEADERS([json/json.h])
AC_CHECK_HEADERS([jsoncpp/json/json.h])
>>>>>>> 5b7d8ec9b9d24cba58f1c7ea1affaf88fdc1a502

AC_DEFINE([RFSNIFFER],[1],[Wiren board rfm sniffer])

AC_CONFIG_FILES([
 Makefile
 libs/libutils/Makefile
 libs/librf/Makefile
 libs/libwb/Makefile
 tests/Makefile
 rfsniffer/Makefile
])

AC_OUTPUT
