set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

include(${ROOT_DIR}/extern/BethesdaCMakeModules/Modules/Papyrus.cmake)

file(GLOB Papyrus_INPUT
	LIST_DIRECTORIES false
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.psc
)

add_papyrus(
	"Papyrus"
	GAME
		${Fallout4Path}
	MODE
		"Fallout4"
	IMPORTS
		${Fallout4Path}/Data/Scripts/Source/User
		${Fallout4Path}/Data/Scripts/Source/CreationClub
		${Fallout4Path}/Data/Scripts/Source/DLC06
		${Fallout4Path}/Data/Scripts/Source/DLC05
		${Fallout4Path}/Data/Scripts/Source/DLC04
		${Fallout4Path}/Data/Scripts/Source/DLC03
		${Fallout4Path}/Data/Scripts/Source/DLC02
		${Fallout4Path}/Data/Scripts/Source/DLC01
		${Fallout4Path}/Data/Scripts/Source/Base
	SOURCES
		${Papyrus_INPUT}
)

source_group(
	"Scripts"
	FILES
		${Papyrus_INPUT}
)

if (COPY_BUILD)
	if (DEFINED Fallout4Path)
		foreach(FILE IN LISTS Papyrus_INPUT)
			add_custom_command(
				TARGET "Papyrus"
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy ${FILE} ${Fallout4Path}/Data/Scripts/Source/Baka/
			)
		endforeach()

		foreach(FILE IN LISTS Papyrus_OUTPUT)
			add_custom_command(
				TARGET "Papyrus"
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy ${FILE} ${Fallout4Path}/Data/Scripts/
			)
		endforeach()
	else ()
		message(
			WARNING
			"Variable ${Fallout4Path} is not defined. Skipping post-build copy command."
		)
	endif()
endif()
