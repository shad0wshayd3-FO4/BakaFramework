find_package(
	Python3
	3.9
	MODULE
	REQUIRED
	COMPONENTS
		Interpreter
)

add_subdirectory(c++)
#add_subdirectory(papyrus)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(SCRIPT "${ROOT_DIR}/scripts/archive_artifacts.py")

source_group(
	TREE "${ROOT_DIR}"
	FILES "${SCRIPT}"
)

add_custom_target(
	"archive_artifacts"
	COMMAND
		"$<TARGET_FILE:Python3::Interpreter>"
		"${SCRIPT}"
		"--bin-dir=${CMAKE_CURRENT_BINARY_DIR}"
		#"--papyrus-pex-files=${PAPYRUS_PEX_FILES}"
		#"--papyrus-psc-files=${PAPYRUS_PSC_FILES}"
		"--plugin-files=$<TARGET_FILE:${PROJECT_NAME}>"
		"--pdbs=$<TARGET_PDB_FILE:${PROJECT_NAME}>"
		"--project=${PROJECT_NAME}"
	DEPENDS
		"${PROJECT_NAME}"
		#"papyrus_compile"
	BYPRODUCTS
		"${CMAKE_CURRENT_BINARY_DIR}/artifacts/${PROJECT_NAME}.zip"
		"${CMAKE_CURRENT_BINARY_DIR}/artifacts/${PROJECT_NAME}_pdbs.zip"
	VERBATIM
	SOURCES
		"${SCRIPT}"
)
