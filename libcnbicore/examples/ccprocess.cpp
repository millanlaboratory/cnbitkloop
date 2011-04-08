/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CcBasic.hpp"
#include "CcProcess.hpp"
#include <stdio.h>

using namespace std;

class Matlab : public CcProcess {
	public:
		Matlab(const std::string& cmd) : CcProcess(cmd, true, true) {
		}

		virtual void Exec(void) { 
			execlp(this->_cmd.c_str(),
					"-nodesktop", "-nojvm", "-nosplash", NULL);
					//"1>/dev/null", "2>/dev/null", NULL);
		}
};

int main(void) {
	CcCore::OpenLogger("ccprocess", 
			CcCore::TerminalColors, CcLogEntry::LevelConfig);
	
	CcPtable* table = CcPtable::Instance();

	Matlab p0("matlab");
	Matlab p1("matlab");
	Matlab p2("matlab");
	
	p0.Fork();
	p1.Fork();
	p2.Fork();

	CcTime::Sleep(2000);
	CcPtable::Dump();

	char buffer[2048];
	p0.Write("plot(sin(2*pi*[0:0.01:1]*10), 'b');\n");
	p0.Write("axis tight;\n");
	p0.Write("grid on;\n");
	p0.Write("title('Sinusoid for p0');\n");
	
	std::cout << "p1 rsize: " << p1.Read(buffer, 2048) << std::endl;
	std::cout << buffer << std::endl;
	p1.Write("disp('hello workd');\n");
	std::cout << "p1 rsize: " << p1.Read(buffer, 2048) << std::endl;
	std::cout << buffer << std::endl;
	p1.Write("plot(sin(2*pi*[0:0.01:1]*4), 'r');\n");
	p1.Write("axis tight;\n");
	p1.Write("grid on;\n");
	p1.Write("title('Sinusoid for p1');\n");
	
	p2.Write("while(pause(1));\n");
	
	cout << "Letter and enter to terminate" << endl;
	char a;
	cin >> a;

	p0.Terminate();
	p1.Terminate();
	p2.Terminate();
	
	CcPtable::Dump();

	cout << "Letter and enter to quit" << endl;
	cin >> a;
	
	CcPtable::Release();
	return 0;
}