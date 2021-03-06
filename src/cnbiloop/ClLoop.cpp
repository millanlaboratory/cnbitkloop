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

#ifndef CLLOOP_CPP 
#define CLLOOP_CPP 

#include "ClLoop.hpp" 
#include "ClLoopConfig.hpp"
#include "ClLoopConfigSrv.hpp"
#include <cnbicore/CcSocket.hpp>

/* Initialization */
ClLoop* ClLoop::_instance = NULL;
unsigned int ClLoop::_refCount = 0;

/* Declarations */	
ClProClient ClLoop::pro;
ClAcqClient ClLoop::acq;
ClNmsClient ClLoop::nms;

ClLoop::ClLoop(void) {
}

ClLoop::~ClLoop(void) {
	ClLoop::Disconnect();
}

ClLoop* ClLoop::Instance(void) {
	if(ClLoop::_instance == NULL)
		ClLoop::_instance = new ClLoop;
	++ClLoop::_refCount;
	ClLoopConfig::Instance();
	return ClLoop::_instance;
}

unsigned int ClLoop::Refcount(void) {
	return ClLoop::_refCount;
}

void ClLoop::Release(void) {
	if(--ClLoop::_refCount < 1) 
		ClLoop::Destroy();
	ClLoopConfig::Release();
}

void ClLoop::Destroy(void) {
	if(ClLoop::_instance == NULL) 
		return;
	delete ClLoop::_instance;
	// Edited by L.Tonin  <luca.tonin@epfl.ch> on 12/07/17 16:00:08
	// Added to fix segmentation fault
	ClLoop::_instance = NULL;
}

bool ClLoop::Connect(void) {
	CcAddress nameserver = ClLoopConfig::GetNms();
	/*
	CcAddress envvar = CcCore::GetEnvCnbiTkAddress();
	if(envvar.empty() == false) {
		CcLogDebugS("$CNBITK_ADDRESS defined as " << envvar);
		nameserver = CcSocket::Lookup(envvar) + ":8123";
	} else {
		CcLogDebug("$CNBITK_ADDRESS not defined, assuming localhost");
		nameserver = "127.0.0.1:8123";
	}
	*/

	int status = ClLoop::Connect(nameserver);
	CcLogDebugS("CnbiTk loop nameserver: " << ClLoopConfig::GetNms());
	CcLogDebugS("CnbiTk loop pro: " << ClLoopConfig::GetPro());
	CcLogDebugS("CnbiTk loop acq: " << ClLoopConfig::GetAcq());
	return status; 
}
		
bool ClLoop::Connect(CcAddress nameserver) {
	ClLoopConfig::GetNms() = nameserver;
	if(ClLoop::ConnectNms() == false)
		return false;
	if(ClLoop::ConnectPro() == false)
		return false;
	if(ClLoop::ConnectAcq() == false)
		return false;
	return true;
}
		
bool ClLoop::Connect(CCfgConfig* configuration) {
	CcAddress nameserver;
	try {
	nameserver =
		configuration->RootEx()->QuickStringEx("configuration/clloop/nameserver");
	} catch(CcException e) {
		CcLogError("Configuration failed, using default values");
		return ClLoop::Connect();
	}
	return ClLoop::Connect(nameserver);
}
		
void ClLoop::Disconnect(void) {
	ClLoop::pro.Disconnect();
	ClLoop::acq.Disconnect();
	ClLoop::nms.Disconnect();
}
		
bool ClLoop::IsConnected(void) {
	return ClLoop::pro.IsConnected() &&
		ClLoop::acq.IsConnected() &&
		ClLoop::nms.IsConnected();
}

bool ClLoop::ConnectNms(void) {
	if(ClLoop::nms.Connect(ClLoopConfig::GetNms())) 
		return true;

	CcLogDebugS("Cannot connect to Nms: " << ClLoopConfig::GetNms());
	return false;
}

bool ClLoop::ConnectPro(void) {
	if(ClLoop::pro.Connect(ClLoopConfig::GetPro())) 
		return true;

	CcLogDebugS("Cannot connect to Pro: " << ClLoopConfig::GetPro());
	return false;
}

bool ClLoop::ConnectAcq(void) {
	if(ClLoop::acq.Connect(ClLoopConfig::GetAcq())) 
		return true;

	CcLogDebugS("Cannot connect to Acq: " << ClLoopConfig::GetAcq());
	return false;
}
		
bool ClLoop::Configure(CcAddress nameserver, bool asserver) {
	bool status = true;
	if(nameserver.empty() == true)
		ClLoopConfig::ImplLoad();
	else
		status = ClLoop::Query(nameserver);

	if(asserver == true)
		ClLoopConfigSrv::ConfigureSrv();

	if(status == true) {
		CcLogInfo("Loop correctly configured");
	} else {
		CcLogError("Cannot configure loop");
	}
	
	return status;
}

bool ClLoop::Query(CcAddress nameserver) {
	CcLogConfigS("Querying nameserver: " << nameserver);
	ClLoopConfig::Init();
	CcEndpoint cache;

	cache.SetAddress(nameserver);
	ClLoopConfig::ip.assign(cache.GetIp());
	ClLoopConfig::portNms.assign(cache.GetPort());
	if(ClLoop::ConnectNms() == false) {
		return false;
	}
	
	CcAddress addrPro, addrAcq;
	int statusPro = ClLoop::nms.Query("/processing", &addrPro);
	int statusAcq = ClLoop::nms.Query("/acquisition", &addrAcq);

	cache.SetAddress(addrPro);
	ClLoopConfig::portPro.assign(cache.GetPort());
	
	cache.SetAddress(addrAcq);
	ClLoopConfig::portAcq.assign(cache.GetPort());
	
	if(statusPro != ClNmsLang::Successful || statusAcq != ClNmsLang::Successful)
		return false;
	return true;
}
		
#endif
