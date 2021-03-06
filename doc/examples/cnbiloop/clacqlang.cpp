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
#include <cnbiloop/ClAcqLang.hpp>
#include <iostream>

using namespace std;

int main(void) {
	string label;
	string filegdf, linelog, filelog;
	int id = 0;

	ClAcqLang acq;
	cout << acq.OpenXDF("test.gdf", "mylog", "classifier=test.mat") << endl;
	cout << acq.IsOpenXDF(acq.message->buffer, &filegdf, &filelog, &linelog) << endl;
	cout << "--> Message:  " << acq.message->buffer << endl;
	cout << "--> File XDF: " << filegdf << endl;
	cout << "--> Line LOG: " << linelog << endl;
	
	cout << acq.OpenXDF("test.gdf", "mylog", "classifier=test.mat rejection=0.60") 
		<< endl;
	cout << acq.IsOpenXDF(acq.message->buffer, &filegdf, &filelog, &linelog) << endl;
	cout << "--> Message:  " << acq.message->buffer << endl;
	cout << "--> File XDF: " << filegdf << endl;
	cout << "--> Line LOG: " << linelog << endl;
	
	cout << acq.UpdateLog("classifier=test.mat rejection=0.60") << endl;
	cout << acq.IsUpdateLog(acq.message->buffer, &linelog) << endl;
	cout << "--> Line LOG: " << linelog << endl;
	
	cout << acq.CloseXDF() << endl;
	cout << acq.IsCloseXDF(acq.message->buffer) << endl;

	cout << acq.Error(acq.NDFLimitReached) << endl;
	cout << acq.IsError(acq.message->buffer, &id) << endl;
	
	cout << acq.Ok() << endl;
	cout << acq.IsOk(acq.message->buffer) << endl;

	return 0;
}
