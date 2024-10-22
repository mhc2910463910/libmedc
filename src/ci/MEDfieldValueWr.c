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


#include <med_config.h>
#include <med.h>
#include <med_outils.h>
#include <string.h>
#include <stdlib.h>

/**\ingroup MEDfield
  \brief \MEDfieldValueWrBrief
  \param fid \fid
  \param fieldname \fieldname
  \param numdt \numdt
  \param numit \numit
  \param dt \dt
  \param entitype \entitype
  \param geotype \geotype
  \param switchmode \switchmode
  \param componentselect \componentselect
  \param nentity \nentity
  \param value \value
  \retval med_err \error
  \details \MEDfieldValueWrDetails
  \remarks
  \MEDfieldValueWrRem
 */

med_err MEDfieldValueWr(const med_idt              fid,
			const char*  const         fieldname,
			const med_int              numdt,
			const med_int              numit,
			const med_float            dt,
			const med_entity_type      entitype,
			const med_geometry_type    geotype,
			const med_switch_mode      switchmode,
			const med_int              componentselect,
			const med_int              nentity,
			const unsigned char* const value)
{
  char *  name = "_MEDfieldValueAdvancedWr";
  int     dummy=0;
  med_err fret=-1;
  med_int majeur=0, mineur=0, release=0;
  MedFuncType func;


/*   ISCRUTE((*filter).nentity              ); */
/*   ISCRUTE((*filter).nvaluesperentity     ); */
/*   ISCRUTE((*filter).nconstituentpervalue ); */
/*   ISCRUTE((*filter).constituentselect       ); */
/*   ISCRUTE((*filter).switchmode              ); */
/*   ISCRUTE((*filter).filterarraysize         ); */
/*   ISCRUTE((*filter).profilearraysize        ); */
/*   ISCRUTE((*filter).profilemode             ); */
/*   SSCRUTE((*filter).profilename             ); */


  MEDfileNumVersionRd(fid, &majeur, &mineur, &release);

  /* ISCRUTE(majeur); */
  /* ISCRUTE(mineur); */
  /* ISCRUTE(release); */

  func = _MEDversionedApi3(name,majeur,mineur,release);
  if ( func != (MedFuncType) NULL )
    func (dummy,
	  fid,
	  fieldname,
	  numdt,
	  numit,
	  dt,
	  entitype,
	  geotype,
	  MED_UNDEF_STMODE,
	  MED_NO_PROFILE,
	  MED_NO_LOCALIZATION,
	  switchmode,
	  componentselect,
	  NULL,
	  nentity,
	  value,
	  &fret);

  return fret;
}
