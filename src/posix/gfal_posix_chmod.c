/*
 * Copyright (c) Members of the EGEE Collaboration. 2004.
 * See http://www.eu-egee.org/partners/ for details on the copyright holders.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*
 * @file gfal_posix_chmod.c
 * @brief file for the internal function of the POSIX chmod function
 * @author Devresse Adrien
 * @version 2.0
 * @date 10/05/2011
 * 
 **/
 
 #include <stdio.h>
 #include <errno.h>
 #include <glib.h>
 #include "../common/gfal_prototypes.h"
 #include "gfal_posix_local_file.h"
 #include "gfal_posix_internal.h"
 #include  "../common/gfal_common_plugin.h"
 
 
 int gfal_posix_internal_chmod(const char* path, mode_t mode){
	 GError* tmp_err=NULL;
	 gfal_handle handle;
	 int res= -1;

	if((handle = gfal_posix_instance()) == NULL){
		errno = EIO;
		return -1;
	}
	
	if(path == NULL){
		g_set_error(&tmp_err, 0, EFAULT, " path is an incorrect argument");
	}else{
		if( gfal_check_local_url(path, NULL) == TRUE){
			res = gfal_local_chmod(path, mode, &tmp_err);
		}else{
			res = gfal_plugin_chmodG(handle, path, mode, &tmp_err);
		}
	}
	if(tmp_err){
		gfal_posix_register_internal_error(handle, "[gfal_chmod]", tmp_err);
		errno = tmp_err->code;	
	}
	return res; 
	 
 }
 
 
 