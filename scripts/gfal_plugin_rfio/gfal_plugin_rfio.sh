## configure gfal to load the plugin
##

export LCG_RFIO_TYPE="libdpm.so"
export GFAL_PLUGIN_LIST=libgfal_plugin_rfio.so:$GFAL_PLUGIN_LIST

