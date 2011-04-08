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

#ifndef CCLOGENTRY_HPP
#define CCLOGENTRY_HPP

#include <string>
#include <sstream>

class CcLogger;
typedef unsigned int CcLogLevel;

/*! \brief Entry for CcLogger
 */
class CcLogEntry {
	friend class CcLogger;

	public:
		CcLogEntry(CcLogLevel level, std::string message, std::string caller, 
				std::string file, int line);
		CcLogEntry(CcLogLevel level, std::stringstream message, std::string caller, 
				std::string file, int line);
		virtual ~CcLogEntry(void);
		virtual std::string Serialize(void);
		virtual CcLogLevel GetLevel(void);
		static const std::string GetLevelTxt(CcLogLevel level);
	public:
		static const CcLogLevel LevelDebug = 10;
		static const CcLogLevel LevelConfig = 20;
		static const CcLogLevel LevelInfo = 30; 
		static const CcLogLevel LevelException = 40;
		static const CcLogLevel LevelWarning = 50;
		static const CcLogLevel LevelError = 60;
		static const CcLogLevel LevelFatal = 70;
		
		static const std::string TxtDebug;
		static const std::string TxtConfig;
		static const std::string TxtInfo;
		static const std::string TxtException;
		static const std::string TxtWarning;
		static const std::string TxtError;
		static const std::string TxtFatal;
	protected:
		std::string _timestamp;
		std::string _caller;
		std::string _message;
		std::string _file;
		int _line;
		CcLogLevel _level;
};

#endif