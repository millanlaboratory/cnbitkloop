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

#include "ClProClient.hpp"
#include "ClAcqClient.hpp"
#include "ClNamesClient.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <iostream>

using namespace std;

int main(void) {
	CcCore::OpenLogger("clallclients");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	ClProClient processing;
	ClAcqClient acquisiton;
	ClNamesClient nameserver;

	if(nameserver.Connect() == false) {
		CcLogFatal("Cannot connect to nameserver");
		exit(1);
	}
	if(processing.Connect(nameserver.Query("/processing")) == false) {
		CcLogFatal("Cannot connect to processing");
		exit(1);
	}
	if(acquisiton.Connect(nameserver.Query("/acquisition")) == false) {
		CcLogFatal("Cannot connect to acquisiton");
		exit(1);
	}

	int pid0, pid1;
	if(processing.ForkAndCheck(&pid0) != ClProLang::Successful)
		exit(2);
	if(processing.ForkAndCheck(&pid1) != ClProLang::Successful)
		exit(2);
	
	processing.ChangeDirectory(pid0, "/tmp/");
	processing.IncludeNDF(pid0) ;
	processing.LaunchNDF(pid0, "ndf_monitor", "/tmp/cl.pipe.ndf.0", 
			nameserver.Query("/acquisition"), "127.0.0.1:9500", "");

	processing.ChangeDirectory(pid1, "/tmp/");
	processing.IncludeNDF(pid1) ;
	processing.LaunchNDF(pid1, "ndf_monitor", "/tmp/cl.pipe.ndf.1", 
			nameserver.Query("/acquisition"), "127.0.0.1:9501", "");

	if(processing.Check(pid0) == false) {
		CcLogFatal("PID0 is dead");
		//goto shutdown;
	}
	if(processing.Check(pid1) == false) {
		CcLogFatal("PID1 is dead");
		//goto shutdown;
	}

	std::string timestamp, filebdf, filelog;
	CcTime::Timestamp(&timestamp);
	filebdf.assign("cltest");
	filebdf.append(timestamp);
	filebdf.append(".bdf");

	filelog.assign("cltest");
	filelog.append(timestamp);
	filelog.append(".bdf");

	if(acquisiton.OpenXDF(filebdf, filelog, "debug=1") 
			!= ClAcqLang::Successful) {
		CcLogFatal("Failed to open XDF, going down");
		goto shutdown;
	}

	while(true) {
		if(CcCore::receivedSIGINT.Get() || CcCore::receivedSIGTERM.Get())
			goto shutdown;
		if(processing.Connect() == false)
			goto shutdown;
		if(acquisiton.Connect() == false)
			goto shutdown;
		if(nameserver.Connect() == false)
			goto shutdown;
	
		acquisiton.AddLabelGDF(781);
		acquisiton.AddLabelLPT(2);
		
		CcTime::Sleep(5000.00f);
	}

shutdown:
	acquisiton.CloseXDF();
	processing.Terminate(pid0);
	processing.Terminate(pid1);

	return 0;
}