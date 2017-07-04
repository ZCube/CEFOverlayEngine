cmake_minimum_required(VERSION 3.0.0)

project(CEFOverlayEngine C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

ADD_DEFINITIONS(
  -DBOOST_ALL_NO_LIB
  -D_CRT_SECURE_NO_WARNINGS
  -D_WIN32_WINNT=0x0601
  -DWIN32_LEAN_AND_MEAN
  -DUNICODE
  -D_UNICODE
  -DNOMINMAX
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(address_model "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(address_model "32")
endif()

include_directories(
	${CMAKE_BINARY_DIR}/external/zlib
	zlib
)

ADD_SUBDIRECTORY(prebuilt)

LINK_DIRECTORIES(
	prebuilt/cef/${address_model}/
	prebuilt/${address_model}
	prebuilt/${address_model}/third_party
	prebuilt/${address_model}/third_party/icu
)
ADD_SUBDIRECTORY(external)
ADD_SUBDIRECTORY(src)

IF(MSVC)
  OPTION(USE_STATIC_RUNTIME_LIBRARY "Use Static Runtime Library" ON)
  OPTION(USE_LINK_TIME_OPTIMIZATION "Use Link Time Optimization" ON)

  IF(USE_STATIC_RUNTIME_LIBRARY)
      message(STATUS "- MSVC: Use Static Runtime Library")
  ENDIF(USE_STATIC_RUNTIME_LIBRARY)
  foreach (flag_var
           CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
           CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
           CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
           CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)

    set(_PARAM "")
    IF(USE_STATIC_RUNTIME_LIBRARY)
      string(FIND "${${flag_var}}" "/MT" _TESTVAR)
      if(${_TESTVAR} EQUAL "-1")
        string(FIND "${${flag_var}}" "/MD" _TESTVAR)
        if(${_TESTVAR} EQUAL "-1")
          set(_PARAM "${_PARAM} /MT ")
        else()
          string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        ENDIF()
      ENDIF()
    ENDIF()
		
    string(FIND "${${flag_var}}" "/MP" _TESTVAR)
    if(${_TESTVAR} EQUAL "-1")
      set(_PARAM "${_PARAM} /MP")
    endif()
    if(_PARAM)
      set(${flag_var} "${${flag_var}} ${_PARAM}" CACHE STRING "" FORCE)
    ENDIF()
    
  endforeach()

  IF(USE_LINK_TIME_OPTIMIZATION)
      foreach (flag_var
               CMAKE_EXE_LINKER_FLAGS_RELEASE
               CMAKE_EXE_LINKER_FLAGS_MINSIZEREL CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
               CMAKE_SHARED_LINKER_FLAGS_RELEASE
               CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
               CMAKE_MODULE_LINKER_FLAGS_RELEASE
               CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO)
        string(FIND "${${flag_var}}" "/LTCG" _TESTVAR)
        if(${_TESTVAR} EQUAL "-1")
          set(${flag_var} "${${flag_var}} /LTCG")
        endif()
      endforeach()
      foreach (flag_var
               CMAKE_EXE_LINKER_FLAGS
               CMAKE_SHARED_LINKER_FLAGS
               CMAKE_MODULE_LINKER_FLAGS
               CMAKE_EXE_LINKER_FLAGS_DEBUG 
               CMAKE_SHARED_LINKER_FLAGS_DEBUG 
               CMAKE_MODULE_LINKER_FLAGS_DEBUG)
        string(FIND "${${flag_var}}" "/LTCG" _TESTVAR)
        if(NOT ${_TESTVAR} EQUAL "-1")
          string(REPLACE "/LTCG" " " temp "${${flag_var}}")
          set(${flag_var} "${temp}")
        endif()
        string(FIND "${${flag_var}}" "/INCREMENTAL" _TESTVAR)
        if(${_TESTVAR} EQUAL "-1")
          set(${flag_var} "${${flag_var}} /INCREMENTAL")
        endif()
      endforeach()
      foreach (flag_var
               CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_RELEASE
               CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
               CMAKE_C_FLAGS CMAKE_C_FLAGS_RELEASE
               CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
        string(REPLACE "${${flag_var}}" "${${flag_var}} /GL" ${flag_var} "${${flag_var}}")
      endforeach()
  ELSEIF(USE_LINK_TIME_OPTIMIZATION)
      foreach (flag_var
               CMAKE_EXE_LINKER_FLAGS_RELEASE
               CMAKE_EXE_LINKER_FLAGS_MINSIZEREL CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
               CMAKE_SHARED_LINKER_FLAGS_RELEASE
               CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
               CMAKE_MODULE_LINKER_FLAGS_RELEASE
               CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO
               CMAKE_EXE_LINKER_FLAGS
               CMAKE_SHARED_LINKER_FLAGS
               CMAKE_MODULE_LINKER_FLAGS
               CMAKE_EXE_LINKER_FLAGS_DEBUG 
               CMAKE_SHARED_LINKER_FLAGS_DEBUG 
               CMAKE_MODULE_LINKER_FLAGS_DEBUG)
        string(FIND "${${flag_var}}" "/INCREMENTAL" _TESTVAR)
        if(${_TESTVAR} EQUAL "-1")
          set(${flag_var} "${${flag_var}} /INCREMENTAL")
        endif()
      endforeach()
  ENDIF(USE_LINK_TIME_OPTIMIZATION)
  foreach (flag_var
           CMAKE_EXE_LINKER_FLAGS_RELEASE
           CMAKE_EXE_LINKER_FLAGS_MINSIZEREL CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
           CMAKE_SHARED_LINKER_FLAGS_RELEASE
           CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
           CMAKE_MODULE_LINKER_FLAGS_RELEASE
           CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO
           CMAKE_EXE_LINKER_FLAGS
           CMAKE_SHARED_LINKER_FLAGS
           CMAKE_MODULE_LINKER_FLAGS
           CMAKE_EXE_LINKER_FLAGS_DEBUG 
           CMAKE_SHARED_LINKER_FLAGS_DEBUG 
           CMAKE_MODULE_LINKER_FLAGS_DEBUG)
    set(${flag_var} "${${flag_var}}" CACHE STRING "" FORCE)
  endforeach()
ENDIF()
 
