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


#ifndef GRIDFTP_STAT_MODULE_H
#define GRIDFTP_STAT_MODULE_H

#include "gridftpmodule.h"


extern "C" int gfal_gridftp_statG(plugin_handle handle, const char* name, struct stat* buff, GError ** err);

extern "C" int gfal_gridftp_accessG(plugin_handle handle, const char* name, int mode, GError** err);

#endif /* GRIDFTP_STAT_MODULE_H */ 