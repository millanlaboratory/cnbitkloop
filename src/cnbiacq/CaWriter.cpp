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

#ifndef CAWRITER_CPP 
#define CAWRITER_CPP 

#include "CaWriter.hpp" 
#include <tobicore/TCBlock.hpp>
#include <tobicore/TCTimestamp.hpp>
#include <errno.h>
#include <string.h>


CaWriter::CaWriter(CaDevice* device) {
	this->_file = NULL;
	this->_device = device;
}

CaWriter::~CaWriter(void) {
	if(this->IsOpen())
		this->Close();
}
		
bool CaWriter::Setup(CaDevice* device) {
	this->_semlock.Wait();
	if(device == NULL && this->_device == NULL) {
		CcLogErrorS("Setup requires a CaDevice to be set");
		return false;
	} else if(device != NULL) {
		this->_device = device;
	} 

	if(xdf_set_conf(this->_file,
			XDF_F_REC_DURATION, 1.0,
			XDF_F_REC_NSAMPLE, this->_device->_cap->sampling_freq,
			XDF_NOF) == -1) {
		CcLogErrorS("Cannot setup header: " << strerror(errno));
		goto failure;
	}

	if(SetupChannelGroup(0) == -1) {
		CcLogErrorS("Cannot setup EEG group: " << strerror(errno));
		goto failure;
	}
	if(SetupChannelGroup(1) == -1) {
		CcLogErrorS("Cannot setup EXG group: " << strerror(errno));
		goto failure;
	}
	if(SetupChannelGroup(2) == -1) {
		CcLogErrorS("Cannot setup TRI group: " << strerror(errno));
		goto failure;
	}

	if(xdf_define_arrays(this->_file, 3, this->_device->_strides) == -1) {
		CcLogErrorS("Cannot define strides:" << strerror(errno));
		goto failure;
	}

	if(xdf_prepare_transfer(this->_file) == -1) {
		CcLogErrorS("Cannot prepare transfer: " << strerror(errno));
		goto failure;
	}

	this->_semlock.Post();
	return true;

failure:
	this->_semlock.Post();
	return true;
}

bool CaWriter::Open(const std::string& filename) {
	enum xdffiletype type;

	if(filename.find(".bdf") != std::string::npos) {
		type = XDF_BDF;
		CcLogConfig("XDF filetype detected: BDF");
	} else if(filename.find(".gdf") != std::string::npos) {
		type = XDF_GDF2;
		CcLogConfig("XDF filetype detected: GDFv2");
	} else {
		type = XDF_GDF2;
		CcLogWarning("Cannot detect filetype, defaulting to GDFv2");
	}
	return this->Open(filename, type);
}

bool CaWriter::Close(void) {
	this->_semlock.Wait();
	if(this->_file == NULL) {
		CcLogDebug("File is not open");
		goto failure;
	}

	if(xdf_close(this->_file) == -1) {
		CcLogErrorS("Cannot close file: " << strerror(errno));
		goto failure;
	}
	this->_file = NULL;
	this->_semlock.Post();
	return true;

failure:
	this->_semlock.Post();
	return true;
}

bool CaWriter::IsOpen(void) {
	bool status = false;
	this->_semlock.Wait();
	status = (this->_file != NULL);
	this->_semlock.Post();
	return status;
}

int CaWriter::Write(int nswrite) {
	this->_semlock.Wait();
	if(this->_file == NULL) {
		this->_semlock.Post();
		return -1;
	}

	int nswritten = xdf_write(this->_file, nswrite, 
			this->_device->_eeg, 
			this->_device->_exg, 
			this->_device->_tri);
	this->_semlock.Post();
	
	return nswritten;
}

int CaWriter::SetupChannelGroup(int igrp) {
	char label[32], transducter[128], unit[16], filtering[128];
	double mm[2];
	int isint = 0;
	struct xdfch* ch;

	egd_channel_info(this->_device->_dev, 
			this->_device->_grp[igrp].sensortype, 0,
			EGD_UNIT, unit,
			EGD_TRANSDUCTER, transducter,
			EGD_PREFILTERING, filtering,
			EGD_MM_D, mm,
			EGD_ISINT, &isint,
			EGD_EOL);
	
	isint = (this->_device->_grp[igrp].sensortype == EGD_TRIGGER);

	xdf_set_conf(this->_file, 
			XDF_CF_ARRINDEX, igrp,
			XDF_CF_ARROFFSET, 0,
			XDF_CF_ARRDIGITAL, 0,
			XDF_CF_ARRTYPE, isint ? XDFINT32 : XDFFLOAT,
			XDF_CF_STOTYPE, xdf_closest_type(this->_file, XDFFLOAT),
			XDF_CF_PMIN, mm[0],
			XDF_CF_PMAX, mm[1],
			XDF_CF_TRANSDUCTER, transducter,
			XDF_CF_PREFILTERING, filtering,
			XDF_CF_UNIT, unit,
			XDF_NOF);

	for(unsigned int j = 0; j < this->_device->_grp[igrp].nch; j++) {
		egd_channel_info(this->_device->_dev,
				this->_device->_grp[igrp].sensortype, j, 
				EGD_LABEL, label, EGD_EOL);
		if((ch = xdf_add_channel(this->_file, label)) == NULL)
			return -1;
	}
	return 0;
}

bool CaWriter::Open(const std::string& filename, enum xdffiletype type) {
	this->_semlock.Wait();
	if(this->_file != NULL) {
		CcLogError("File is already open");
		goto failure;
	}

	this->_file = xdf_open(filename.c_str(), XDF_WRITE, type);
	if(this->_file == NULL) {
		CcLogErrorS("Cannot open file: " << strerror(errno));
		goto failure;
	}
	this->_relative.Tic();
	this->_semlock.Post();
	return true;

failure:
	this->_semlock.Post();
	return false;
}

double CaWriter::TocOpen(void) {
	this->_semlock.Wait();
	double ms = -1;
	if(this->_file != NULL)
		ms = this->_relative.Toc();
	this->_semlock.Post();
	return ms;
}
		
void CaWriter::Tic(TCBlock* block) {
	this->_semlock.Wait();
	block->relative.Set(&this->_relative.timestamp);
	this->_semlock.Post();
}
		
bool CaWriter::AddEvent(int event, double duration) {
	this->_semlock.Wait();
	bool isopen = (this->_file != NULL);
	double onset = this->_relative.Toc()/1000.00f;
	int type = xdf_add_evttype(this->_file, event, "");
	if(type != -1) {
		if(xdf_add_event(this->_file, type, onset, duration) != -1) {
			this->_semlock.Post();
			return true;
		} 
	}
	this->_semlock.Post();
	if(isopen == true) {
		CcLogErrorS("Cannot add event " << event << ": " << strerror(errno));
	}
	return false;
}

#endif
