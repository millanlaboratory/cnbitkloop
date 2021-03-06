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

#include <cnbicore/CcCore.hpp>
#include <cnbicore/CcObject.hpp>
#include <cnbicore/CcServer.hpp>
#include <cnbicore/CcSocketProxy.hpp>
#include <cnbicore/CcTime.hpp>
#include <stdio.h>

class EventHandler : public CcSocketProxy {
	public:
		void HandleRecv(CcSocket* caller, CcStreamer* stream) { 
			CcServer* server = (CcServer*)caller;
			printf("[EventHandler] Server received a message\n");
			server->Send((const char*)"<BounceAll>");
			stream->Clear();
			printf("[EventHandler] Streamer now empty: '%d'\n", 
					stream->Size());
		}
		
		void HandleRecvEndpoint(CcSocket* caller, CcAddress addr, CcStreamer* stream) { 
			CcServer* server = (CcServer*)caller;
			printf("[EventHandler] Server received a message from %s\n",
					addr.c_str());
			server->SendNot("<BounceOthers>", addr);
		}
};

void RegisterAll(CcServer* server, EventHandler *handler) {
	CB_CcSocket(server->iOnRelease,    handler, HandleRelease);
	CB_CcSocket(server->iOnAccept,     handler, HandleAccept);
	CB_CcSocket(server->iOnAcceptPeer, handler, HandleAcceptPeer);
	CB_CcSocket(server->iOnDrop,       handler, HandleDrop);
	CB_CcSocket(server->iOnDropPeer,   handler, HandleDropPeer);
	CB_CcSocket(server->iOnRecv,       handler, HandleRecv);
	CB_CcSocket(server->iOnRecvPeer,   handler, HandleRecvPeer);
}

int main(void) {
	EventHandler *handler = new EventHandler();

	CcServer *server;
	
	printf("[Main] Starting server\n");
	server = new CcServer();
	RegisterAll(server, handler);
	server->Bind("0.0.0.0:8000", 2);

	printf("[Main] Up and running\n");
	CcTimeValue clock;
	CcTime::Tic(&clock);
	while(CcTime::Toc(&clock) < 60000) {
		server->Send("<Multicast>");
		CcTime::Sleep(2000);
	}

	printf("[Main] Stopping ASync server\n");
	server->Release();
	
	delete server;
	
	return 0;
}
