cmake_minimum_required(VERSION 3.18)

# ---- Project ----

project(
	BakaFramework
	VERSION 1.0.0
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

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/lib/AutoTOML/include/AutoTOML.hpp
	${CMAKE_CURRENT_BINARY_DIR}/include/AutoTOML.hpp
	COPYONLY
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Globals ----

set(MSVC_COMPILE_OPTIONS
	/Gy	# Enable Function-Level Linking
	/MP	# Build with Multiple Processes
	/Oi	# Generate Intrinsic Functions
	/sdl	# Enable Additional Security Checks
	/Zi	# Debug Information Format
	
	/permissive-	# Standards conformance
	
	/Zc:alignedNew	# C++17 over-aligned allocation
	/Zc:auto	# Deduce Variable Type
	/Zc:char8_t
	/Zc:__cplusplus	# Enable updated __cplusplus macro
	/Zc:externC
	/Zc:externConstexpr	# Enable extern constexpr variables
	/Zc:forScope	# Force Conformance in for Loop Scope
	/Zc:hiddenFriend
	/Zc:implicitNoexcept	# Implicit Exception Specifiers
	/Zc:lambda
	/Zc:noexceptTypes	# C++17 noexcept rules
	/Zc:preprocessor	# Enable preprocessor conformance mode
	/Zc:referenceBinding	# Enforce reference binding rules
	/Zc:rvalueCast	# Enforce type conversion rules
	/Zc:sizedDealloc	# Enable Global Sized Deallocation Functions
	/Zc:strictStrings	# Disable string literal type conversion
	/Zc:ternary	# Enforce conditional operator rules
	/Zc:threadSafeInit	# Thread-safe Local Static Initialization
	/Zc:tlsGuards
	/Zc:trigraphs	# Trigraphs Substitution
	/Zc:wchar_t	# wchar_t Is Native Type
	
	/experimental:external
	/external:anglebrackets
	/external:W0
	
	/W4	# Warning level (all warnings)
	/WX	# Warning level (warnings are errors)
)

set(MSVC_COMPILE_OPTIONS_DEBUG
	/Zc:inline-	# Keep unreferenced COMDAT
	/JMC	# Just My Code debugging
)

set(MSVC_COMPILE_OPTIONS_RELEASE
	/Zc:inline	# Remove unreferenced COMDAT
	/JMC-	# Disable Just My Code debugging
)

set(MSVC_LINK_OPTIONS_DEBUG
	/INCREMENTAL	# Link Incrementally
	/OPT:NOREF	# Optimizations (keep functions/data never referenced)
	/OPT:NOICF	# Optimizations (prevent identical COMDAT folding)
)

set(MSVC_LINK_OPTIONS_RELEASE
	/INCREMENTAL:NO	# Link Incrementally
	/OPT:REF	# Optimizations (eliminate functions/data never referenced)
	/OPT:ICF	# Optimizations (perform identical COMDAT folding)
)

add_compile_definitions(
	"$<$<BOOL:${MSVC}>:_UNICODE>"
	TOML_WINDOWS_COMPAT=0
)

add_compile_options(
	"$<$<BOOL:${MSVC}>:${MSVC_COMPILE_OPTIONS}>"
	"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:DEBUG>>:${MSVC_COMPILE_OPTIONS_DEBUG}>"
	"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:RELEASE>>:${MSVC_COMPILE_OPTIONS_RELEASE}>"
)

add_link_options(
	"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:DEBUG>>:${MSVC_LINK_OPTIONS_DEBUG}>"
	"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:RELEASE>>:${MSVC_LINK_OPTIONS_RELEASE}>"
)

# ---- Dependencies ----

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_DEBUG_RUNTIME ON)

add_subdirectory($ENV{CommonLibF4Path} CommonLibF4)

find_package(spdlog REQUIRED CONFIG)
find_package(tomlplusplus REQUIRED CONFIG)

# ---- Add source files ----

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${headers}
		${sources}
)

source_group(
	TREE ${CMAKE_CURRENT_BINARY_DIR}
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/include/AutoTOML.hpp
		${CMAKE_CURRENT_BINARY_DIR}/include/version.h
)

# ---- Create DLL ----

set(MSVC_TARGET_COMPILE_OPTIONS
	/TP	# Specify Source File Type (C++)
)

add_library(BakaFramework SHARED
	${headers}
	${sources}
	${CMAKE_CURRENT_BINARY_DIR}/include/AutoTOML.hpp
	${CMAKE_CURRENT_BINARY_DIR}/include/version.h
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	BakaFramework.toml
)

set_property(
	TARGET
		BakaFramework
		CommonLibF4
	PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_compile_features(BakaFramework
	PUBLIC
		cxx_std_17
)

target_compile_options(BakaFramework
	PRIVATE
		"$<$<BOOL:${MSVC}>:${MSVC_TARGET_COMPILE_OPTIONS}>"
)

target_include_directories(BakaFramework
	PRIVATE 
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(BakaFramework
	PUBLIC
		CommonLibF4
		spdlog::spdlog
		tomlplusplus::tomlplusplus
)

target_link_options(BakaFramework
	PUBLIC
		"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:RELEASE>>:/DEBUG:FULL>"
)

target_precompile_headers(BakaFramework
	PRIVATE
		src/PCH.h
)

# ---- Post build ----

add_custom_command(
	TARGET BakaFramework
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:BakaFramework> $ENV{Fallout4Path}/Data/F4SE/Plugins/
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:BakaFramework> $ENV{Fallout4Path}/Data/F4SE/Plugins/
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/BakaFramework.toml $ENV{Fallout4Path}/Data/F4SE/Plugins/
)