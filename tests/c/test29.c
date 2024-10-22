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

/******************************************************************************
 * - Nom du fichier : test29.c
 *
 * - Description : ecriture d'un joint dans un maillage MED 
 *
 *****************************************************************************/

#include <med.h>
#define MESGERR 1
#include <med_utils.h>

#ifdef DEF_LECT_ECR
#define MODE_ACCES MED_ACC_RDWR
#elif DEF_LECT_AJOUT
#define MODE_ACCES MED_ACC_RDEXT
#else
#define MODE_ACCES MED_ACC_CREAT
#endif

int main (int argc, char **argv)


{
  med_idt fid;

  char maa[MED_NAME_SIZE+1]= "maa1";
  char jnt[MED_NAME_SIZE+1] = "joint";
  char des_jnt[MED_COMMENT_SIZE+1] = "joint avec le sous-domaine 2";
  char maa_distant[MED_NAME_SIZE+1]= "maa1";
  med_int dom_dist = 2;

  med_int mdim = 3;
  med_int ncor = 3;
  med_int cor[6] = {1,2,3,4,5,6};
  med_int cor2[6] = {10,20,30,40,50,60};
  char nomcoo[3*MED_SNAME_SIZE+1] = "x               y               z               ";
  char unicoo[3*MED_SNAME_SIZE+1] = "cm              cm              cm              ";


  /* Creation du fichier "test29.med" */
  if ((fid = MEDfileOpen("test29.med",MODE_ACCES)) < 0) {
    MESSAGE("Erreur a la creation du fichier test29.med");
    return -1;
  }

  if (MEDmeshCr( fid, maa, mdim, mdim, MED_UNSTRUCTURED_MESH,
		 "un maillage pour test29","s", MED_SORT_DTIT,
		 MED_CARTESIAN, nomcoo, unicoo) < 0) {
    MESSAGE("Erreur a la creation du maillage : "); SSCRUTE(maa);
    return -1;
  }

  /* Creation du joint */
  if (MEDsubdomainJointCr(fid,maa,jnt,des_jnt,dom_dist,maa_distant) < 0) {
    MESSAGE("Erreur a la creation du joint");
    return -1;
  }

  /* Ecriture de la correspondance Noeud, Noeud */
  if (MEDsubdomainCorrespondenceWr(fid,maa,jnt,MED_NO_DT,MED_NO_IT,
				   MED_NODE,MED_NONE,MED_NODE,MED_NONE,ncor,cor) < 0) {
    MESSAGE("Erreur a l'ecriture du tableau des correspondances (noeud,noeud)");
    return -1;
  }

  /* Ecriture de la correspondance Noeud Maille */
  if (MEDsubdomainCorrespondenceWr(fid,maa,jnt,MED_NO_DT,MED_NO_IT,
				   MED_NODE,MED_NONE,MED_CELL,MED_TRIA3,ncor,cor2) < 0) {
    MESSAGE("Erreur a l'ecriture du tableau des correspondances (noeud,maille TRIA3)");
    return -1;
  }

  /* Fermeture du fichier */
  if (MEDfileClose(fid) < 0) {
    MESSAGE("Erreur a la fermeture du fichier");
    return -1;
  }

  return 0;
}




