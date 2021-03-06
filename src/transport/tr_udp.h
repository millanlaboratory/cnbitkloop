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

#ifndef UDP_H
#define UDP_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "tr_types.h"

/*! \file tr_udp.h 
 *	\brief Low-level UDP functionalities
 */ 

//! Initialize the tr_socket as a UDP server socket
void tr_udpserver(tr_socket* sock);

//! Initialize the tr_socket as a UDP client socket
void tr_udpclient(tr_socket* sock);

//! Receive as UDP on the internal buffer
ssize_t tr_recvudp(tr_socket* sock);

//! Receive as UDP
ssize_t tr_recvudpb(tr_socket* sock, void* buffer, size_t bsize);

//! Send as UDP
ssize_t tr_sendudp(tr_socket* sock, const void* buffer, size_t bsize);

#ifdef __cplusplus
}
#endif

#endif
