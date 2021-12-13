set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(SCRIPT "${ROOT_DIR}/scripts/papyrus_cmake.py")

source_group(
	TREE "${ROOT_DIR}"
	FILES "${SCRIPT}"
)

macro(papyrus_target TARGET)
	set(_PREFIX "papyrus_target")
	cmake_parse_arguments(${_PREFIX} "" "TARGET_DEPENDS" "DEPENDS;OUTPUT" ${ARGN})

	if(DEFINED ${_PREFIX}_UNPARSED_ARGUMENTS)
		foreach(_ARG ${${_PREFIX}_UNPARSED_ARGUMENTS})
			message(WARNING "Unused argument: ${_ARG}")
		endforeach()
	endif()

	set(_DUMMY "${CMAKE_CURRENT_BINARY_DIR}/papyrus_${TARGET}_ready")

	add_custom_target(
		"papyrus_${TARGET}"
		ALL
		DEPENDS
			"${_DUMMY}"
		SOURCES
			"${SCRIPT}"
	)

	add_custom_command(
		OUTPUT
			"${_DUMMY}"
			${${_PREFIX}_OUTPUT}
		COMMAND
			"$<TARGET_FILE:Python3::Interpreter>"
			"${SCRIPT}"
			"--bin-dir=${CMAKE_CURRENT_BINARY_DIR}"
			"--sse-dir=${Skyrim64Path}"
			"--src-dir=${ROOT_DIR}"
			"--target=${TARGET}"
		COMMAND
			"${CMAKE_COMMAND}" -E touch "${_DUMMY}"
		DEPENDS
			${${_PREFIX}_DEPENDS}
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		VERBATIM
	)

	if(DEFINED ${_PREFIX}_TARGET_DEPENDS)
		add_dependencies("papyrus_${TARGET}" "papyrus_${${_PREFIX}_TARGET_DEPENDS}")
	endif()

	unset(_DUMMY)
	unset(${_PREFIX}_TARGET_DEPENDS)
	unset(${_PREFIX}_DEPENDS)
	unset(${_PREFIX}_OUTPUT)
	unset(_PREFIX)
endmacro()

cmake_path(SET VANILLA_ARCHIVE NORMALIZE "${Skyrim64Path}/Data/Scripts.zip")
papyrus_target(
	"copy_vanilla"
	DEPENDS
		"${VANILLA_ARCHIVE}"
)

set(SOURCE_FILES
	"placeholder"
)

set(PAPYRUS_COMPILE_IN ${SOURCE_FILES})
list(TRANSFORM PAPYRUS_COMPILE_IN PREPEND "${ROOT_DIR}/src/papyrus/")
list(TRANSFORM PAPYRUS_COMPILE_IN APPEND ".psc")

set(PAPYRUS_COMPILE_OUT ${SOURCE_FILES})
list(TRANSFORM PAPYRUS_COMPILE_OUT PREPEND "${CMAKE_CURRENT_BINARY_DIR}/bin/")
list(TRANSFORM PAPYRUS_COMPILE_OUT APPEND ".pex")

papyrus_target(
	"compile"
	TARGET_DEPENDS "copy_vanilla"
	DEPENDS
		${PAPYRUS_COMPILE_IN}
	OUTPUT
		${PAPYRUS_COMPILE_OUT}
)

set(PAPYRUS_PSC_FILES ${PAPYRUS_COMPILE_IN} PARENT_SCOPE)
set(PAPYRUS_PEX_FILES ${PAPYRUS_COMPILE_OUT} PARENT_SCOPE)

if("${COPY_OUTPUT}")
	foreach(FILE IN LISTS PAPYRUS_COMPILE_OUT)
		add_custom_command(
			TARGET "papyrus_compile"
			POST_BUILD
			COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${FILE}" "${Skyrim64Path}/Data/Scripts/"
			VERBATIM
		)
	endforeach()

	foreach(FILE IN LISTS PAPYRUS_COMPILE_IN)
		add_custom_command(
			TARGET "papyrus_compile"
			POST_BUILD
			COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${FILE}" "${Skyrim64Path}/Data/Source/Scripts/"
			VERBATIM
		)
	endforeach()
endif()
