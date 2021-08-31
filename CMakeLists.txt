cmake_minimum_required(VERSION 3.21)

# Options

option(COPY_BUILD "Copy the build output to the Fallout 4 directory." ON)

# Cache build vars

macro(set_from_environment VARIABLE)
	if(NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif()
endmacro()

set_from_environment(Fallout4Path)

# Project

project(
	BakaFramework
	VERSION 1.4.0
	LANGUAGES CXX
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/version.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# Include guards

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

# Dependencies

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(Boost_USE_STATIC_LIBS ON)

add_subdirectory("extern/CommonLibF4/CommonLibF4" CommonLibF4)

find_package(AutoTOML REQUIRED CONFIG)
find_package(fmt REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(SteamworksSDK REQUIRED CONFIG)
find_package(tomlplusplus REQUIRED CONFIG)
find_package(xbyak REQUIRED CONFIG)

# Create Library

include(cmake/sourcelist.cmake)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES ${SOURCES}
)

source_group(
	TREE "${CMAKE_CURRENT_BINARY_DIR}"
	FILES "${CMAKE_CURRENT_BINARY_DIR}/include/version.h"
)

add_library(
	"${PROJECT_NAME}"
	SHARED
	${SOURCES}
	"${CMAKE_CURRENT_BINARY_DIR}/include/version.h"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	res/settings.toml
	.clang-format
	.editorconfig
)

target_compile_definitions(
	CommonLibF4
	PUBLIC
		F4SE_SUPPORT_XBYAK
)

target_compile_definitions(
	"${PROJECT_NAME}"
	PUBLIC
		TOML_WINDOWS_COMPAT=0
		F4SE_SUPPORT_XBYAK
)

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
		cxx_std_20
)

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		AutoTOML::AutoTOML
		CommonLibF4::CommonLibF4
		fmt::fmt
		spdlog::spdlog
		SteamworksSDK::SteamworksSDK
		tomlplusplus::tomlplusplus
		xbyak::xbyak
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		"src/PCH.h"
)

if(MSVC)
	target_compile_options(
		"${PROJECT_NAME}"
		PRIVATE
			"/sdl"	# Enable Additional Security Checks
			"/utf-8"	# Set Source and Executable character sets to UTF-8
			"/Zi"	# Debug Information Format

			"/permissive-"	# Standards conformance
			"/Zc:preprocessor"	# Enable preprocessor conformance mode

			# disable warnings
			"/wd4324" # 'struct_name' : structure was padded due to __declspec(align())

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		"${PROJECT_NAME}"
		PRIVATE
			/WX	# Treat Linker Warnings as Errors

			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif()

# File copying

if(COPY_BUILD)
	if(DEFINED Fallout4Path)
		add_custom_command(
			TARGET "${PROJECT_NAME}"
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${Fallout4Path}/Data/F4SE/Plugins/"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${Fallout4Path}/Data/F4SE/Plugins/"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/res/settings.toml" "${Fallout4Path}/Data/F4SE/Plugins/${PROJECT_NAME}.toml"
		)
	else()
		message(
			WARNING
			"Variable Fallout4Path is not defined. Skipping post-build copy command."
		)
	endif()
endif()