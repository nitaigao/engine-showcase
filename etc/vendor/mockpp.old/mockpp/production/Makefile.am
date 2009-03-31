INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

lib_LTLIBRARIES = libmockpp_production.la

libmockpp_production_la_LDFLAGS = -no-undefined -version-info \
	$(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libmockpp_productionincludedir = $(includedir)/mockpp/production

libmockpp_production_la_SOURCES = TimeServer.cpp

libmockpp_productioninclude_HEADERS = mockpp_production.h TimeServer.h
