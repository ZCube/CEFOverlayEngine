SET(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)
SET(BUILD_STATIC_LIBS ON CACHE STRING "" FORCE)

include_directories(
	${CMAKE_BUILD_DIR}/external/zlib
	zlib
)

ADD_SUBDIRECTORY(zlib)
ADD_SUBDIRECTORY(boost)

# for /MT /MD setting....
# global setting is not working.
# TODO : check.
# ADD_SUBDIRECTORY(jsoncpp)
include_directories(
	jsoncpp/include
)

ADD_LIBRARY(jsoncpp_lib_static STATIC
	jsoncpp/src/lib_json/json_tool.h
	jsoncpp/src/lib_json/json_reader.cpp
	jsoncpp/src/lib_json/json_valueiterator.inl
	jsoncpp/src/lib_json/json_value.cpp
	jsoncpp/src/lib_json/json_writer.cpp
)

set_target_properties (zlib PROPERTIES FOLDER external/zlib)
set_target_properties (zlibstatic PROPERTIES FOLDER external/zlib)
set_target_properties (example PROPERTIES FOLDER external/zlib)
set_target_properties (minigzip PROPERTIES FOLDER external/zlib)
set_target_properties (boost PROPERTIES FOLDER external)
set_target_properties (jsoncpp_lib_static PROPERTIES FOLDER external)
