cmake_minimum_required(VERSION 3.22)
set(NAME "BakaFramework")
set(VERSION 1.10.0)

# ---- Options ----

option(COPY_BUILD "Copy the build output to the Fallout 4 directory." OFF)

# ---- Cache build vars ----

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif()
endmacro()

macro(find_commonlib_path)
	if (CommonLibName AND NOT ${CommonLibName} STREQUAL "")
		# Check extern
		find_path(CommonLibPath
			NAMES
				include/REL/Relocation.h
			PATHS
				extern/${CommonLibName}
				extern/${CommonLibName}/${CommonLibName}
		)
		if (${CommonLibName} STREQUAL "CommonLibName-NOTFOUND")
			#Check path
			set_from_environment(${CommonLibName}Path)
			set(CommonLibName ${${CommonLibName}Path})
		endif()
	endif()
endmacro()

set_from_environment(VCPKG_ROOT)
if (NOT DEFINED VCPKG_ROOT)
	message(
		WARNING
		"Variable VCPKG_ROOT is not set. Continuing without vcpkg."
	)
endif()

set_from_environment(Fallout4Path)
set(CommonLibName "CommonLibF4")
find_commonlib_path("CommonLibF4")

message(
	STATUS
	"Building for Fallout 4 at ${Fallout4Path} with ${CommonLibName} at ${CommonLibPath}."
)

set(Boost_USE_STATIC_RUNTIME OFF CACHE BOOL "")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "")

# ---- Project ----

project(
	${NAME}
	VERSION ${VERSION}
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif()

# ---- Subdirectories ----

add_subdirectory(src)
