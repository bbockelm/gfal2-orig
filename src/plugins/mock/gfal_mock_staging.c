/*
 * Copyright (c) CERN 2013-2015
 *
 * Copyright (c) Members of the EMI Collaboration. 2010-2013
 *  See  http://www.eu-emi.eu/partners for details on the copyright
 *  holders.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gfal_mock_plugin.h"
#include <string.h>


int gfal_plugin_mock_bring_online(plugin_handle plugin_data, const char *url,
    time_t pintime, time_t timeout, char *token, size_t tsize, int async,
    GError **err)
{
    MockPluginData *mdata = plugin_data;
    char arg_buffer[64];

    // Bring online errno
    gfal_plugin_mock_get_value(url, "staging_errno", arg_buffer, sizeof(arg_buffer));
    int staging_errno = gfal_plugin_mock_get_int_from_str(arg_buffer);

    // Polling time
    gfal_plugin_mock_get_value(url, "staging_time", arg_buffer, sizeof(arg_buffer));
    mdata->staging_end = time(NULL) + gfal_plugin_mock_get_int_from_str(arg_buffer);

    // Fake token
    g_strlcpy(token, "mock-token", tsize);

    // Now, if remaining is <= 0, or blocking call, we are done
    if (mdata->staging_end <= time(NULL) || !async) {
        if (staging_errno) {
            gfal_plugin_mock_report_error(strerror(staging_errno), staging_errno, err);
            return -1;
        }
        return 1;
    }

    return 0;
}


int gfal_plugin_mock_bring_online_poll(plugin_handle plugin_data,
    const char *url, const char *token, GError **err)
{
    MockPluginData *mdata = plugin_data;

    char arg_buffer[64];
    gfal_plugin_mock_get_value(url, "staging_errno", arg_buffer, sizeof(arg_buffer));
    int staging_errno = gfal_plugin_mock_get_int_from_str(arg_buffer);

    if (mdata->staging_end <= time(NULL)) {
        if (staging_errno) {
            gfal_plugin_mock_report_error(strerror(staging_errno), staging_errno, err);
            return -1;
        }
        return 1;
    }
    else {
        gfal_plugin_mock_report_error("Not ready", EAGAIN, err);
    }

    return 0;
}


int gfal_plugin_mock_release_file(plugin_handle plugin_data, const char *url,
    const char *token, GError **err)
{
    char arg_buffer[64];
    gfal_plugin_mock_get_value(url, "release_errno", arg_buffer, sizeof(arg_buffer));
    int release_errno = gfal_plugin_mock_get_int_from_str(arg_buffer);

    if (release_errno) {
        gfal_plugin_mock_report_error(strerror(release_errno), release_errno, err);
        return -1;
    }
    return 0;
}


int gfal_plugin_mock_bring_online_list(plugin_handle plugin_data, int nbfiles,
    const char *const *urls, time_t pintime, time_t timeout, char *token,
    size_t tsize, int async, GError **err)
{
    int terminal_count = 0, r, i;

    for (i = 0; i < nbfiles; ++i) {
        r = gfal_plugin_mock_bring_online(plugin_data, urls[i], pintime, timeout, token, tsize, async, &(err[i]));
        if (r > 0)
            ++terminal_count;
    }

    if (terminal_count == nbfiles)
        return 1;
    return 0;
}


int gfal_plugin_mock_bring_online_poll_list(plugin_handle plugin_data,
    int nbfiles, const char *const *urls, const char *token, GError **err)
{
    int terminal_count = 0, r, i, error_count = 0;

    for (i = 0; i < nbfiles; ++i) {
        r = gfal_plugin_mock_bring_online_poll(plugin_data, urls[i], token, &(err[i]));
        if (r > 0) {
            ++terminal_count;
        }
        else if (r < 0) {
            ++terminal_count;
            ++error_count;
        }
    }

    if (terminal_count == nbfiles)
        return 1;
    return 0;
}


int gfal_plugin_mock_release_file_list(plugin_handle plugin_data, int nbfiles,
    const char *const *urls, const char *token, GError **err)
{
    int i;
    for (i = 0; i < nbfiles; ++i) {
        gfal_plugin_mock_release_file(plugin_data, urls[i], token, &(err[i]));
    }
    return 1;
}