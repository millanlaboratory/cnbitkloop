AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src
lib_LTLIBRARIES = libcnbicore.la

libcnbicore_includedir = $(includedir)/cnbicore

libcnbicore_include_HEADERS = CcBasic.hpp \
							  CcCallback.hpp \
							  CcClient.hpp \
							  CcClientExt.hpp \
							  CcCore.hpp \
							  CcDeserializer.hpp \
							  CcEndpoint.hpp \
							  CcException.hpp \
							  CcFlags.hpp \
							  CcLog.hpp \
							  CcLogEntry.hpp \
							  CcLogger.hpp \
							  CcMutex.hpp \
							  CcNetworkTypes.hpp \
							  CcObject.hpp \
							  CcPipe.hpp \
							  CcPipeWriter.hpp \
							  CcPipeReader.hpp \
							  CcProcess.hpp \
							  CcProxy.hpp \
							  CcPtable.hpp \
							  CcSemaphore.hpp \
							  CcServer.hpp\
							  CcSocket.hpp \
							  CcSocketProxy.hpp \
							  CcStreamer.hpp \
							  CcThread.hpp \
							  CcThreadSafe.hpp \
							  CcThreadSafe.cpp \
							  CcThreadrunner.hpp \
							  CcTime.hpp \
							  CcDoubleBuffer.hpp \
							  CcBuffer.cpp \
							  CcBuffer.hpp \
							  CcFile.cpp \
							  CcFile.hpp \
							  CcPipeServer.hpp \
							  CcUDPListener.hpp


libcnbicore_la_SOURCES = CcBasic.hpp \
						 CcCallback.hpp \
						 CcClient.cpp \
						 CcClient.hpp \
						 CcClientExt.cpp \
						 CcClientExt.hpp \
						 CcCore.cpp \
						 CcCore.hpp \
						 CcDeserializer.hpp \
						 CcEndpoint.cpp \
						 CcEndpoint.hpp \
						 CcException.cpp \
						 CcException.hpp \
						 CcFlags.hpp \
						 CcLog.hpp \
						 CcLogEntry.cpp \
						 CcLogEntry.hpp \
						 CcLogger.cpp \
						 CcLogger.hpp \
						 CcMutex.cpp \
						 CcMutex.hpp \
						 CcNetworkTypes.hpp \
						 CcObject.cpp \
						 CcObject.hpp \
						 CcPipe.cpp \
						 CcPipe.hpp \
						 CcPipeReader.cpp \
						 CcPipeReader.hpp \
						 CcPipeWriter.cpp \
						 CcPipeWriter.hpp \
						 CcProcess.cpp \
						 CcProcess.hpp \
						 CcProxy.hpp \
						 CcPtable.cpp \
						 CcPtable.hpp \
						 CcSemaphore.cpp \
						 CcSemaphore.hpp \
						 CcServer.cpp \
						 CcServer.hpp \
						 CcSocket.cpp \
						 CcSocket.hpp \
						 CcSocketProxy.cpp \
						 CcSocketProxy.hpp \
						 CcStreamer.cpp \
						 CcStreamer.hpp \
						 CcThread.cpp \
						 CcThread.hpp \
						 CcThreadSafe.cpp \
						 CcThreadSafe.hpp \
						 CcThreadrunner.cpp \
						 CcThreadrunner.hpp \
						 CcTime.cpp \
						 CcTime.hpp \
						 CcDoubleBuffer.cpp \
						 CcDoubleBuffer.hpp \
						 CcBuffer.cpp \
						 CcBuffer.hpp \
						 CcFile.cpp \
						 CcFile.hpp \
						 CcPipeServer.cpp \
						 CcPipeServer.hpp \
						 CcUDPListener.hpp \
						 CcUDPListener.cpp 

libcnbicore_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE)

libcnbicore_la_LIBADD = $(top_builddir)/src/transport/libtransport.la \
						$(top_builddir)/src/transpipe/libtranspipe.la \
						$(PTHREAD_LIBS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbicore.pc
