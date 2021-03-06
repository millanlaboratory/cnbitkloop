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

#include "ClLoop.hpp"
#include "ClLoopConfigSrv.hpp"
#include "ClAcqServer.hpp" 
#include "ClDevServer.hpp" 
#include "ClBusServer.hpp" 
#include "ClAcqLang.hpp"
#include "ClNmsClient.hpp"
#include <cnbiacq/CaWriter.hpp>
#include <cnbiacq/CaDevice.hpp>
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcPipeServer.hpp>
#include <cnbicore/CcServer.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define PIPELINES 15

using namespace std;

void idle(void) {
	char a;
	printf("Press Enter to initiate acquisition");
	cin >> a;
}

void usage(void) { 
	printf("Usage: cl_acquisition [OPTION]...\n\n");
	printf("  -d       device: gtec, biosemi, GDF/BDF file, EGD string\n");
	printf("  -f       buffering rate in Hz (16 default)\n");
	printf("  -n       basename for the pipes (/tmp/cl.pipe.ndf. default)\n");
	printf("  -i       interactive acquisition starting\n");
	printf("  -p       print labels added to NDF frame\n");
	printf("  -b       print EGD/NDF buffer informations\n");
	printf("  -w       warn when late\n");
	printf("  -r       re-open device once down\n");
	printf("  -h       display this help and exit\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optdevice("");
	std::string optfs("16");
	std::string optpipename("/tmp/cl.pipe.ndf.");
	bool optinteractive = false, optprintndf = false, optprintbuffers = false,
		 optwarnlate = false, optreopen = false;

	while((opt = getopt(argc, argv, "d:f:l:n:hipwbr")) != -1) {
		if(opt == 'd')
			optdevice.assign(optarg);
		else if(opt == 'f')
			optfs.assign(optarg);
		else if(opt == 'n')
			optpipename.assign(optarg);
		else if(opt == 'i')
			optinteractive = true;
		else if(opt == 'r')
			optreopen = true;
		else if(opt == 'p')
			optprintndf = true;
		else if(opt == 'w')
			optwarnlate = true;
		else if(opt == 'b')
			optprintbuffers = true;
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	CcCore::OpenLogger("cl_acquisition");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	ClLoop::Configure("", true);
	
	CcLogConfigS("Acquisition: " << 
			optfs << "Hz, " << 
			optpipename << "[0," << PIPELINES-1 << "]");
	CcLogConfigS("Pipelines " << PIPELINES << ": " <<
			"/pipe[0," << PIPELINES-1 << "] and /ctrl[0," << PIPELINES-1 << "]");

	// Variables for mainloop
	CcTimeValue ticSignals;
	size_t gsize = -1, asize = -1;
	
	// Prepare NDF frame and its semafore
	ndf_frame frame;
	CcSemaphore semframe;

	// Open, Setup and Start EGD device
	CaDevice eegdev;
	if(eegdev.Open(optdevice) == false) {
		CcLogFatal("Cannot open EGD device");
		CcCore::Exit(2);
	}
	if(eegdev.Setup((float)atof(optfs.c_str())) == false) {
		CcLogFatal("Cannot setup EGD device");
		CcCore::Exit(3);
	}
	// Initialize XDF writer and NDF frame
	CaWriter writer(&eegdev);
	eegdev.InitNDF(&frame);
	CcLogConfigS("NDF/EGD configured:" <<
			" Sf=" << frame.config.sf << " Hz" <<
			", Block=" << frame.config.samples << " samples" << 
			", EEG=" << frame.config.eeg_channels <<
			", EXG=" << frame.config.exg_channels <<
			", TRI=" << frame.config.tri_channels);

	// Setup XDF writer
	std::string tmpfn;
	CcTime::Timestamp(&tmpfn);
	tmpfn.append(".bdf");

	// Setup CcPipeServer
	CcPipeServer* pipes;
	pipes = new CcPipeServer(frame.ack.buffer, frame.ack.bsize,
			frame.data.bsize);
	pipes->Open(optpipename, PIPELINES);
	
	CcLogConfigS("Acq configured as: " << ClLoopConfigSrv::GetSrvAcq());
	CcLogConfigS("Bus configured as: " << ClLoopConfigSrv::GetSrvBus());
	CcLogConfigS("Dev configured as: " << ClLoopConfigSrv::GetSrvDev());

	// Initialize nameserver client
	ClNmsClient nsclient;
	int nsstatus;
	
	CcServer serverAcq(CCCORE_1MB);
	ClAcqServer handleAcq(&writer);
	handleAcq.Register(&serverAcq);
	if(serverAcq.Bind(ClLoopConfigSrv::GetSrvAcq()) == false) {
		CcLogFatal("Cannot bind acquisition socket");
		CcCore::Exit(4);
	}

	CcServer serverBus(CCCORE_1MB);
	ClBusServer handleBus(&writer, &frame, &semframe);
	handleBus.Register(&serverBus);
	if(serverBus.Bind(ClLoopConfigSrv::GetSrvBus()) == false) {
		CcLogFatal("Cannot bind bus socket");
		CcCore::Exit(5);
	}
	
	CcServer serverDev(CCCORE_1MB);
	ClDevServer handleDev(&writer, &frame, &semframe);
	handleDev.Register(&serverDev);
	if(serverDev.Bind(ClLoopConfigSrv::GetSrvDev()) == false) {
		CcLogFatal("Cannot bind dev socket");
		CcCore::Exit(50);
	}

	if(nsclient.Connect(ClLoopConfigSrv::GetNms()) == false) {
		CcLogFatal("Cannot connect to nameserver");
		CcCore::Exit(6);
	}

	nsstatus = nsclient.Set("/acquisition", ClLoopConfigSrv::GetAcq());
	if(nsstatus != ClNmsLang::Successful) {
		CcLogFatal("Cannot register acquisition with nameserver");
		CcCore::Exit(7);
	}
	
	nsstatus = nsclient.Set("/bus", ClLoopConfigSrv::GetBus());
	if(nsstatus != ClNmsLang::Successful) {
		CcLogFatal("Cannot register bus with nameserver");
		CcCore::Exit(8);
	}
	
	nsstatus = nsclient.Set("/dev", ClLoopConfigSrv::GetDev());
	if(nsstatus != ClNmsLang::Successful) {
		CcLogFatal("Cannot register dev with nameserver");
		CcCore::Exit(8);
	}
	
	// Register pipes on nameserver
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream pipename, pipepath;
		pipename << "/pipe" << p;
		pipepath << optpipename << p;
		int nsstatus = nsclient.Set(pipename.str(), pipepath.str());
		if(nsstatus != ClNmsLang::Successful) {
			CcLogFatal("Cannot register pipes with nameserver");
			CcCore::Exit(9);
		}
	}
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream ctlname, ctrladdr;
		ctlname << "/ctrl" << p;
		ctrladdr << ClLoopConfigSrv::GetSrvIp() << ":950" << p;
		int nsstatus = nsclient.Set(ctlname.str(), ctrladdr.str());
		if(nsstatus != ClNmsLang::Successful) {
			CcLogFatal("Cannot register controllers with nameserver");
			CcCore::Exit(10);
		}
	}

	if(optinteractive == true)
		idle();

	if(eegdev.Start() == false) {
		CcLogFatal("Cannot start EGD device");
		goto shutdown;
	}
	CcLogInfo("Started EGD device");

	CcTime::Tic(&ticSignals);
	while(true) {
		gsize = eegdev.GetData();
		asize = eegdev.GetAvailable();
		if(gsize == (size_t)-1) { 
			if(optreopen == false) {
				CcLogFatal("EGD device is down");
				break;
			} else {
				CcLogWarning("EGD device is down");
				eegdev.Close();
				if(eegdev.Open(optdevice) == false) {
					CcLogFatal("Cannot re-open EGD device");
					break;
				}
				if(eegdev.Setup((float)atof(optfs.c_str())) == false) {
					CcLogFatal("Cannot re-setup EGD device");
					break;
				}
				eegdev.Start();
				CcLogInfo("EGD device was successfully re-opened");
			}
			continue;
		}
		writer.Write(gsize);
		
		semframe.Wait();
		eegdev.WriteNDF(&frame);
		pipes->Write(frame.data.buffer, 0);
		if(optprintndf) 
			ndf_print_labels(&frame);
		ndf_clear_labels(&frame);
		semframe.Post();
		
		if(CcTime::Toc(&ticSignals) >= 500.00f) {
			if(nsclient.IsConnected() == false)  {
				CcLogFatal("Lost connection with nameserver");
				break;
			}
			if(CcCore::receivedSIGAny.Get()) 
				break;
			CcTime::Tic(&ticSignals);
		}
		if(asize > 0 && optwarnlate)
			CcLogWarningS("Running late: Get/Avail=" << gsize << "/" << asize);
		if(optprintbuffers) 
			printf("Get=%5.5lu, Avail=%5.5lu %s\n", 
					gsize, asize, asize > 0 ? "Warning: running late!" : "");
	}

shutdown:
	// Stop and unregister ClAcqServer
	serverAcq.Release();
	nsclient.Unset("/acquisition");
	
	serverBus.Release();
	nsclient.Unset("/bus");
	
	serverDev.Release();
	nsclient.Unset("/dev");
	
	// Deregister pipes on nameserver
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream pipename;
		pipename << "/pipe" << p;
		nsclient.Unset(pipename.str());
	}
	// Deregister pipes on nameserver
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream ctrl;
		ctrl << "/ctrl" << p;
		nsclient.Unset(ctrl.str());
	}

	if(writer.Close() == false)
		CcLogError("Cannot stop XDF writer");
	if(eegdev.Stop() == false)
		CcLogError("Cannot stop EGD device");
	if(eegdev.Close() == false) 
		CcLogError("Cannot close EGD device");

	delete pipes;
	ndf_free(&frame);

	CcCore::Exit(0);
}
