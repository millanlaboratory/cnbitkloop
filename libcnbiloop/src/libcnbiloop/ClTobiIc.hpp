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

#ifndef CLTOBIIC_HPP 
#define CLTOBIIC_HPP 

#include "ClLoop.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcServerSingle.hpp>
#include <libcnbicore/CcThreadSafe.hpp>
#include <libtobicore/TCException.hpp>
#include <libtobiic/ICMessage.hpp>
#include <libtobiic/ICSerializerRapid.hpp>
#include <iostream>

class ClTobiIc : public CcSocketProxy {
	public:
		ClTobiIc(void);
		virtual ~ClTobiIc(void);
		virtual bool Open(const CcPort port, const std::string& name);
		virtual bool Close(void);
		virtual bool GetMessage(ICSerializerRapid* serializer, bool lock = true);
	protected:
		virtual void HandleAccept(CcSocket* caller);
		virtual void HandleDrop(CcSocket* caller);
		virtual void HandleRecv(CcSocket* caller);

	public:

	protected:
		CcServerSingle* _server;
		std::string _name;
		std::stringstream _stream;
		CcSemaphore _hasmessage;

		std::string _buffer;
		CcSemaphore _sembuffer;
		CcThreadSafe<bool> _hasdropped;
};

#endif
