ADD_LIBRARY(loader
	SHARED
	loader.cpp
	../overlay.def
	Resource.rc
	resource.h
	version.h
)

set_target_properties(loader PROPERTIES OUTPUT_NAME "mod_loader_${address_model}")
