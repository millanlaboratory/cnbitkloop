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

#ifndef CLPROLANG_HPP 
#define CLPROLANG_HPP 

#include "ClLanguage.hpp" 
#include <libcnbicore/CcBasic.hpp>
#include <libcnbiconfig/CCfgTypes.hpp>
#include <string>

class ClProLang : public ClLanguage {
	public:
		char* Fork(void);
		char* Launch(const int pid, const std::string& call);
		char* Terminate(const int pid);
		char* IsAlive(const int pid);
		char* Died(const int pid);
		char* ChangeDirectory(const int pid, const std::string& path);
		char* Include(const int pid, const std::string& path0, const std::string& path1);
		char* LaunchNDF(const int pid, const std::string& function, 
				const std::string& pipename, const CcAddress iD, 
				const CcAddress iC);
		char* Ok(const int pid);
		char* Error(const int code);
		bool IsFork(const char* message);
		bool IsLaunch(const char* message, int* pid, std::string* call);
		bool IsTerminate(const char* message, int* pid);
		bool IsIsAlive(const char* message, int* pid);
		bool IsDied(const char* message, int* pid);
		bool IsOk(const char* message, int* pid);
		bool IsError(const char* message, int* code);
		bool IsChangeDirectory(const char* message, int* pid, std::string* path);
		bool IsInclude(const char* message, int* pid, std::string* path0, 
				std::string* path1);
		bool IsLaunchNDF(const char* message, int* pid, std::string* function,
				std::string* pipename, std::string* iD, 
				std::string* iC);

	public:
		static const std::string Hdr;
		static const std::string Trl;
		static const int StatusLost = -3;
		static const int NotSupported = -2;
		static const int NotUndestood = -1;
		static const int NoReply = 0;
		static const int Successful = 1;
		static const int ForkFailed = 2;
		static const int IsDead = 3;
		static const int NotFound = 4;
};

#endif
