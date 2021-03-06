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

#include "tr_udp.h"
#include "tr_net.h"
#include <string.h>

void tr_udpserver(tr_socket* sock) {
	sock->protocol = TR_PROTO_UDP;
	sock->type = TR_TYPE_SERVER;
}

void tr_udpclient(tr_socket* sock) {
	sock->protocol = TR_PROTO_UDP;
	sock->type = TR_TYPE_CLIENT;
}

ssize_t tr_recvudp(tr_socket* sock) {
	return tr_recvudpb(sock, sock->buffer, sock->bsize);
}

ssize_t tr_recvudpb(tr_socket* sock, void* buffer, size_t bsize) {
	if(sock->protocol != TR_PROTO_UDP)
		return TR_PROTO_NOTSUPPORTED;

	int addr_len = sizeof(sock->address_endpoint);
	int status = recvfrom(sock->fd, buffer, bsize, 0,
			(struct sockaddr *)&sock->address_endpoint, (socklen_t*)&addr_len);
	
	tr_gethost_remote(sock, &(sock->remote));
	return status;
}

ssize_t tr_sendudp(tr_socket* sock, const void* buffer, size_t bsize) {
	if(sock->protocol != TR_PROTO_UDP)
		return TR_PROTO_NOTSUPPORTED;

	return sendto(sock->fd, buffer, bsize, 0, 
			sock->info->ai_addr, sock->info->ai_addrlen);
}
