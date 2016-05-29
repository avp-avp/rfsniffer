AM_CPPFLAGS = -I../include/
bin_PROGRAMS = rfsniffer
rfsniffer_SOURCES = rfsniffer.cpp MqttConnect.cpp
rfsniffer_LDADD = $(top_builddir)/libutils/libutils.la \
	$(top_builddir)/librf/librf.la -lmosquitto  -lmosquittopp -lpthread
