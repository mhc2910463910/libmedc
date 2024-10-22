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

#include <med.h>
#include <med_config.h>
#include <stdlib.h>

#ifdef PPRO_NT
#define F_OK 0
#else
#include <unistd.h>
#endif

#define nedfexit F77_FUNC(edfexit,EDFEXIT)

#ifdef PPRO_NT
med_int
 EDFEXIT(med_int *ret )
#else
med_int
nedfexit(med_int *ret)
#endif
{
  _exit((int) *ret);
  return *ret;
}

