/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ClLoop.hpp"
#include <cnbicore/CcBasic.hpp>
#include <iostream>

using namespace std;

void usage(void) { 
	printf("Usage: cl_checkloop [OPTION]\n\n");
	printf("  -p       true/false, enable ndf_monitor scope\n");
	printf("  -h       display this help and exit\n");
	CcCore::Exit(0);
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optplot("false");
	
	while ((opt = getopt(argc, argv, "p:h")) != -1) {
		if(opt == 'p')
			optplot.assign(optarg);
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	CcCore::OpenLogger("cl_checkloop");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	ClLoop::Instance();
	if(ClLoop::Connect() == false) {
		CcLogFatal("Cannot connect to loop");
		CcCore::Exit(1);
	}
	
	std::string timestamp, filebdf, filelog;
	CcTime::Timestamp(&timestamp);
	filebdf.assign("cltest_");
	filebdf.append(timestamp);
	filebdf.append(".bdf");

	filelog.assign("cltest_");
	filelog.append(timestamp);
	filelog.append(".log");

	int pid0, pid1;
	if(ClLoop::processing.ForkAndCheck(&pid0) != ClProLang::Successful) {
		CcLogFatal("Cannot spawn PID0");
		CcCore::Exit(2);
	}
	if(ClLoop::processing.ForkAndCheck(&pid1) != ClProLang::Successful) {
		CcLogFatal("Cannot spawn PID1");
		CcCore::Exit(2);
	}

	ClLoop::nameserver.Erase("ndf_monitor::plot");
	ClLoop::nameserver.Store("ndf_monitor::plot", optplot);

	//ClLoop::nameserver.Set("/feedback0", "127.0.0.1:9500");
	ClLoop::processing.ChangeDirectory(pid0, "/tmp/");
	ClLoop::processing.IncludeNDF(pid0) ;
	ClLoop::processing.LaunchNDF(pid0, "ndf_monitor", "/pipe0", 
			"/acquisition", "/feedback0");

	ClLoop::nameserver.Set("/feedback1", "127.0.0.1:9501");
	ClLoop::processing.ChangeDirectory(pid1, "/tmp/");
	ClLoop::processing.IncludeNDF(pid1) ;
	ClLoop::processing.LaunchNDF(pid1, "ndf_monitor", "/pipe1", 
			"/acquisition", "/feedback1");
	
	if(ClLoop::processing.Check(pid0) == false) {
		CcLogFatal("PID0 is dead");
		goto shutdown;
	}
	if(ClLoop::processing.Check(pid1) == false) {
		CcLogFatal("PID1 is dead");
		goto shutdown;
	}

	if(ClLoop::acquisition.OpenXDF(filebdf, filelog, "debug=1") 
			!= ClAcqLang::Successful) {
		CcLogFatal("Failed to open XDF, going down");
		goto shutdown;
	}

	while(true) {
		if(CcCore::receivedSIGAny.Get())
			goto shutdown;
		if(ClLoop::IsConnected() == false) {
			CcLogFatal("Lost connection with loop");
			goto shutdown;
		}
		if(ClLoop::processing.Check(pid0) == false) {
			CcLogFatal("PID0 died");
			goto shutdown;
		}
		if(ClLoop::processing.Check(pid1) == false) {
			CcLogFatal("PID1 died");
			goto shutdown;
		}

		CcTime::Sleep(1000.00f);
	}

shutdown:
	ClLoop::acquisition.CloseXDF();
	ClLoop::nameserver.Erase("ndf_monitor::plot");
	ClLoop::processing.Terminate(pid0);
	ClLoop::processing.Terminate(pid1);

	CcCore::Exit(0);
}