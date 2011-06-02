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

#ifndef CLTOBIIC_CPP 
#define CLTOBIIC_CPP 

#include "ClTobiIc.hpp" 

ClTobiIc::ClTobiIc(int mode) {
	ClLoop::Instance();
	this->_mode = mode;
	this->_server = NULL;
	this->_client = NULL;
	this->_onwsname = false;
}

ClTobiIc::~ClTobiIc(void) {
	ClLoop::Release();
}

bool ClTobiIc::Attach(const std::string& name) {
	if(this->_server != NULL || this->_client != NULL)
		return false;

	if(ClLoop::Connect() == false) { 
		CcLogErrorS("Cannot connect to loop");
		return false;
	}
	this->_name.assign(name);
	this->_onwsname = false;
	
	CcAddress address;
	int status = ClLoop::nameserver.Query(this->_name, &address);
	if(status != ClNamesLang::Successful) {
		CcLogErrorS("Cannot query " << name);
		return false;
	}
	
	CcEndpoint peer(address);
	if(this->_mode == ClTobiIc::GetOnly) {
		CcLogConfigS("Configuring iC for GetOnly (query name): " << address);
		this->_server = new CcServer();
		CB_CcSocket(this->_server->iOnAccept, this, HandleAccept);
		CB_CcSocket(this->_server->iOnDrop, this, HandleDrop);
		CB_CcSocket(this->_server->iOnRecvPeer, this, HandleRecvPeer);

		if(this->_server->Bind(peer.GetPort()) == false) {
			CcLogErrorS("Cannot bind to port " << peer.GetPort());
			return false;
		}
		this->_hasmessage.Wait();
	} else { 
		CcLogConfigS("Configuring iC for SetOnly: " << address);
		this->_client = new CcClient();
		CB_CcSocket(this->_client->iOnConnect, this, HandleAccept);
		CB_CcSocket(this->_client->iOnDisconnect, this, HandleDrop);
		
		if(this->_client->Connect(peer.GetAddress()) == false) {
			CcLogErrorS("Cannot connect to address" << peer.GetAddress());
			return false;
		}
	}

	this->iOnAttach.Execute();
	return true;
}
		
bool ClTobiIc::Attach(const CcPort port, const std::string& name) {
	if(this->_mode != ClTobiIc::GetOnly) {
		CcLogError("Cannot attach as GetOnly when configured as SetOnly");
		return false;
	}
	if(this->_server != NULL)
		return false;
	
	if(ClLoop::Connect() == false) { 
		CcLogErrorS("Cannot connect to loop");
		return false;
	}
	this->_name.assign(name);
	
	CcEndpoint peer("0.0.0.0", port);
	CcLogConfigS("Configuring iC for GetOnly (set name): " << peer.GetAddress());
	this->_server = new CcServer();
	CB_CcSocket(this->_server->iOnAccept, this, HandleAccept);
	CB_CcSocket(this->_server->iOnDrop, this, HandleDrop);
	CB_CcSocket(this->_server->iOnRecvPeer, this, HandleRecvPeer);

	if(this->_server->Bind(peer.GetPort()) == false) {
		CcLogErrorS("Cannot bind to port " << port);
		return false;
	}

	int status = ClLoop::nameserver.Set(this->_name, peer.GetAddress());
	if(status != ClNamesLang::Successful) {
		CcLogErrorS("Cannot set " << name << " as " << peer.GetAddress());
		this->_onwsname = false;
		return false;
	} 
	this->_onwsname = true;

	this->_hasmessage.Wait();
	this->iOnAttach.Execute();
	return true;
}

bool ClTobiIc::Detach(void) {
	if(this->_server == NULL && this->_client == NULL)
		return true;


	if(this->_onwsname == true)
		ClLoop::nameserver.Unset(this->_name);
	
	if(this->_server != NULL) {
		this->_server->Release();
		delete(this->_server);
		this->_server = NULL;
		this->_hasmessage.Post();
	}
	
	if(this->_client != NULL) {
		this->_client->Disconnect();
		delete(this->_client);
		this->_client = NULL;
	}
	
	this->iOnDetach.Execute();
	return true;
}

bool ClTobiIc::IsAttached(void) {
	if(this->_mode == ClTobiIc::GetOnly) {
		if(this->_server == NULL)
			return false;
		else
			return this->_server->IsConnected();
	} else {
		if(this->_client == NULL)
			return false;
		else
			return this->_client->IsConnected();
	}
}

int ClTobiIc::GetMessage(ICSerializerRapid* serializer) {
	if(this->_mode != ClTobiIc::GetOnly) {
		CcLogError("Cannot wait for messages when not GetOnly");
		return ClTobiIc::WrongMode;
	}

	if(this->IsAttached() == false)
		return ClTobiIc::Detached;
	if(this->_hasmessage.TryWait() == false)
		return ClTobiIc::NoMessage;
	return Deserialize(serializer);
}

int ClTobiIc::WaitMessage(ICSerializerRapid* serializer) {
	if(this->_mode != ClTobiIc::GetOnly) {
		CcLogError("Cannot get messages when not GetOnly");
		return ClTobiIc::WrongMode;
	}
	
	if(this->IsAttached() == false)
		return ClTobiIc::Detached;
	this->_hasmessage.Wait();
	if(this->IsAttached() == false)
		return ClTobiIc::Detached;
	return Deserialize(serializer);
}
		
int ClTobiIc::SetMessage(ICSerializerRapid* serializer, int blockidx) {
	if(this->_mode != ClTobiIc::SetOnly) {
		CcLogError("Cannot set messages when not SetOnly");
		return ClTobiIc::WrongMode;
	}
	
	if(this->IsAttached() == false)
		return ClTobiIc::Detached;

	std::string buffer;
	serializer->message->absolute.Tic();
	serializer->message->SetBlockIdx(blockidx);
	serializer->Serialize(&buffer);
	if(this->_client->Send(buffer) < 0)
		return ClTobiIc::Detached;	
	return ClTobiIc::MessageSet;
}

int ClTobiIc::Deserialize(ICSerializerRapid* serializer) {
	this->_sembuffer.Wait();
	try {
		serializer->Deserialize(&this->_buffer);
		this->_buffer.clear();
	} catch(TCException e) { 
		this->_sembuffer.Post();
		return ClTobiIc::NoMessage;
	}
	this->_sembuffer.Post();
	return ClTobiIc::HasMessage;
}

void ClTobiIc::HandleRecvPeer(CcSocket* caller, CcAddress addr, 
		CcStreamer* stream) { 
	if(this->_sembuffer.TryWait() == false) 
		return;

	if(stream->Extract(&this->_buffer, "<tobiic", "</tobiic>") == true) {
		this->_hasmessage.Post();
	}
	this->_sembuffer.Post();
	this->iOnHasMessage.Execute();
}

void ClTobiIc::HandleAccept(CcSocket* caller) { 
	CcServer *server = (CcServer*)caller;
	CcLogDebugS("Accepted TCP endpoint: " << server->GetRemote());
	this->iOnAccept.Execute();
}
		
void ClTobiIc::HandleDrop(CcSocket* caller) { 
	CcServer *server = (CcServer*)caller;
	CcLogDebugS("Dropped TCP endpoint: " << server->GetRemote());
	this->_hasmessage.Post();
	this->iOnDrop.Execute();
}

#endif
