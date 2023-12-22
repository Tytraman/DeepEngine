# Indique la version minimum de CMake requise pour exécuter le script.
cmake_minimum_required(VERSION 3.8)

# Indique les versions à utiliser des langages.
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../Build")

enable_language(C ASM_MASM)

if(MSVC)
    #add_compile_options(/fsanitize=address)
	# Enable Hot Reload for MSVC compilers if supported.
	if(POLICY CMP0141)
	  cmake_policy(SET CMP0141 NEW)
	  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
	endif()
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../variables.cmake")

project(${CORE_PROJECT} VERSION 0.0.1)

add_compile_definitions(PRIVATE DE_AUTHORS=${DE_AUTHORS})
add_compile_definitions(PRIVATE DE_VERSION=${DE_VERSION})

set(CORE_SOURCE "${CMAKE_CURRENT_LIST_DIR}/Source" CACHE INTERNAL "")
set(CORE_INCLUDE "${CORE_SOURCE}/Include" CACHE INTERNAL "")

# Variables qui stockent les chemins des différentes librairies utilisées.
set(SDL_INCLUDE "C:/Users/tytra/Documents/libs/SDL2/include")
set(SDL_LIB "C:/Users/tytra/Documents/libs/SDL2/lib/x64")

set(IMGUI_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/../Third-party/Libs/imgui")

set(LIBPNG_INCLUDE "C:/Users/tytra/Documents/libs/libpng/1.6.40")
set(LIBPNG_LIB "C:/Users/tytra/Documents/libs/libpng/1.6.40/build/Debug")

set(OPENAL_INCLUDE "C:/Users/tytra/Documents/libs/openal-soft/include")
set(OPENAL_LIB "C:/Users/tytra/Documents/libs/openal-soft/libs/Win64")

file(GLOB CORE_HEADERS
	"${CORE_INCLUDE}/*.hpp" "${CORE_INCLUDE}/*.h"
	"${CORE_INCLUDE}/DE/*.hpp" "${CORE_INCLUDE}/DE/*.h"
	"${CORE_INCLUDE}/DE/audio/*.hpp" "${CORE_INCLUDE}/DE/audio/*.h"
	"${CORE_INCLUDE}/DE/c-wrapper/*.hpp" "${CORE_INCLUDE}/DE/c-wrapper/*.h"
	"${CORE_INCLUDE}/DE/ecs/*.hpp" "${CORE_INCLUDE}/DE/ecs/*.h"
	"${CORE_INCLUDE}/DE/graphics/*.hpp" "${CORE_INCLUDE}/DE/graphics/*.h"
	"${CORE_INCLUDE}/DE/image/*.hpp" "${CORE_INCLUDE}/DE/image/*.h"
	"${CORE_INCLUDE}/DE/imgui/*.hpp" "${CORE_INCLUDE}/DE/imgui/*.h"
	"${CORE_INCLUDE}/DE/memory/*.hpp" "${CORE_INCLUDE}/DE/memory/*.h"
	"${CORE_INCLUDE}/DE/rendering/*.hpp" "${CORE_INCLUDE}/DE/rendering/*.h"
    "${CORE_INCLUDE}/DE/resource/*.hpp" "${CORE_INCLUDE}/DE/resource/*.h"
    "${CORE_INCLUDE}/DE/file/*.hpp" "${CORE_INCLUDE}/DE/file/*.h"

	"${CORE_INCLUDE}/glad/*.hpp" "${CORE_INCLUDE}/glad/*.h"
	"${CORE_INCLUDE}/KHR/*.hpp" "${CORE_INCLUDE}/KHR/*.h"

	"${IMGUI_INCLUDE}/*.hpp" "${IMGUI_INCLUDE}/*.h"
	"${IMGUI_INCLUDE}/backends/*.hpp" "${IMGUI_INCLUDE}/backends/*.h"
)

file(GLOB CORE_SOURCES
	"${CORE_SOURCE}/*.cpp" "${CORE_SOURCE}/*.c"
	"${CORE_SOURCE}/audio/*.cpp" "${CORE_SOURCE}/audio/*.c"
	"${CORE_SOURCE}/ecs/*.cpp" "${CORE_SOURCE}/ecs/*.c"
	"${CORE_SOURCE}/glad/*.cpp" "${CORE_SOURCE}/glad/*.c"
	"${CORE_SOURCE}/graphics/*.cpp" "${CORE_SOURCE}/graphics/*.c"
	"${CORE_SOURCE}/image/*.cpp" "${CORE_SOURCE}/image/*.c"
	"${CORE_SOURCE}/imgui/*.cpp" "${CORE_SOURCE}/imgui/*.c"
	"${CORE_SOURCE}/memory/*.cpp" "${CORE_SOURCE}/memory/*.c" "${CORE_SOURCE}/src/memory/*.asm"
	"${CORE_SOURCE}/rendering/*.cpp" "${CORE_SOURCE}/rendering/*.c"
    "${CORE_SOURCE}/resource/*.cpp" "${CORE_SOURCE}/resource/*.c"
    "${CORE_SOURCE}/file/*.cpp" "${CORE_SOURCE}/file/*.c"

	"${IMGUI_INCLUDE}/*.cpp" "${IMGUI_INCLUDE}/*.c"
	"${IMGUI_INCLUDE}/backends/imgui_impl_sdl2.cpp"
	"${IMGUI_INCLUDE}/backends/imgui_impl_opengl3.cpp"
)


add_library(${CORE_PROJECT} SHARED ${CORE_HEADERS} ${CORE_SOURCES})

target_include_directories(${CORE_PROJECT} PRIVATE ${CORE_INCLUDE})
target_include_directories(${CORE_PROJECT} PUBLIC
	${SDL_INCLUDE}
	${IMGUI_INCLUDE}
	${LIBPNG_INCLUDE}
    ${OPENAL_INCLUDE}
)

target_link_directories(${CORE_PROJECT} PUBLIC
	${SDL_LIB}
	${LIBPNG_LIB}
    ${OPENAL_LIB}
)

target_link_libraries(${CORE_PROJECT}
	SDL2.lib
	libpng16d.lib
	OpenGL32.lib
    OpenAL32.lib
)

target_compile_definitions(${CORE_PROJECT} PRIVATE DEEPENGINE_LIB=1)