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
#include <hdf5.h>

/*
 * - Nom de la fonction : _MEDindiceNum
 * - Description : en argument de H5Giterate, donne le nombre
 *                 d'objets HDF (data set ou data group)
 *                 contenu dans l'objet HDF passe en argument
 * - Parametres :
 *     - id      (IN)     : l'ID de l'objet HDF
 *     - nom     (OUT)    : le nom du sous-objet
 *     - donnees (OUT)    : tampon
 * - Resultat : le nombre d'objets en cas de succes, -1 sinon
 */ 
med_err _MEDindiceNum(med_idt id,const char *nom, void *donnees)
{
  int *compteur;
  
  compteur = (int *) donnees;
  (*compteur)++;

  return 0;
}
