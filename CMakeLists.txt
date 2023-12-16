# Indique la version minimum de CMake requise pour exécuter le script.
cmake_minimum_required(VERSION 3.8)



# Indique les versions à utiliser des langages.
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build")

enable_language(C ASM_MASM)

if(MSVC)
    #add_compile_options(/fsanitize=address)
	# Enable Hot Reload for MSVC compilers if supported.
	if(POLICY CMP0141)
	  cmake_policy(SET CMP0141 NEW)
	  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
	endif()
endif()

# Variables qui contiennent le nom des différents projets.
set(DE_PROJECT "DeepEngine")
set(CUI_PROJECT "CreationUI")

# ==========[ DeepEngine ]========== #

set(DE_SOURCE "${CMAKE_CURRENT_LIST_DIR}/Source")
set(DE_INCLUDE "${DE_SOURCE}/Include")

# Variables qui stockent les chemins des différentes librairies utilisées.
set(SDL_INCLUDE "C:/Users/tytra/Documents/libs/SDL2/include")
set(SDL_LIB "C:/Users/tytra/Documents/libs/SDL2/lib/x64")

set(IMGUI_INCLUDE "${DE_INCLUDE}/imgui")

set(LIBPNG_INCLUDE "C:/Users/tytra/Documents/libs/libpng/1.6.40")
set(LIBPNG_LIB "C:/Users/tytra/Documents/libs/libpng/1.6.40/build/Debug")

set(OPENAL_INCLUDE "C:/Users/tytra/Documents/libs/openal-soft/include")
set(OPENAL_LIB "C:/Users/tytra/Documents/libs/openal-soft/libs/Win64")

file(GLOB DE_HEADERS
	"${DE_INCLUDE}/*.hpp" "${DE_INCLUDE}/*.h"
	"${DE_INCLUDE}/DE/*.hpp" "${DE_INCLUDE}/DE/*.h"
	"${DE_INCLUDE}/DE/audio/*.hpp" "${DE_INCLUDE}/DE/audio/*.h"
	"${DE_INCLUDE}/DE/c-wrapper/*.hpp" "${DE_INCLUDE}/DE/c-wrapper/*.h"
	"${DE_INCLUDE}/DE/ecs/*.hpp" "${DE_INCLUDE}/DE/ecs/*.h"
	"${DE_INCLUDE}/DE/graphics/*.hpp" "${DE_INCLUDE}/DE/graphics/*.h"
	"${DE_INCLUDE}/DE/image/*.hpp" "${DE_INCLUDE}/DE/image/*.h"
	"${DE_INCLUDE}/DE/imgui/*.hpp" "${DE_INCLUDE}/DE/imgui/*.h"
	"${DE_INCLUDE}/DE/memory/*.hpp" "${DE_INCLUDE}/DE/memory/*.h"
	"${DE_INCLUDE}/DE/rendering/*.hpp" "${DE_INCLUDE}/DE/rendering/*.h"
    "${DE_INCLUDE}/DE/resource/*.hpp" "${DE_INCLUDE}/DE/resource/*.h"
    "${DE_INCLUDE}/DE/file/*.hpp" "${DE_INCLUDE}/DE/file/*.h"

	"${DE_INCLUDE}/glad/*.hpp" "${DE_INCLUDE}/glad/*.h"
	"${DE_INCLUDE}/KHR/*.hpp" "${DE_INCLUDE}/KHR/*.h"

	"${IMGUI_INCLUDE}/*.hpp" "${IMGUI_INCLUDE}/*.h"
	"${IMGUI_INCLUDE}/backends/*.hpp" "${IMGUI_INCLUDE}/backends/*.h"
)

file(GLOB DE_SOURCES
	"${DE_SOURCE}/*.cpp" "${DE_SOURCE}/*.c"
	"${DE_SOURCE}/audio/*.cpp" "${DE_SOURCE}/audio/*.c"
	"${DE_SOURCE}/ecs/*.cpp" "${DE_SOURCE}/ecs/*.c"
	"${DE_SOURCE}/glad/*.cpp" "${DE_SOURCE}/glad/*.c"
	"${DE_SOURCE}/graphics/*.cpp" "${DE_SOURCE}/graphics/*.c"
	"${DE_SOURCE}/image/*.cpp" "${DE_SOURCE}/image/*.c"
	"${DE_SOURCE}/imgui/*.cpp" "${DE_SOURCE}/imgui/*.c"
	"${DE_SOURCE}/memory/*.cpp" "${DE_SOURCE}/memory/*.c" "${DE_SOURCE}/src/memory/*.asm"
	"${DE_SOURCE}/rendering/*.cpp" "${DE_SOURCE}/rendering/*.c"
    "${DE_SOURCE}/resource/*.cpp" "${DE_SOURCE}/resource/*.c"
    "${DE_SOURCE}/file/*.cpp" "${DE_SOURCE}/file/*.c"

	"${IMGUI_INCLUDE}/*.cpp" "${IMGUI_INCLUDE}/*.c"
	"${IMGUI_INCLUDE}/backends/imgui_impl_sdl2.cpp"
	"${IMGUI_INCLUDE}/backends/imgui_impl_opengl3.cpp"
)

project(${DE_PROJECT} VERSION 0.0.1)
add_library(${DE_PROJECT} SHARED ${DE_HEADERS} ${DE_SOURCES})

target_include_directories(${DE_PROJECT} PRIVATE ${DE_INCLUDE})
target_include_directories(${DE_PROJECT} PUBLIC
	${SDL_INCLUDE}
	${IMGUI_INCLUDE}
	${LIBPNG_INCLUDE}
    ${OPENAL_INCLUDE}
)

target_link_directories(${DE_PROJECT} PUBLIC
	${SDL_LIB}
	${LIBPNG_LIB}
    ${OPENAL_LIB}
)

target_link_libraries(${DE_PROJECT}
	SDL2.lib
	libpng16d.lib
	OpenGL32.lib
    OpenAL32.lib
)

target_compile_definitions(${DE_PROJECT} PRIVATE DEEPENGINE_LIB=1)
add_compile_definitions(PRIVATE DE_AUTHORS="Tytraman")
add_compile_definitions(PRIVATE DE_VERSION="dev build 0.0.1")


# ==========[ Creation UI ]========== #

file(GLOB CUI_SOURCES
	"${CMAKE_CURRENT_LIST_DIR}/CreationUI/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/CreationUI/*.c"
)

add_executable(${CUI_PROJECT} ${CUI_SOURCES})

target_include_directories(${CUI_PROJECT} PUBLIC
	${DE_INCLUDE}
	${SDL_INCLUDE}
)

target_link_directories(${CUI_PROJECT} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/out/build/x64-Debug")

target_link_libraries(${CUI_PROJECT} ${DE_PROJECT})

