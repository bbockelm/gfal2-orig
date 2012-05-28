#pragma once
#ifndef _GFAL_PLUGIN_INTERFACE_
#define _GFAL_PLUGIN_INTERFACE_
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


/**
 * @file plugins related API 
 * @author Devresse Adrien
 * 
 * */

#include <glib.h>
#include <common/gfal_types.h>
#include <common/gfal_constants.h>

#ifdef __cplusplus
extern "C"
{
#endif 

/**
 * Prototype of the plugins entry point
 * @param handle : gfal_handle of the current call
 * @param err : Error report in case of fatal error while the plugin load.
 * 
 * */
typedef gfal_plugin_interface (*gfal_plugin_init_t)(gfal_handle handle, GError** err);


/**
 * @struct _gfal_plugin_interface 
 * 
 *  Main interface that MUST be returned the entry point function "gfal_plugin_init" of each GFAL 2.0 plugin.
 *  the minimum calls are : getName, plugin_delete, check_plugin_url
 *  all the unused function pointers must be set to NULL
 */
struct _gfal_plugin_interface{
	 //! @cond
	 // internal gfal data : touching this causes the death of a little cat
	void * gfal_data;
	//! @endcond
	
	/**
	 *  plugin reserved pointer, free to use for plugin's personnal datas
	 * 
	 * */
	plugin_handle plugin_data;
	/**
	 *  MANDATORY : return a the string id of the plugin.
	 *  string id must be short, constant and unique ( ex : "plugin_gridftp" )
	 *  @return the constant identity string
	 * */
	const char* (*getName)();
	/**
	 * OPTIONAL : Last call before the unload of the plugin for the associated handle
	 *        You can use this to clear your internal context
	 * @param plugin_data : internal plugin data
	 */
	void (*plugin_delete)(plugin_handle plugin_data); 
	/**
	 *  MANDATORY: Main critical function of the plugins, URL checker.
	 *  check the availability of the given operation on this plugin for the given URL
	 *  
	 *  This function is used by gfal 2.0 to determine which plugin need to be contacted for a given operation
	 *  
	 *   
	 *  @warning This function is a key function of GFAL 2.0, It MUST be as fast as possible.
	 *  @param plugin_data : internal plugin data
	 *  @param url : URL to check for the protocol compatibility
	 *  @param operation :  operation to check
	 *  @param err : error handle, should be used ONLY in case of major failure.
	 *  @return must return TRUE if the operation is compatible with this plugin, else FALSE
	 */
	gboolean (*check_plugin_url)(plugin_handle plugin_data, const char* url,  plugin_mode operation, GError** err);
	 
	/**
	 *  OPTIONAL : gfal_access function  support
	 *  @param plugin_data : internal plugin data
	 *  @param url : URL for access checking
	 *  @param mode : mode to check ( see man 2 access )
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 or -1 if error occures,
	 *          err MUST be set in case of error		 
	 * */
	int (*accessG)(plugin_handle plugin_data, const char* url, int mode, GError** err);

	/**
	 *  OPTIONAL : gfal_chmod function  support
	 *  @param plugin_data : internal plugin data
	 *  @param url : URL of the file 
	 *  @param mode : mode to set 
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 or -1 if error occures,
	 *          err MUST be set in case of error		 
	 * */
	int	(*chmodG)(plugin_handle plugin_data, const char * url, mode_t mode, GError** err);
	/**
	 *  OPTIONAL : gfal_rename function  support
	 *  @param plugin_data : internal plugin data
	 *  @param oldurl : old url of the file
	 *  @param urlnew : new url of the file
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 or -1 if error occures,
	 *          err MUST be set in case of error		 
	 * */	
	int	(*renameG)(plugin_handle plugin_data, const char * oldurl, const char * urlnew, GError** err);
	/**
	 *  OPTIONAL : gfal_symlink function  support
	 *  @param plugin_data : internal plugin data
	 *  @param oldurl : old url of the file
	 *  @param urlnew : symlink to create
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 or -1 if error occures,
	 *          err MUST be set in case of error	
	 * */	
	int (*symlinkG)(plugin_handle plugin_data, const char* oldurl, const char* newold, GError** err);
	
	/**
	 *  MANDATORY : gfal_stat function  support
	 *  @param plugin_data : internal plugin data
	 *  @param url : url to stat
	 *  @param buf : informations of the file
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 or -1 if error occures,
	 *          err MUST be set in case of error	  
	 * */		
	int (*statG)(plugin_handle plugin_data , const char* url, struct stat *buf, GError** err);
	
	/**
	 *  OPTIONAL : gfal_lstat function  support
	 * 			In case of non support for this function, calls to @ref gfal_lstat are mapped to @ref gfal_stat.
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url to stat
	 *  @param buf : informations of the file
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 or -1 if error occures,
	 *          err MUST be set in case of error	  
	 * */		
	int (*lstatG)(plugin_handle plugin_data, const char* url, struct stat *buf, GError** err);
	/**
	 *  OPTIONAL : gfal_readlink function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url to stat
	 *  @param buff : buffer for the readlink result
	 *  @param size_t : buffsize maximum size to fill in the buffer
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return number of bytes in buff in case of success or -1 if error occures,
	 *          err MUST be set in case of error
	 * */	
	ssize_t (*readlinkG)(plugin_handle plugin_data, const char* url, char* buff, size_t buffsiz, GError** );

	/**
	 *  OPTIONAL : gfal_opendir function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url of directory to list
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return gfa_file_handle in case of success or NULL if error,
	 *          err MUST be set in case of error
	 * */		
	 gfal_file_handle (*opendirG)(plugin_handle plugin_data, const char* url, GError** err); 
	
	/**
	 *  MANDATORY IF OPENDIR : gfal_closedir function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param dir_desc : directory descriptor to use
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 in case of success or -1 if error,
	 *          err MUST be set in case of error
	 * */		 
	 int (*closedirG)(plugin_handle plugin_data, gfal_file_handle dir_desc, GError** err);
	 
	/**
	 *  MANDATORY IF OPENDIR : gfal_readdir function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param dir_desc : directory descriptor to use
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return dirent information in case of success or NULL if end of listing or error,
	 *          err MUST be set in case of error
	 * */	
	struct dirent* (*readdirG)(plugin_handle plugin_data, gfal_file_handle dir_desc, GError** err);
	
	/**
	 *  OPTIONAL : gfal_mkdir function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url of the directory to create
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 in case of success or -1 if error occures,
	 *          err MUST be set in case of error
	 * */	
	int (*mkdirpG)(plugin_handle plugin_data, const char* url, mode_t, gboolean pflag, GError** err);  //function pointer for the mkdir call, if pflag is set, the call is considered like a recursive call for a full path creation
	/**
	 *  OPTIONAL : gfal_rmdir function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url of the directory to delete
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 in case of success or -1 if error occures,
	 *          err MUST be set in case of error
	 * */	
	int (*rmdirG)(plugin_handle plugin_data, const char* url, GError** err);	 	 

	 // basic file operations
	/**
	 *  OPTIONAL : gfal_open function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url of the directory to open
	 *  @param flag : open flags
	 *  @param mode : mode of the file, in case of creation
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return gfal_file_handle in case of success or NULL if error occures,
	 *          err MUST be set in case of error
	 * */		 
	 gfal_file_handle (*openG)(plugin_handle plugin_data, const char* url, int flag, mode_t mode, GError**);
	 
	/**
	 *  MANDATORY IF OPEN : gfal_read function  support
	 * 			
	 * */		 
	 int (*readG)(plugin_handle, gfal_file_handle fd, void* buff, size_t count, GError**);
	
	/**
	 *  MANDATORY IF OPEN : gfal_write function  support
	 * 			
	 * */		 
	 int (*writeG)(plugin_handle, gfal_file_handle fd, const void* buff, size_t count, GError**);
	
	/**
	 *  MANDATORY IF OPEN : gfal_close function  support
	 * 			
	 * */		 
	 int (*closeG)(plugin_handle, gfal_file_handle fd, GError **);
	
	/**
	 *  MANDATORY IF OPEN : gfal_lseek function  support
	 * 			
	 * */	 
	 int (*lseekG)(plugin_handle, gfal_file_handle fd, off_t offset, int whence, GError** err);
	 
	 // vector operations
	/**
	 *  OPTIONAL : gfal_pread function  support
	 * 
	 *  Allow fast parallels read support, If not implemented, this function is simulated by GFAL 2.0
	 * 			
	 * */		 
	 ssize_t (*preadG)(plugin_handle, gfal_file_handle fd, void* buff, size_t count, off_t offset, GError**);	
	/**
	 *  OPTIONAL : gfal_pwriteG function  support
	 * 
	 *  Allow fast parallels write support, If not implemented, this function is simulated by GFAL 2.0
	 * 			
	 * */		 
	 ssize_t (*pwriteG)(plugin_handle, gfal_file_handle fd, const void* buff, size_t count, off_t offset, GError**);
	 
	 	 
	 // remove operations
	/**
	 *  OPTIONAL : gfal_unlink function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url of the file to delete
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 in case of success or -1 if error occures,
	 *          err MUST be set in case of error
	 * */
	 int (*unlinkG)(plugin_handle plugin_data, const char* url, GError** err);
	 
	 // advanced attributes management
	/**
	 *  OPTIONAL : gfal_getxattr function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url of the file to delete
	 *  @param key : key of the attribute to get
	 *  @param buff : buffer for the attribute content
	 *  @param s_buff : maximum buffer size
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return size of the attribute in case of success or -1 if error occures,
	 *          err MUST be set in case of error
	 * */
	 ssize_t (*getxattrG)(plugin_handle plugin_data, const char* url, const char* key, 
							void* buff, size_t s_buff, GError** err);
	/**
	 *  OPTIONAL : gfal_listxattr function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url of the file to delete
	 *  @param list : buffer for the list attribute content
	 *  @param s_buff : maximum buffer size
	 *  @param s_list : Error report, the code field of err should be set to errno value when possible
	 *  @return size of the list in case of success or -1 if error occures,
	 *          err MUST be set in case of error
	 * */
	 ssize_t (*listxattrG)(plugin_handle plugin_data, const char* url, char* list, size_t s_list, GError** err);
	
	/**
	 *  OPTIONAL : gfal_setxattr function  support
	 * 			
	 *  @param plugin_data : internal plugin data
	 *  @param url : url of the file to delete
	 *  @param key : key of the attribute to set
	 *  @param buff : buffer for the attribute content
	 *  @param s_buff : maximum buffer size
	 *  @param flas : set/get flags
	 *  @param err : Error report, the code field of err should be set to errno value when possible
	 *  @return 0 or -1 if error occures,
	 *          err MUST be set in case of error
	 * */
	 int (*setxattrG)(plugin_handle plugin_data, const char* url, const char* key,
					const void* buff , size_t s_buff, int flags, GError** err);


	 /**
	  *  this function is called to inform plugin of a new parameter
	  *  must return 1 if this parameter is used by the plugin else return 0
	  *  @warning must be a NULL pointer if not used
	  * 
	  * */
	 int (*is_used_parameter)(plugin_handle, const char* namespacep, const char* key);

	 /**
	  *  called when a change occures on a parameter
	  *  push the information to the plugins
	  *  to check the new value associated with the key, call gfal_get_*_parameter functions
	  *  @warning must be a NULL pointer if not used
	  *  @return 0 if parameter changed or if no parameter triggered, -1 if major error with the new parameter
	  **/
	 void (*notify_change_parameter)(plugin_handle, const char* namespacep, const char* key);
	 
	 // reserved for future usage
	 //! @cond
	 void* future[25];
	 //! @endcond
};

//! @cond
struct _plugin_opts{
	gfal_plugin_interface plugin_list[MAX_PLUGIN_LIST];
	int plugin_number;
};
//! @endcond


// internal API for inter plugin communication
//! @cond
int gfal_plugins_accessG(gfal_handle handle, const char* path, int mode, GError** err);
int gfal_plugin_rmdirG(gfal_handle handle, const char* path, GError** err);
ssize_t gfal_plugin_readlinkG(gfal_handle handle, const char* path, char* buff, size_t buffsiz, GError** err);




int gfal_plugin_chmodG(gfal_handle handle, const char* path, mode_t mode, GError** err);
int gfal_plugin_statG(gfal_handle handle,const char* path, struct stat* st, GError** err);
int gfal_plugin_renameG(gfal_handle handle, const char* oldpath, const char* newpath, GError** err);
int gfal_plugin_symlinkG(gfal_handle handle, const char* oldpath, const char* newpath, GError** err);
int gfal_plugin_lstatG(gfal_handle handle,const char* path, struct stat* st, GError** err);
int gfal_plugin_mkdirp(gfal_handle handle, const char* path, mode_t mode, gboolean pflag,  GError** err);


gfal_file_handle gfal_plugin_opendirG(gfal_handle handle, const char* name, GError** err);
int gfal_plugin_closedirG(gfal_handle handle, gfal_file_handle fh, GError** err);
struct dirent* gfal_plugin_readdirG(gfal_handle handle, gfal_file_handle fh, GError** err);
 	

gfal_file_handle gfal_plugin_openG(gfal_handle handle, const char * path, int flag, mode_t mode, GError ** err);
int gfal_plugin_closeG(gfal_handle handle, gfal_file_handle fh, GError** err);
int gfal_plugin_writeG(gfal_handle handle, gfal_file_handle fh, void* buff, size_t s_buff, GError** err);
int gfal_plugin_lseekG(gfal_handle handle, gfal_file_handle fh, off_t offset, int whence, GError** err);
int gfal_plugin_readG(gfal_handle handle, gfal_file_handle fh, void* buff, size_t s_buff, GError** err);

ssize_t gfal_plugin_preadG(gfal_handle handle, gfal_file_handle fh, void* buff, size_t s_buff, off_t offset, GError** err);
ssize_t gfal_plugin_pwriteG(gfal_handle handle, gfal_file_handle fh, void* buff, size_t s_buff, off_t offset, GError** err);


int gfal_plugin_unlinkG(gfal_handle handle, const char* path, GError** err);


ssize_t gfal_plugin_getxattrG(gfal_handle, const char*, const char*, void* buff, size_t s_buff, GError** err);
ssize_t gfal_plugin_listxattrG(gfal_handle, const char*, char* list, size_t s_list, GError** err);
int gfal_plugin_setxattrG(gfal_handle, const char*, const char*, const void*, size_t, int, GError**);
//! @endcond

#ifdef __cplusplus
}
#endif 


#endif

