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

#ifndef CCOBJECT_HPP
#define CCOBJECT_HPP

#include "CcCore.hpp"

/*! \brief Root object
 *
 * All the classes in LibCNBICore inherit from CcObject. 
 * Nothing special for the time being.
 * If you are handling streams, I strongly suggest you use this class.
 */
class CcObject {
	friend class CcCore;
	public:
		CcObject(void);
		virtual ~CcObject(void);
		unsigned int GetInstances(void) const;
		const void DumpInfo(void) const;

	private:
		static unsigned int _objectInstances;
		unsigned int _objectId;
		CcCore* _core;
};

#endif
