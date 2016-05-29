AM_CPPFLAGS = -I../include/
bin_PROGRAMS = rftest
rftest_SOURCES = LogTest.cpp RfParserTest.cpp main.cpp rfm69Test.cpp snifferTest.cpp mqttTest.cpp
rftest_LDADD = $(top_builddir)/libutils/libutils.la \
	$(top_builddir)/librf/librf.la -lmosquitto  -lmosquittopp -lpthread

AM_CFLAGS = -pthread
AM_CXXFLAGS = -pthread

