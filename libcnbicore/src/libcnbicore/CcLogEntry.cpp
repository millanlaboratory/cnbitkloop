/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCLOGENTRY_CPP
#define CCLOGENTRY_CPP

#include "CcLogEntry.hpp"
#include "CcTime.hpp"

const std::string CcLogEntry::TxtDebug = "Debug";
const std::string CcLogEntry::TxtConfig = "Config";
const std::string CcLogEntry::TxtInfo = "Info";
const std::string CcLogEntry::TxtException = "Exception";
const std::string CcLogEntry::TxtWarning = "Warning";
const std::string CcLogEntry::TxtError = "Error";
const std::string CcLogEntry::TxtFatal = "Fatal";

CcLogEntry::CcLogEntry(CcLogLevel level, std::string message,
		std::string caller, std::string file, int line) {
	this->_caller.assign(caller);
	this->_message.assign(message);
	this->_file.assign(file);
	this->_line = line;
	this->_level = level;
	CcTime::Datetime(&this->_timestamp);
}

CcLogEntry::CcLogEntry(CcLogLevel level, std::stringstream message, 
		std::string caller, std::string file, int line) {
	this->_caller.assign(caller);
	this->_file.assign(file);
	this->_line = line;
	this->_level = level;
	this->_message = message.str();
	CcTime::Datetime(&this->_timestamp);
}

CcLogEntry::~CcLogEntry(void) {
}

std::string CcLogEntry::Serialize(void) {
	std::string buffer;
	buffer.append("<CcLogEntry timestamp='");
	buffer.append(this->_timestamp);
	buffer.append("' level='");
	switch(this->_level) {
		case CcLogEntry::LevelConfig:
			buffer.append("Config");
			break;
		case CcLogEntry::LevelException:
			buffer.append("Exception");
			break;
		case CcLogEntry::LevelDebug:
			buffer.append("Debug");
			break;
		case CcLogEntry::LevelInfo:
			buffer.append("Info");
			break;
		case CcLogEntry::LevelWarning:
			buffer.append("Warning");
			break;
		case CcLogEntry::LevelError:
			buffer.append("Error");
			break;
		case CcLogEntry::LevelFatal:
			buffer.append("Fatal");
			break;
		default:
			buffer.append("Unknown");
			break;
	}
	buffer.append("'><caller>");
	buffer.append(this->_caller);
	buffer.append("</caller><file>");
	buffer.append(this->_file);
	buffer.append("</file><line>");
	std::stringstream tline;
	tline << this->_line;
	std::string sline = tline.str();
	buffer.append(sline);
	buffer.append("</line><message>");
	buffer.append(this->_message);
	buffer.append("</message></CcLogEntry>");
	
	return buffer;
}
		
CcLogLevel CcLogEntry::GetLevel(void) {
	return this->_level;
}

const std::string CcLogEntry::GetLevelTxt(CcLogLevel level) {
	switch(level) {
		case CcLogEntry::LevelDebug:
			return CcLogEntry::TxtDebug;
		case CcLogEntry::LevelConfig:
			return CcLogEntry::TxtConfig;
		case CcLogEntry::LevelInfo:
			return CcLogEntry::TxtInfo;
		case CcLogEntry::LevelException:
			return CcLogEntry::TxtException;
		case CcLogEntry::LevelWarning:
			return CcLogEntry::TxtWarning;
		case CcLogEntry::LevelError:
			return CcLogEntry::TxtError;
		case CcLogEntry::LevelFatal:
			return CcLogEntry::TxtFatal;
	}
	return "undef";
}
#endif