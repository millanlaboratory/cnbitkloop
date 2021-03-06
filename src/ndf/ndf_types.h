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

#ifndef NDFTYPES_H
#define NDFTYPES_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdlib.h>
#include <stdint.h>

#define	NDF_UINT8 	 	1
#define	NDF_UINT16	 	2
#define	NDF_UINT32		3
#define	NDF_UINT64		4
#define	NDF_INT8		5
#define	NDF_INT16		6
#define	NDF_INT32		7
#define	NDF_INT64		8
#define	NDF_FLOAT		9
#define	NDF_DOUBLE		10
#define	NDF_TIMEVAL		11
#define	NDF_DOUBLETIME	12

#define NDF_PRESET_UNKNOWN	0
#define NDF_PRESET_GTEC		100
#define NDF_PRESET_BIOSEMI	200
#define NDF_PRESET_GDF		300
#define NDF_PRESET_BDF		400

typedef unsigned int ndf_typeid;
size_t ndf_sizeof(ndf_typeid id);

#ifdef __cplusplus
}
#endif

#endif
