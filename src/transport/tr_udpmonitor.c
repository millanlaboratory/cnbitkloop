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

#include "tr_net.h"
#include "tr_udp.h"
#include <stdio.h>
#include <unistd.h>

#define EXAMPLE_NAME "[tr_udpmonitor]"

int main(void) { 
	printf("%s Starting up\n", EXAMPLE_NAME);
	tr_socket socket;
	tr_init_socket(&socket, 1024, 1);
	tr_udpserver(&socket);
	
	printf("%s Opening socket\n", EXAMPLE_NAME);
	tr_open(&socket);
	
	printf("%s Binding socket\n", EXAMPLE_NAME);
	tr_bind(&socket, "0.0.0.0", "8000");

	printf("%s Local UDP socket: %s:%u [server]\n", EXAMPLE_NAME, 
			socket.local.address, socket.local.port);
	
	printf("%s Entering recv loop\n", EXAMPLE_NAME);
	while(1) {
		if(tr_recv(&socket) == -1) {
			printf("%s Local socket error\n", EXAMPLE_NAME);
			break;
		}
		printf("%s >> %s\n", EXAMPLE_NAME, (char*)socket.buffer);
		printf("%s Remote TCP socket: %s:%u [endpoint]\n", EXAMPLE_NAME, 
				socket.remote.address, socket.remote.port);
	}

	tr_close(&socket);

	return 0;
}

