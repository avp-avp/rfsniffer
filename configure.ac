AC_INIT([rfsniffer], [0.1], [avp@avp.name])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
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

AC_DEFINE([RFSNIFFER],[1],[Wiren board rfm sniffer])

AC_CONFIG_FILES([
 Makefile
 libutils/Makefile
 librf/Makefile
 tests/Makefile
 rfsniffer/Makefile
])

AC_OUTPUT
