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

#include "ClNamesClient.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <iostream>

using namespace std;

int main(void) {
	CcCore::OpenLogger("clclientacq");

	ClNamesClient client;
	if(client.Connect("127.0.0.1:8000") == false) {
		CcLogFatal("Cannot connect to endpoint");
		exit(1);
	}
	

	CcAddress address;

	while(client.Connect()) {
		client.Set("/pippo", "127.0.0.1:12000");
		client.Set("/pippo", "127.0.0.1:12000");
		client.Query("/pippo", &address);
		client.Query("/pluto", &address);
		client.Unset("/pippo");
		client.Unset("/pippo");
		CcTime::Sleep(10000.00f);
	}



	while(client.Connect()) {
		client.Set("/pippo", "127.0.0.1:12000");
		client.Set("/pluto", "127.0.0.1:12001");
		client.Set("/minie", "127.0.0.1:12002");
		
		client.Unset("/pippo");
		client.Unset("/pluto");
		client.Unset("/minie");
	}

	/*
	while(client.Connect()) {
		int pid0 = 0, pid1 = 0;
		
		if(client.ForkAndCheck(&pid0) != ClProLang::Successful)
			return 1;
		if(client.ForkAndCheck(&pid1) != ClProLang::Successful)
			return 1;
		cout << "Spawned " << pid0 << endl;
		cout << "Spawned " << pid1 << endl;
		
		client.ChangeDirectory(pid0, "/home/mtavella/");
		client.Launch(pid0, "a=rand(10,10);");
		client.Launch(pid0, "save;");
		client.Launch(pid0, "bench(1);");
		
		client.Launch(pid1, "antani;");
	
		for(int i = 0; i < 4; i++) {	
			if(client.Connect() == false)
				return 2;

			int status0 = client.IsAlive(pid0);
			int status1 = client.IsAlive(pid1);

			cout << "PID0 " << status0 << endl;
			cout << "PID1 " << status1 << endl;
	
			if(status0 != ClProLang::Successful)
				client.Terminate(pid0);
			if(status1 != ClProLang::Successful)
				client.Terminate(pid1);

			CcTime::Sleep(1000.00f);
		}
		
		client.Terminate(pid0);
		client.Terminate(pid1);

		CcTime::Sleep(5000.00f);
	}
	*/
	return 0;
}