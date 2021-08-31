if(NOT DEFINED ENV{STEAMWORKS_SDK})
	message(FATAL_ERROR "environment variable STEAMWORKS_SDK is not set")
endif()

set(MAJOR 1)
set(MINOR 34)

cmake_path(SET STEAMWORKS_SDK NORMALIZE $ENV{STEAMWORKS_SDK}/steamworks_sdk_${MAJOR}${MINOR})
if(NOT IS_DIRECTORY ${STEAMWORKS_SDK})
	message(FATAL_ERROR "failed to find required version in STEAMWORKS_SDK")
endif()

set(SOURCE_PATH ${CURRENT_BUILDTREES_DIR}/src)

file(
	COPY
		${STEAMWORKS_SDK}/sdk/public
		${CMAKE_CURRENT_LIST_DIR}/cmake/CMakeLists.txt
		${CMAKE_CURRENT_LIST_DIR}/cmake/config.cmake.in
	DESTINATION
		${SOURCE_PATH}
)

configure_file(
	${CMAKE_CURRENT_LIST_DIR}/cmake/version.cmake.in
	${SOURCE_PATH}/version.cmake
	@ONLY
)

vcpkg_cmake_configure(SOURCE_PATH ${SOURCE_PATH})
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(
	PACKAGE_NAME SteamworksSDK
	CONFIG_PATH lib/cmake/SteamworksSDK
)

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)

file(INSTALL ${STEAMWORKS_SDK}/sdk/Readme.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)