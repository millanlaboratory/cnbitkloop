AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src -std=c++11
lib_LTLIBRARIES = libcnbiloop.la

bin_PROGRAMS = cl_processing \
			   cl_ndfmonitor \
			   cl_nameserver \
			   cl_tidmonitor \
			   cl_tidsender \
			   cl_ticmonitor \
			   cl_ticsender \
			   cl_checkloop \
			   cl_rpc \
			   cl_init \
			   cl_acquisition \
			   cl_acquisitionscope

dist_bin_SCRIPTS = cl_rpcnotify \
				   cl_grpc \
				   cl_screenloop \
				   cl_attachloop \
				   cl_keepalive \
				   cl_keeprunning \
				   cl_termloop \
				   cl_killloop \
				   cl_runloop \
				   cl_runloopscope \
				   cl_debugloop \
				   cl_develloop \
				   cl_killmatlab \
				   cl_copyxsl

libcnbiloop_includedir=$(includedir)/cnbiloop

libcnbiloop_include_HEADERS = ClMatlab.hpp \
							  ClLanguage.hpp \
							  ClAcqLang.hpp \
							  ClProLang.hpp \
							  ClNmsLang.hpp \
							  ClAcqClient.hpp \
							  ClNmsClient.hpp \
							  ClProClient.hpp \
							  ClClient.hpp \
							  ClLoop.hpp \
							  ClLoopConfig.hpp \
							  ClTobiIc.hpp \
							  ClTobiId.hpp

libcnbiloop_la_SOURCES = ClMatlab.cpp \
						 ClMatlab.hpp \
						 ClLanguage.cpp \
						 ClLanguage.hpp \
						 ClAcqLang.cpp \
						 ClAcqLang.hpp \
						 ClProLang.cpp \
						 ClProLang.hpp \
						 ClNmsLang.cpp \
						 ClNmsLang.hpp \
						 ClNmsClient.hpp \
						 ClNmsClient.cpp \
						 ClAcqClient.hpp \
						 ClAcqClient.cpp \
						 ClProClient.hpp \
						 ClProClient.cpp \
						 ClClient.hpp \
						 ClClient.cpp \
						 ClLoop.hpp \
						 ClLoop.cpp \
						 ClLoopConfig.hpp \
						 ClLoopConfig.cpp \
						 ClLoopConfigSrv.hpp \
						 ClLoopConfigSrv.cpp \
						 ClTobiIc.cpp \
						 ClTobiIc.hpp \
						 ClTobiId.cpp \
						 ClTobiId.hpp

libcnbiloop_la_LDFLAGS = $(AM_LDLAGS) -ltobicore \
						 -version-info $(CURRENT):$(REVISION):$(AGE) 

libcnbiloop_la_LIBADD = $(builddir)/../cnbicore/libcnbicore.la \
						$(builddir)/../ndf/libndf.la \
						$(builddir)/../cnbiconfig/libcnbiconfig.la \
						$(TOBIIC_LIBS) \
						$(TOBIID_LIBS)

LDADD = libcnbiloop.la \
		$(top_builddir)/src/cnbicore/libcnbicore.la \
		$(builddir)/../cnbiconfig/libcnbiconfig.la \
		$(builddir)/../ndf/libndf.la \
		$(TOBICORE_LIBS) \
		$(TOBIIC_LIBS) \
		$(TOBIID_LIBS)

cl_acquisition_SOURCES = cl_acquisition.cpp \
						 ClAcqServer.cpp \
						 ClAcqServer.hpp \
						 ClBusServer.cpp \
						 ClBusServer.hpp \
						 ClDevServer.cpp \
						 ClDevServer.hpp

cl_acquisition_LDADD = libcnbiloop.la \
					   $(top_builddir)/src/cnbiacq/libcnbiacq.la \
					   $(LDADD)

cl_acquisitionscope_SOURCES = cl_acquisitionscope.cpp \
						 ClAcqServer.cpp \
						 ClAcqServer.hpp \
						 ClBusServer.cpp \
						 ClBusServer.hpp \
						 ClDevServer.cpp \
						 ClDevServer.hpp \
						 mcpanel.h

cl_acquisitionscope_LDADD = libcnbiloop.la \
				-lmcpanel \
			   $(top_builddir)/src/cnbiacq/libcnbiacq.la \
			   $(LDADD)

cl_processing_SOURCES = cl_processing.cpp \
						ClProServer.cpp \
						ClProServer.hpp
cl_nameserver_SOURCES = cl_nameserver.cpp \
						ClNmsServer.cpp \
						ClNmsServer.hpp
cl_ndfmonitor_SOURCES = cl_ndfmonitor.cpp
cl_tidmonitor_SOURCES = cl_tidmonitor.cpp
cl_tidsender_SOURCES = cl_tidsender.cpp
cl_ticmonitor_SOURCES = cl_ticmonitor.cpp
cl_ticsender_SOURCES = cl_ticsender.cpp
cl_checkloop_SOURCES = cl_checkloop.cpp
cl_rpc_SOURCES = cl_rpc.cpp
cl_init_SOURCES = cl_init.cpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbiloop.pc
