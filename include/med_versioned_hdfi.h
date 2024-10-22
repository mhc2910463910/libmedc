/*  This file is part of MED.
 *
 *  COPYRIGHT (C) 1999 - 2020  EDF R&D, CEA/DEN
 *  MED is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MED is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with MED.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MED_VERSIONED_HDFI_H
#define MED_VERSIONED_HDFI_H

#include <stdarg.h>

/* Interfaces versionnées de l'API HDFI */
/* Il faut ajouter une ligne dans le fichier misc/MEDversionedApi.cxx */

extern void _MEDdatasetNumEcrire231 (int foo,...);
extern void _MEDdatasetNumEcrire232 (int foo,...);
extern void _MEDdatasetNumLire231   (int foo,...);
extern void _MEDdatasetNumLire232   (int foo,...);

#endif
