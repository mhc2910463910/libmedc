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
#include <med_outils.h>

#include <stdlib.h>
#include <string.h>

med_err 
MEDglobalNumLire(med_idt fid,char *maa, med_int *num, med_int n, 
		 med_entite_maillage type_ent,med_geometrie_element type_geo)
{
  med_idt root=0,maaid=0, entid=0, geoid=0;
  med_err ret=-1;
  char chemin [MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  char nom_ent[MED_TAILLE_NOM_ENTITE+1];
  char nom_geo[MED_TAILLE_NOM_ENTITE+1];
  med_entite_maillage _type_ent=type_ent;

  if ( type_ent == MED_NOEUD_MAILLE ) _type_ent=MED_NOEUD ;

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();
if (MEDcheckVersion(fid) < 0) return -1;


  /*
   * Si le maillage n'existe pas => erreur
   */
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  if ((maaid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    goto ERREUR;

  /*
   * On met a jour le nom du Data Group representant
   * le type des entites
   */
  if ( _MEDnomEntite(nom_ent,_type_ent ) < 0)
    goto ERREUR;

  /*
   * Si le Data Group des entites n'existe pas => erreur
   */
  if ((entid = _MEDdatagroupOuvrir(maaid,nom_ent)) < 0)
    goto ERREUR;

  /*
   * Pour les mailles, les faces et le aretes, on cree
   * si le Data Group du type geometrique => erreur
   */
  if ((_type_ent==MED_MAILLE)||(_type_ent==MED_FACE)||(_type_ent==MED_ARETE)) {
    if ( _MEDnomGeometrie30(nom_geo,type_geo) < 0)
      goto ERREUR;
    if ((geoid = _MEDdatagroupOuvrir(entid,nom_geo)) < 0)
      goto ERREUR;
  } else
    geoid = -1;

  /*
   * lecture du Data Set "GLB"
   */
  if (geoid == -1)
    root = entid;
  else
    root = geoid;

#if defined(HAVE_F77INT64)
  if ( _MEDdatasetNumLire(root,MED_NOM_GLB,MED_INT64,
			  MED_NO_INTERLACE,1,MED_ALL,
			  MED_NOPF,MED_NO_PFLMOD,MED_PFL_NON_COMPACT,0,MED_NOPG,0,
			  (unsigned char *)num) < 0 )
    goto ERREUR;
#else
  if ( _MEDdatasetNumLire(root,MED_NOM_GLB,MED_INT32,
			  MED_NO_INTERLACE,1,MED_ALL,
			  MED_NOPF,MED_NO_PFLMOD,MED_PFL_NON_COMPACT,0,MED_NOPG,0,
			  (unsigned char *)num) < 0 )
    goto ERREUR;
#endif

  ret = 0;
 ERREUR:

  /*
   * On ferme tout
   */
  if (geoid>0) if (_MEDdatagroupFermer(geoid) < 0) {
    MESSAGE("Impossible de fermer le datagroup : ");
    ISCRUTE_id(geoid); ret = -1; 
  }

  if (entid>0) if (_MEDdatagroupFermer(entid) < 0) {
    MESSAGE("Impossible de fermer le datagroup : ");
    ISCRUTE_id(entid); ret = -1; 
  }

  if (maaid>0) if (_MEDdatagroupFermer(maaid) < 0) {
    MESSAGE("Impossible de fermer le datagroup : ");
    ISCRUTE_id(maaid); ret = -1; 
  }

  return ret; 
}








