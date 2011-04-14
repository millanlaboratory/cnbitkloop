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

#ifndef CLTOBIIC_CPP 
#define CLTOBIIC_CPP 

#include "ClTobiIc.hpp" 

ClTobiIc::ClTobiIc(void) {
	this->_server = NULL;
}

ClTobiIc::~ClTobiIc(void) {
}
		
bool ClTobiIc::Open(const CcPort port, const std::string& name) {
	if(this->_server != NULL)
		return false;
	
	this->_name.assign(name);

	ClLoop::Instance();
	if(ClLoop::Connect() == false) { 
		CcLogError("Cannot connect to loop");
		return false;
	}
	CcEndpoint endpoint("0.0.0.0", port);
	
	this->_server = new CcServerSingle();
	try { 
		this->_server->Bind(endpoint);
	} catch(CcException e) {
		CcLogDebugS(this->_stream, "Cannot bind to port " << port);
		return false;
	}

	int status = ClLoop::nameserver.Set(this->_name, endpoint.GetAddress());
	if(status != ClNamesLang::Successful) {
		CcLogDebugS(this->_stream, "Cannot set " << name 
				<< " as " << endpoint.GetAddress());
		return false;
	}

	CB_CcSocket(this->_server->iOnAccept, this, HandleAccept);
	CB_CcSocket(this->_server->iOnDrop, this, HandleDrop);
	CB_CcSocket(this->_server->iOnRecv, this, HandleRecv);
	
	this->_hasdropped.Set(false);
	this->_hasmessage.Wait();

	return true;
}
		
bool ClTobiIc::Close(void) {
	if(this->_server == NULL)
		return true;

	ClLoop::nameserver.Unset(this->_name);
	this->_server->Release();
	delete(this->_server);
	this->_server = NULL;
	this->_hasmessage.Post();

	return true;
}

bool ClTobiIc::WaitMessage(ICSerializerRapid* serializer) {
	return GetMessage(serializer, false);
}

bool ClTobiIc::GetMessage(ICSerializerRapid* serializer) {
	return GetMessage(serializer, true);
}

bool ClTobiIc::GetMessage(ICSerializerRapid* serializer, bool lock) {
	if(this->_hasdropped.Get() == true)
		return false;

	if(lock == false) {
		if(this->_hasmessage.TryWait() == false)
			return false;
	} else {
		this->_hasmessage.Wait();
		if(this->_hasdropped.Get() == true)
			return false;
	}
	
	this->_sembuffer.Wait();
	try {
		serializer->Deserialize(&this->_buffer);
		this->_buffer.clear();
	} catch(TCException e) { 
		CcLogException("Caught TCException");
	}
	this->_sembuffer.Post();
	return true;
}

void ClTobiIc::HandleRecv(CcSocket* caller) { 
	if(this->_sembuffer.TryWait() == false) 
		return;

	bool status = false;
	status = this->_server->datastream.Extract(&this->_buffer, "<tobiic",
			"</tobiic>", CcStreamer::Forward);
	if(status)
		this->_hasmessage.Post();
	this->_sembuffer.Post();
}

void ClTobiIc::HandleAccept(CcSocket* caller) { 
	CcServerSingle *server = (CcServerSingle*)caller;
	CcLogDebugS(this->_stream, "Accepted TCP endpoint: " << 
			server->GetRemote());
}
		
void ClTobiIc::HandleDrop(CcSocket* caller) { 
	CcServerSingle *server = (CcServerSingle*)caller;
	CcLogDebugS(this->_stream, "Dropped TCP endpoint: " << 
			server->GetRemote());
	this->_hasdropped.Set(true);
	this->_hasmessage.Post();
}

#endif
