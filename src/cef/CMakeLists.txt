SET(IMGUI_DIRECTORY ${CMAKE_HOME_DIRECTORY}/external/reshade/deps/imgui)
SET(STB_DIRECTORY ${CMAKE_HOME_DIRECTORY}/external/reshade/deps/stb)
SET(CEF_DIRECTORY
${CMAKE_HOME_DIRECTORY}/prebuilt/cef/${address_model}/include
${CMAKE_HOME_DIRECTORY}/prebuilt/cef/${address_model}
)

INCLUDE_DIRECTORIES(
	${IMGUI_DIRECTORY}
	${STB_DIRECTORY}
	${CEF_DIRECTORY}
)


IF(USE_SSL)
	ADD_DEFINITIONS(
		-DUSE_SSL
	)
ENDIF(USE_SSL)

ADD_LIBRARY(CefOverlay
	SHARED
	${IMGUI_DIRECTORY}/imconfig.h
	${IMGUI_DIRECTORY}/imgui.cpp
	${IMGUI_DIRECTORY}/imgui.h
	${IMGUI_DIRECTORY}/imgui_draw.cpp
	${IMGUI_DIRECTORY}/imgui_internal.h
	${IMGUI_DIRECTORY}/stb_rect_pack.h
	${IMGUI_DIRECTORY}/stb_textedit.h
	${IMGUI_DIRECTORY}/stb_truetype.h
	overlay.cpp
	overlay.def
	Resource.rc
	resource.h
	version.h
)

TARGET_LINK_LIBRARIES(CefOverlay
	jsoncpp_lib_static
	boost
	libcef
	libcef_dll_wrapper
)

ADD_EXECUTABLE(CefOverlayProc
	WIN32
	${IMGUI_DIRECTORY}/imconfig.h
	${IMGUI_DIRECTORY}/imgui.cpp
	${IMGUI_DIRECTORY}/imgui.h
	${IMGUI_DIRECTORY}/imgui_draw.cpp
	${IMGUI_DIRECTORY}/imgui_internal.h
	${IMGUI_DIRECTORY}/stb_rect_pack.h
	${IMGUI_DIRECTORY}/stb_textedit.h
	${IMGUI_DIRECTORY}/stb_truetype.h
	overlay_proc.cpp
	Resource.rc
	resource.h
	version.h
)

TARGET_LINK_LIBRARIES(CefOverlayProc
	jsoncpp_lib_static
	boost
	libcef
	libcef_dll_wrapper
)


ADD_LIBRARY(loader
	SHARED
	loader.cpp
	overlay.def
	Resource.rc
	resource.h
	version.h
)

TARGET_LINK_LIBRARIES(loader
)

