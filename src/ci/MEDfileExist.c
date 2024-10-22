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

#ifdef PPRO_NT_CALL
#define	R_OK	4		/* Test for read permission.  */
#define	W_OK	2		/* Test for write permission.  */
#define	X_OK	1		/* Test for execute permission.  */
#define	F_OK	0		/* Test for existence.  */
#else
#include <unistd.h>
#endif

#ifndef PPRO_NT_CALL
#include <libgen.h>
#endif

#include <string.h>


/**\ingroup MEDfile
  \brief \MEDfileExistBrief
  \param filename \filename
  \param accessmode \accessmode
  \param fileexist \fileexist
  \param accessok \accessok
  \retval med_err  \error
  \details \MEDfileExistDetails
  \par Remarques
*/

med_err
MEDfileExist(const char* const     filename,
	     const med_access_mode accessmode,
	     med_bool * const      fileexist,
	     med_bool * const      accessok )
{
  med_idt _fid      =  0;
  med_err _ret      = -1;
  int     _mode     = R_OK;
  char   *_filename = NULL;

#ifdef PPRO_NT_CALL
  char   _dirname[_MAX_PATH];
  char   _drive[_MAX_DRIVE];
  char   _direc[_MAX_DIR];
#else
  char   *_dirname  = NULL;
#endif

  /*
   * On inhibe le gestionnaire d'erreur HDF
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On ouvre le fichier MED sous HDF
   */ 
  if ( access(filename,F_OK) ) {
    /*Le fichier n'existe pas*/
    *fileexist = MED_FALSE;
    *accessok  = MED_FALSE;
    if (accessmode==MED_ACC_RDONLY) goto SORTIE;
    /*Si le fichier n'existe pas, il est necessaire d'avoir 
      les droits d'accès et d'écriture au répertoire*/

#ifndef PPRO_NT_CALL
    _mode      = _mode|W_OK|X_OK;
    if (!(_filename = strndup (filename,2*MED_PATHNAME_SIZE))) {
      MED_ERR_(_ret,MED_ERR_CALL,MED_ERR_API,"strndup");
      SSCRUTE(filename);SSCRUTE(_filename);goto ERROR;
    }
#else
    /* access windows ne support pas |X_OK; */
    /* - Cette fct ne vérifie pas les paramètres de sécurité du système de fichiers 
         (besoin d'un token pour çà) 
       - Dans les systèmes d’exploitation Windows 2000 et versions ultérieures, 
         tous les répertoires disposent d’un accès en lecture et en écriture.
    */
    _mode      = _mode|W_OK;
    if (!(_filename = _strdup (filename))) {
      MED_ERR_(_ret,MED_ERR_CALL,MED_ERR_API,"strdup");
      SSCRUTE(filename);SSCRUTE(_filename);goto ERROR;
    }
#endif

#ifndef PPRO_NT_CALL
    if (!(_dirname = dirname(_filename))) {
      MED_ERR_(_ret,MED_ERR_CALL,MED_ERR_API,"dirname");
      SSCRUTE(_filename);SSCRUTE(_dirname);goto ERROR;
    }
#else
    _splitpath(_filename, _drive, _direc, NULL, NULL);
    /*Le répertoire peut être vide */
    /* if (!strlen(_direc)) { */
    /*   MED_ERR_(_ret,MED_ERR_CALL,MED_ERR_API,"_splitpath"); */
    /*   SSCRUTE(_filename);SSCRUTE(_direc);SSCRUTE(_drive);goto ERROR; */
    /* } */
    // Join drive letter and directory name:
    _makepath(_dirname, _drive, _direc, NULL, NULL);
#endif
    if ( _MEDaccess(_dirname,_mode) )  *accessok = MED_FALSE; else *accessok = MED_TRUE;
    
  } else {

    /*Le fichier existe*/
    *fileexist = MED_TRUE;
    /*Si le mode d'accès au fichier est MED_ACC_RDWR our MED_ACC_CREATE, il faut vérifier l'accès en écriture*/
    (accessmode!=MED_ACC_RDONLY) && (_mode=_mode|W_OK);
    if ( _MEDaccess(filename,_mode) ) *accessok = MED_FALSE; else *accessok = MED_TRUE;
    
  }
  
  
 SORTIE:
  _ret=0;
  
 ERROR:

  if (_filename) free(_filename);
 
  return _ret;
}

