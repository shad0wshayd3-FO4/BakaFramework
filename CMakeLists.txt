cmake_minimum_required(VERSION 3.22)

project(
	BakaFramework
	VERSION 1.5.1
	LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

macro(set_from_environment VARIABLE)
	if(NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif()
endmacro()

set_from_environment(Fallout4Path)
if(NOT DEFINED Fallout4Path)
	message(FATAL_ERROR "Fallout4Path is not set")
endif()

option(COPY_OUTPUT "copy the output of build operations to the game directory" ON)

add_subdirectory(src)
