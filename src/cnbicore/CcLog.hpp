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

#ifndef CCLO_CPP
#define CCLO_CPP

#include "CcCore.hpp"
#include "CcLogger.hpp"

#define CcLog(level, message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(level, message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogConfig(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelConfig,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogException(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelException,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogDebug(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelDebug,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogInfo(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelInfo,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogWarning(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelWarning,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogError(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelError,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogFatal(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelFatal,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogS(level, message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(level, (char*)message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogConfigS(...)\
	{\
	std::stringstream _cclogmessage;\
	_cclogmessage << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelConfig,\
				_cclogmessage.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	_cclogmessage.str("");\
	}

#define CcLogExceptionS(...)\
	{\
	std::stringstream _cclogmessage;\
	_cclogmessage << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelException,\
				_cclogmessage.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	_cclogmessage.str("");\
	}

#define CcLogDebugS(...)\
	{\
	std::stringstream _cclogmessage;\
	_cclogmessage << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelDebug,\
				_cclogmessage.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	_cclogmessage.str("");\
	}

#define CcLogInfoS(...)\
	{\
	std::stringstream _cclogmessage;\
	_cclogmessage << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelInfo,\
				_cclogmessage.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	_cclogmessage.str("");\
	}

#define CcLogWarningS(...)\
	{\
	std::stringstream _cclogmessage;\
	_cclogmessage << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelWarning,\
				_cclogmessage.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	_cclogmessage.str("");\
	}

#define CcLogErrorS(...)\
	{\
	std::stringstream _cclogmessage;\
	_cclogmessage << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelError,\
				_cclogmessage.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	_cclogmessage.str("");\
	}

#define CcLogFatalS(...)\
	{\
	std::stringstream _cclogmessage;\
	_cclogmessage << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelFatal,\
				_cclogmessage.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	_cclogmessage.str("");\
	}

#define CcLogConfigSH(message, header, ...)\
	{CcLogConfigS(message, "[" << header << "] " << __VA_ARGS__);}

#define CcLogExceptionSH(message, header, ...)\
	{CcLogExceptionS(message, "[" << header << "] " << __VA_ARGS__);}

#define CcLogDebugSH(message, header, ...)\
	{CcLogDebugS(message, "[" << header << "] " << __VA_ARGS__)};

#define CcLogInfoSH(message, header, ...)\
	{CcLogInfoS(message, "[" << header << "] " << __VA_ARGS__)};

#define CcLogWarningSH(message, header, ...)\
	{CcLogWarningS(message, "[" << header << "] " << __VA_ARGS__)};

#define CcLogErrorSH(message, header, ...)\
	{CcLogErrorS(message, "[" << header << "] " << __VA_ARGS__)};

#define CcLogFatalSH(message, header, ...)\
	{CcLogFatalS(message, "[" << header << "] " << __VA_ARGS__)};

#endif
