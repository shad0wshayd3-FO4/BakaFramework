cmake_minimum_required(VERSION 3.20)

project(
	SteamworksSDK
	VERSION 1.34
	LANGUAGES CXX
)

include(GNUInstallDirs)

file(GLOB HEADERS public/steam/*.h)

add_library(${PROJECT_NAME} INTERFACE ${HEADERS})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(LINK_LIB_FILE "sdkencryptedappticket.lib")
		set(LINK_LIB_FULL "public/steam/lib/win32/${LINK_LIB_FILE}")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(LINK_LIB_FILE "sdkencryptedappticket64.lib")
		set(LINK_LIB_FULL "public/steam/lib/win64/${LINK_LIB_FILE}")
	endif()
endif()

target_link_libraries(
	${PROJECT_NAME}
	INTERFACE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LINK_LIB_FULL}>"
		"$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_LIBDIR}/${LINK_LIB_FILE}>"
)

install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}-targets
)

configure_file(
	config.cmake.in
	${PROJECT_NAME}Config.cmake
	@ONLY
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
	EXPORT ${PROJECT_NAME}-targets
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
	FILES ${HEADERS}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/steam"
)

install(
	FILES "${LINK_LIB_FULL}"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
