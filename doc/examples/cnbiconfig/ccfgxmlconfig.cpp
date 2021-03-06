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

#include <cnbiconfig/CCfgXMLConfig.hpp>
#include <iostream>

using namespace std;

int main(void) {
	try {
		CCfgXMLConfig xml;
		xml.ImportFileEx("../extra/example.xml");
		xml.RootEx()->GoEx("offline")->GoEx("smr")->GoEx("description")->GetRawEx();
		xml.RootEx()->GoEx("online")->GoEx("smr")->GoEx("description")->GetRawEx();
		xml.Dump();
		
		xml.RootEx()->GoEx("offline")->GoEx("smr")->SetBranch();
		xml.BranchEx()->GoEx("description")->GetRawEx();
		xml.BranchEx()->GoEx("taskset")->GetRawEx();
		cout << "got: " << xml.BranchEx()->GoEx("taskset")->GetEx().Key() << endl;
		xml.Dump();

		xml.RootEx()->GoEx("tasks")->GoEx("mi_hand_right")->GoEx("hwtrigger")->SetBranch();
		string key = xml.BranchEx()->GetEx().Key();
		string id = xml.BranchEx()->GetEx().Id();
		int value = xml.BranchEx()->GetEx().Int();
		cout << "got: " << id << "/" << key << "=" << value << endl;
		
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
	}
	return 0;
}
