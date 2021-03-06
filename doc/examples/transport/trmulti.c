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

#include <transport/tr_net.h>
#include <transport/tr_tcp.h>
#include <stdio.h>
#include <unistd.h>

#define EXAMPLE_NAME "[tr_tcpservert]"

int main(int argc, char * const argv[]) {
	if(argc < 3) {
		printf("Usage: tr_tcpserver IP PORT\n");
		return 2;
	}

	printf("%s Starting up\n", EXAMPLE_NAME);
	tr_socket socket;
	tr_init_socket(&socket, 1024, 10);
	tr_tcpserver(&socket);

	tr_socket endpoint0;
	tr_socket endpoint1;
	
	printf("%s Opening socket\n", EXAMPLE_NAME);
	tr_open(&socket);

	printf("%s Binding socket\n", EXAMPLE_NAME);
	tr_bind(&socket, argv[1], argv[2]);
	
	printf("%s Local TCP socket: %s:%u [server]\n", EXAMPLE_NAME, 
			socket.local.address, socket.local.port);

	printf("%s Listening\n", EXAMPLE_NAME);
	tr_listen(&socket);
	

	printf("%s Entering accept loop\n", EXAMPLE_NAME);
		
	tr_init_socket(&endpoint0, 1024, 1);
	tr_tcpendpoint(&endpoint0);
	tr_accept(&socket, &endpoint0);
	printf("%s Accepted 0\n", EXAMPLE_NAME);

	tr_init_socket(&endpoint1, 1024, 1);
	tr_tcpendpoint(&endpoint1);
	tr_accept(&socket, &endpoint1);
	printf("%s Accepted 1\n", EXAMPLE_NAME);

	while(1) {
		tr_send(&endpoint0, "Welcome Mr. Client 0\0");
		tr_send(&endpoint1, "Welcome Mr. Client 1\0");
		tr_recv(&endpoint0);
		tr_recv(&endpoint1);
	}


	//while(1) {
	//	tr_init_socket_default(&endpoint);
	//	tr_tcpendpoint(&endpoint);
	//	printf("%s Waiting for endpoint\n", EXAMPLE_NAME);
	//	tr_accept(&socket, &endpoint);
	//
	//	printf("%s Remote TCP socket: %s:%u [endpoint]\n", EXAMPLE_NAME, 
	//			endpoint.remote.address, endpoint.remote.port);

	//	tr_send(&endpoint, "Welcome Mr. Client");
	//	while(tr_recv(&endpoint) != -1) {
	//		printf("%s Received: %s\n", EXAMPLE_NAME, endpoint.buffer);
	//		if(tr_send(&endpoint, "Thanks for your message") == -1)
	//			break;
	//	}
	//	printf("%s Endpoint dropped the connection\n", EXAMPLE_NAME);
	//	tr_close(&endpoint);
	//}

	printf("%s Closing socket\n", EXAMPLE_NAME);
	tr_close(&socket);
	
	return 0;
}
