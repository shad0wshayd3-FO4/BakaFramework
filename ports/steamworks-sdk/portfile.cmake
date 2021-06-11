set(MAJOR 1)
set(MINOR 34)
set(REF "v${MAJOR}.${MINOR}")

set(ARCHIVE "steamworks_sdk_${MAJOR}${MINOR}.zip")
vcpkg_download_distfile(
	ARCHIVE
	URLS "http://partner.steamgames.com/downloads/${ARCHIVE}"
	SHA512 0d5f7cd0509d8e9d40a5aabbb5d010ea3d1d0bf54fe411fa832be579f3fd066ed6b2f621327425589a8bc6f9de86537b24a1c79fee28feca469f8b30d2c3803b
	FILENAME "${ARCHIVE}"
)

vcpkg_extract_source_archive_ex(
	OUT_SOURCE_PATH SOURCE_PATH
	ARCHIVE "${ARCHIVE}"
	REF ${REF}
)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/cmake/CMakeLists.txt DESTINATION ${SOURCE_PATH})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/cmake/config.cmake.in DESTINATION ${SOURCE_PATH})

vcpkg_cmake_configure(SOURCE_PATH ${SOURCE_PATH})
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(
	PACKAGE_NAME SteamworksSDK
	CONFIG_PATH lib/cmake/SteamworksSDK
)

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)

file(INSTALL ${SOURCE_PATH}/Readme.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)