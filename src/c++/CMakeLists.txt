set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

set(SOURCE_DIR "${ROOT_DIR}/src/c++")
include("${ROOT_DIR}/cmake/sourcelist.cmake")

source_group(TREE "${SOURCE_DIR}" PREFIX "src" FILES ${SOURCE_FILES})

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h")
configure_file(
	"${ROOT_DIR}/cmake/Plugin.h.in"
	"${VERSION_HEADER}"
	@ONLY
)

source_group("include" FILES "${VERSION_HEADER}")

add_library(
	"${PROJECT_NAME}"
	SHARED
	${SOURCE_FILES}
	"${VERSION_HEADER}"
	"${ROOT_DIR}/res/settings.toml"
	"${ROOT_DIR}/.clang-format"
	"${ROOT_DIR}/.editorconfig"
)

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
		cxx_std_20
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(
		"${PROJECT_NAME}"
		PRIVATE
			"/sdl"	# Enable Additional Security Checks
			"/utf-8"	# Set Source and Executable character sets to UTF-8
			"/Zi"	# Debug Information Format

			"/permissive-"	# Standards conformance
			"/Zc:preprocessor"	# Enable preprocessor conformance mode

			"/wd4200" # nonstandard extension used : zero-sized array in struct/union
			"/wd4828" # The file contains a character that cannot be represented in the current code page (number)

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		"${PROJECT_NAME}"
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif()

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}/include"
		"${INCLUDE_DIR}"
		"${SOURCE_DIR}"
)

set(F4SE_SUPPORT_XBYAK ON CACHE BOOL "")

add_subdirectory("${ROOT_DIR}/external/CommonLibF4/CommonLibF4" CommonLibF4)

find_package(AutoTOML REQUIRED CONFIG)
find_package(fmt REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(SteamworksSDK REQUIRED CONFIG)
find_package(tomlplusplus REQUIRED CONFIG)
find_package(xbyak REQUIRED CONFIG)

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
		"${SOURCE_DIR}/PCH.h"
)

target_compile_definitions(
	"${PROJECT_NAME}"
	PRIVATE
		_UNICODE
		_CRT_SECURE_NO_WARNINGS
)

if("${COPY_OUTPUT}")
	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${Fallout4Path}/Data/F4SE/Plugins/"
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${ROOT_DIR}/res/settings.toml" "${Fallout4Path}/Data/F4SE/Plugins/${PROJECT_NAME}.toml"
		VERBATIM
	)
endif()
