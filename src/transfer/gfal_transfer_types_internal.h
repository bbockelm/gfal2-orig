#pragma once
#ifndef _GFAL2_TRANSFER_TYPES_INTERNAL_
#define _GFAL2_TRANSFER_TYPES_INTERNAL_

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

#include <iostream>
#include <string>


#include <exceptions/gfalcoreexception.hpp>

#include <common/gfal_constants.h>
#include <common/gfal_types.h>
#include <common/gfal_common_errverbose.h>

#include <global/gfal_global.h>

#include <transfer/gfal_transfer.h>
#include <transfer/gfal_transfer_plugins.h>
#include <transfer/gfalmaindecorator.h>


struct _gfalt_params_t{
	gboolean lock; 			// lock enabled after the start of the transfer
	uuid_t uuid;			// unique id of the transfer
	guint64 timeout; 			// connexion timeout
	gboolean replace_existing; // replace destination or not
	off_t start_offset;		// start offset in case of restart
	guint nb_data_streams;	// nb of parallels streams
	gpointer user_data;			// user data information for the monitoring callback
	gfalt_monitor_func callback;
	size_t buffer_size; // internal buffer size per flow for non-third party transfer
};


namespace Gfal{
	
namespace Transfer{


class FileCopy : public MainDecorator {
public:
	FileCopy(PluginFactory* wrap);
	virtual ~FileCopy();
	
	void start_copy(gfalt_params_t p, const std::string & src, const std::string & dst);
	
protected:
	const plugin_filecopy_call find_copy_plugin(const std::string & src, const std::string & dst, void** plugin_data);
	
}; 

} // end Transfer

} //end Gfal



#endif //_GFAL2_TRANSFER_TYPES_INTERNAL_
