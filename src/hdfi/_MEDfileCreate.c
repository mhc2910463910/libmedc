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

#include <string.h>
#include "med_filechar.h"

/*
 * - Nom de la fonction : _MEDfichierCreer
 * - Description : creation d'un fichier HDF
 * - Parametres :
 *     - nom (IN) : le nom du fichier
 * - Resultat : ID du fichier en cas de succes, -1 sinon
 */
med_idt _MEDfileCreate(const char * const filename, const med_access_mode accessmode,
		       const med_int major,
		       const med_int minor,
		       const med_int release)
{
  med_idt _fid=-1,_gid=-1;
  med_int _major   	  = 0;
  med_int _minor   	  = 0;
  med_int _release 	  = 0;
  uint8_t _i8_major   	  = 0;
  uint8_t _i8_minor   	  = 0;
  uint8_t _i8_release 	  = 0;
  hid_t   _fapl    	  = H5P_DEFAULT;
  uint8_t *_h518medfileMMN=NULL;
  FILE    *_fp            =NULL;
  size_t  _fpwritten      =0; 
  med_bool _hdfok 	  = MED_FALSE;
  med_bool _medok 	  = MED_FALSE;
  /* H5AC_cache_config_t config; */

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On autorise depuis 41 la création d'un nouveau fichier de Majeur différent 
   * de celui de la bibliothèque.
   */
  if ( major == 3 ) {
    
    _i8_major = major;
    if ( (minor < 0) || (minor > MED_3_LATEST_MINOR) ) {
      MED_ERR_(_fid,MED_ERR_RANGE,MED_ERR_PROPERTY,MED_ERR_FILEVERSION_MSG);
      ISCRUTE(major);ISCRUTE(minor);ISCRUTE(release);
      ISCRUTE(MED_3_LATEST_MINOR);
      goto ERROR;
    }
    _i8_minor=minor;
    
    _h518medfileMMN = malloc(h518medfileMMN_SIZE);
    memcpy(_h518medfileMMN,h518medfileMMN,h518medfileMMN_SIZE);
    /* Les numéros sont gérés sur 1 digit,
     * Accès en int8 pour éviter les pbs BE/LE */
    _h518medfileMMN[MED_FILE_H518_MAJOR  ]= _i8_major;
    _h518medfileMMN[MED_FILE_H518_MINOR  ]= _i8_minor;
    _h518medfileMMN[MED_FILE_H518_RELEASE]= _i8_release; /* 0 */

    _fp = fopen(filename, "w");
    if(!_fp) {
      MED_ERR_(_fid,MED_ERR_CREATE,MED_ERR_FILE,filename);
      ISCRUTE(major);ISCRUTE(minor);ISCRUTE(release);
      ISCRUTE_int8(_i8_major);ISCRUTE_int8(_i8_minor);ISCRUTE_int8(_i8_release);
      goto ERROR;
    }
    _fpwritten = fwrite(_h518medfileMMN, sizeof(_h518medfileMMN[0]), h518medfileMMN_SIZE, _fp);
    if ( (_fpwritten != h518medfileMMN_SIZE) || (ferror(_fp)) ) {
      MED_ERR_(_fid,MED_ERR_NOTEQUAL,MED_ERR_PROPERTY,MED_ERR_NAME_MSG);
      ISCRUTE_int(_fpwritten);ISCRUTE_int(h518medfileMMN_SIZE);
      ISCRUTE(major);ISCRUTE(minor);ISCRUTE(release);
      ISCRUTE_int8(_i8_major);ISCRUTE_int8(_i8_minor);ISCRUTE_int8(_i8_release);
      goto ERROR;
    }
    fclose(_fp);
    free(_h518medfileMMN);_h518medfileMMN=NULL;

    if ( MEDfileCompatibility(filename, &_hdfok, &_medok) < 0 ) {
      MED_ERR_(_fid, MED_ERR_CALL,MED_ERR_API,"MEDfileCompatibility");
      ISCRUTE(major);ISCRUTE(minor);ISCRUTE(release);
      ISCRUTE_int8(_i8_major);ISCRUTE_int8(_i8_minor);ISCRUTE_int8(_i8_release);
      goto ERROR;
    }
    if ( ! (_hdfok && _medok) ) {
      MED_ERR_(_fid,MED_ERR_CREATE,MED_ERR_FILE,filename);
      ISCRUTE_int(_hdfok);
      ISCRUTE_int(_medok);
      ISCRUTE(major);ISCRUTE(minor);ISCRUTE(release);
      ISCRUTE_int8(_i8_major);ISCRUTE_int8(_i8_minor);ISCRUTE_int8(_i8_release);
      goto ERROR;
    }

    if ((_fid = _MEDfileOpen((char*) filename, accessmode)) < 0) {
      MED_ERR_(_fid,MED_ERR_OPEN,MED_ERR_FILE,filename);
      goto ERROR;
    }

    goto SORTIE;
  }

  if ( major != MED_NUM_MAJEUR ) {
    MED_ERR_(_fid,MED_ERR_RANGE,MED_ERR_PROPERTY,MED_ERR_FILEVERSION_MSG);
    ISCRUTE(major);ISCRUTE(minor);ISCRUTE(release);
    goto ERROR;
  } else
    _major = MED_NUM_MAJEUR;
  
  _minor = minor;
  switch(minor)
    {
    case 0: _release = MED_40_LATEST_RELEASE; break;
#if (MED_NUM_MINEUR == 0) && (MED_NUM_RELEASE > 1)
#error "Don't forget to change the value of MED_40_LATEST_RELEASE when you change the release of version 4.0 of the library !"
#endif
    /* case 1: _release = 0; break; */
    /* case 2: _release = 1; break; */
#if MED_NUM_MINEUR > 1
#error "Don't forget to add a case line version here when you change the minor version of the library !"
#endif
#if MED_NUM_MAJEUR != 4
#error "Don't forget to add a case line version here when you change the major version of the library !"
#endif
    case MED_NUM_MINEUR: _release = MED_NUM_RELEASE; break;
    default:
      MED_ERR_(_fid,MED_ERR_RANGE,MED_ERR_PROPERTY,MED_ERR_FILEVERSION_MSG);
      ISCRUTE(major);ISCRUTE(minor);ISCRUTE(release);
      goto ERROR;
    }
  
  if ( (_fapl = H5Pcreate (H5P_FILE_ACCESS)) < 0 ) {
    MED_ERR_(_fid,MED_ERR_CREATE,MED_ERR_PROPERTY,MED_ERR_FILEVERSION_MSG);
    goto ERROR;
  }

  /*
   * Cette ligne est censée obliger HDF à ne pas utiliser un modèle interne supérieur à 1.8.z
   * En HDF5-1.10.0p1 cela n'a aucun effet ! 
   * Un test autoconf permet de fixer un intervalle de version HDF à MED.
   */
#if H5_VERS_MINOR > 10
#error "Don't forget to change the compatibility version of the library !"
#endif
   
  if ( H5Pset_libver_bounds( _fapl, H5F_LIBVER_18, H5F_LIBVER_18 ) ) {
    MED_ERR_(_fid,MED_ERR_INIT,MED_ERR_PROPERTY,MED_ERR_FILEVERSION_MSG);
    goto ERROR;
  }

  if ((_fid = H5Fcreate(filename,H5F_ACC_TRUNC,
  			H5P_DEFAULT,_fapl)) < 0) {
    MED_ERR_(_fid,MED_ERR_CREATE,MED_ERR_FILE,filename);
    goto ERROR;
  }

/*   if ((_fid = H5Fcreate(filename,H5F_ACC_TRUNC, */
/* 			H5P_DEFAULT,H5P_DEFAULT)) < 0) { */
/*     MED_ERR_(_fid,MED_ERR_CREATE,MED_ERR_FILE,filename); */
/*     goto ERROR; */
/*   } */

  if ( H5Pclose(_fapl) < 0 ) {
    MED_ERR_(_fid,MED_ERR_CLOSE,MED_ERR_PROPERTY,"");
    _fid=-1;goto ERROR;
  }

/* Adjust the size of metadata cache */
/* config.version = H5AC__CURR_CACHE_CONFIG_VERSION; */
/* H5Fget_mdc_config(_fid, &config); */
/* config.set_initial_size = 1; */
/* config.initial_size = 8*1024*1024; */
/* config.max_size = 16*1024*1024; */
/* H5Fset_mdc_config(_fid, &config); */

  _MEDsetModeAcces(_fid,MED_ACC_RDWR);

  if ((_gid = _MEDdatagroupCreer(_fid,MED_INFOS)) < 0) {
    MED_ERR_(_fid,MED_ERR_CREATE,MED_ERR_FILE,MED_INFOS);
    goto ERROR;
  }

  /* Numero de versions de MED */
  if ( _MEDattributeIntWr(_gid,MED_NOM_MAJEUR,&_major)) {
    MED_ERR_(_fid,MED_ERR_WRITE,MED_ERR_ATTRIBUTE,MED_NOM_MAJEUR);
    goto ERROR;
  }

  if ( _MEDattributeIntWr(_gid,MED_NOM_MINEUR,&_minor) < 0) {
    MED_ERR_(_fid,MED_ERR_WRITE,MED_ERR_ATTRIBUTE,MED_NOM_MINEUR);
    goto ERROR;
  }

  if (_MEDattributeIntWr(_gid,MED_NOM_RELEASE,&_release) < 0) {
    MED_ERR_(_fid,MED_ERR_WRITE,MED_ERR_ATTRIBUTE,MED_NOM_RELEASE);
    goto ERROR;
  }

  /* On ferme tout */
  if ( _MEDdatagroupFermer(_gid) < 0) {
    MED_ERR_(_fid,MED_ERR_CLOSE,MED_ERR_DATAGROUP,MED_INFOS);
    goto ERROR;
  }

  _MEDsetModeAcces(_fid,accessmode);
  
  _MEDfileVersion(_fid);

 SORTIE:
 ERROR:
  if (_h518medfileMMN) free(_h518medfileMMN);
  
  return _fid;
  
}
