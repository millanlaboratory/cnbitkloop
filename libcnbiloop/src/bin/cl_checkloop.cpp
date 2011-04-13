/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ClLoop.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <iostream>

using namespace std;

void usage(void) { 
	printf("Usage: cl_checkloop [-p]\n");
	printf("Where: -p      true/false\n");
	exit(1);
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optplot("false");
	
	while ((opt = getopt(argc, argv, "p:h")) != -1) {
		if(opt == 'p')
			optplot.assign(optarg);
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

	CcCore::OpenLogger("clallclients");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	ClLoop cl;
	if(cl.Connect() == false) {
		CcLogFatal("Cannot connect to loop");
		exit(1);
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
	if(cl.processing.ForkAndCheck(&pid0) != ClProLang::Successful)
		exit(2);
	if(cl.processing.ForkAndCheck(&pid1) != ClProLang::Successful)
		exit(2);

	cl.nameserver.Erase("ndf_monitor::scope");
	cl.nameserver.Store("ndf_monitor::scope", optplot);

	cl.nameserver.Set("/feedback0", "127.0.0.1:9500");
	cl.processing.ChangeDirectory(pid0, "/tmp/");
	cl.processing.IncludeNDF(pid0) ;
	cl.processing.LaunchNDF(pid0, "ndf_monitor", "/pipe0", 
			"/acquisition", "/feedback0", "");

	cl.nameserver.Set("/feedback1", "127.0.0.1:9501");
	cl.processing.ChangeDirectory(pid1, "/tmp/");
	cl.processing.IncludeNDF(pid1) ;
	cl.processing.LaunchNDF(pid1, "ndf_monitor", "/pipe1", 
			"/acquisition", "/feedback1", "");
	
	if(cl.processing.Check(pid0) == false) {
		CcLogFatal("PID0 is dead");
		goto shutdown;
	}
	if(cl.processing.Check(pid1) == false) {
		CcLogFatal("PID1 is dead");
		goto shutdown;
	}

	if(cl.acquisition.OpenXDF(filebdf, filelog, "debug=1") 
			!= ClAcqLang::Successful) {
		CcLogFatal("Failed to open XDF, going down");
		goto shutdown;
	}

	CcTimeValue tic;
	CcTime::Tic(&tic);
	while(true) {
		if(CcCore::receivedSIGINT.Get() || CcCore::receivedSIGTERM.Get())
			goto shutdown;
		if(cl.IsConnected() == false) {
			CcLogFatal("Lost connection with loop");
			goto shutdown;
		}
		if(cl.processing.Check(pid0) == false) {
			CcLogFatal("PID0 died");
			goto shutdown;
		}
		if(cl.processing.Check(pid1) == false) {
			CcLogFatal("PID1 died");
			goto shutdown;
		}

		if(CcTime::Toc(&tic) > 20000.00f) {
			cl.acquisition.AddLabelGDF(781);
			cl.acquisition.AddLabelLPT(2);
			CcTime::Tic(&tic);
		}
		
		CcTime::Sleep(2000.00f);
	}

shutdown:
	cl.acquisition.CloseXDF();
	cl.nameserver.Erase("ndf_monitor::scope");
	cl.processing.Terminate(pid0);
	cl.processing.Terminate(pid1);

	return 0;
}