# Indique la version minimum de CMake requise pour exécuter le script.
cmake_minimum_required(VERSION 3.15)

# Indique les versions à utiliser des langages.
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build")

enable_language(C ASM_MASM)

project(${CORE_PROJECT} VERSION 0.0.1)

if(MSVC)
    # add_compile_options(/fsanitize=address)
    # Enable Hot Reload for MSVC compilers if supported.
    if(POLICY CMP0141)
      cmake_policy(SET CMP0141 NEW)
      set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endif()

# Inclue les variables partagées.
include("${CMAKE_CURRENT_LIST_DIR}/../variables.cmake")

set(THIRD_PARTY_LIBS "${CMAKE_CURRENT_LIST_DIR}/../Third-party/Libs")
set(ZLIB_BUILD_DIRECTORY "${THIRD_PARTY_LIBS}/Build/zlib")
set(LIBPNG_BUILD_DIRECTORY "${THIRD_PARTY_LIBS}/Build/libpng")
set(SDL_BUILD_DIRECTORY "${THIRD_PARTY_LIBS}/Build/SDL")
set(OPENALSOFT_BUILD_DIRECTORY "${THIRD_PARTY_LIBS}/Build/openal-soft")

# Crée un dossier au moment de la génération du projet CMake.
file(MAKE_DIRECTORY "${ZLIB_BUILD_DIRECTORY}")
file(MAKE_DIRECTORY "${LIBPNG_BUILD_DIRECTORY}")
file(MAKE_DIRECTORY "${SDL_BUILD_DIRECTORY}")
file(MAKE_DIRECTORY "${OPENALSOFT_BUILD_DIRECTORY}")

# Certaines dépendances ont besion de zlib, généralement ce sont ces 2 variables qui sont utilisées.
set(ZLIB_INCLUDE_DIR "${THIRD_PARTY_LIBS}/zlib" CACHE INTERNAL "zlib_include")
set(ZLIB_LIBRARY "${ZLIB_BUILD_DIRECTORY}/zlibd.lib")

# Indique à CMake qu'un sous projet CMake est présent et doit être compilé.
add_subdirectory("${THIRD_PARTY_LIBS}/zlib" "${ZLIB_BUILD_DIRECTORY}")
add_subdirectory("${THIRD_PARTY_LIBS}/libpng" "${LIBPNG_BUILD_DIRECTORY}")
add_subdirectory("${THIRD_PARTY_LIBS}/SDL" "${SDL_BUILD_DIRECTORY}")
add_subdirectory("${THIRD_PARTY_LIBS}/openal-soft" "${OPENALSOFT_BUILD_DIRECTORY}")

# Ajoute des constantes au préprocesseur.
add_compile_definitions(PRIVATE DE_AUTHORS=${DE_AUTHORS})   # Le nom des auteurs stockés dans les variables partagées.
add_compile_definitions(PRIVATE DE_VERSION=${DE_VERSION})   # Le nom de la version actuelle stockée dans les variables partagées.

set(CORE_SOURCE "${CMAKE_CURRENT_LIST_DIR}/Source" CACHE INTERNAL "")
set(CORE_INCLUDE "${CORE_SOURCE}/Include" CACHE INTERNAL "")

# Variables qui stockent les chemins des différentes librairies utilisées.
set(SDL_INCLUDE "${THIRD_PARTY_LIBS}/SDL/include")

set(IMGUI_INCLUDE "${THIRD_PARTY_LIBS}/imgui")

set(ZLIB_INCLUDE "${THIRD_PARTY_LIBS}/zlib")
set(ZLIB_LIB "${ZLIB_BUILD_DIRECTORY}")
set(ZLIB_INCLUDE_2 "${ZLIB_BUILD_DIRECTORY}")

set(LIBPNG_INCLUDE "${THIRD_PARTY_LIBS}/libpng")
set(LIBPNG_LIB "${LIBPNG_BUILD_DIRECTORY}")

set(OPENAL_INCLUDE "${THIRD_PARTY_LIBS}/openal-soft/include")

file(GLOB CORE_HEADERS
    "${CORE_INCLUDE}/glad/*.hpp" "${CORE_INCLUDE}/glad/*.h"
    "${CORE_INCLUDE}/KHR/*.hpp" "${CORE_INCLUDE}/KHR/*.h"

    "${IMGUI_INCLUDE}/*.hpp" "${IMGUI_INCLUDE}/*.h"
    "${IMGUI_INCLUDE}/backends/*.hpp" "${IMGUI_INCLUDE}/backends/*.h"

    "${CORE_INCLUDE}/*.hpp" "${CORE_INCLUDE}/*.h"
    "${CORE_INCLUDE}/DE/*.hpp" "${CORE_INCLUDE}/DE/*.h"
    "${CORE_INCLUDE}/DE/audio/*.hpp" "${CORE_INCLUDE}/DE/audio/*.h"
    "${CORE_INCLUDE}/DE/c-wrapper/*.hpp" "${CORE_INCLUDE}/DE/c-wrapper/*.h"
    "${CORE_INCLUDE}/DE/drivers/opengl/*.hpp" "${CORE_INCLUDE}/DE/drivers/opengl/*.h"
    "${CORE_INCLUDE}/DE/ecs/*.hpp" "${CORE_INCLUDE}/DE/ecs/*.h"
    "${CORE_INCLUDE}/DE/graphics/*.hpp" "${CORE_INCLUDE}/DE/graphics/*.h"
    "${CORE_INCLUDE}/DE/image/*.hpp" "${CORE_INCLUDE}/DE/image/*.h"
    "${CORE_INCLUDE}/DE/imgui/*.hpp" "${CORE_INCLUDE}/DE/imgui/*.h"
    "${CORE_INCLUDE}/DE/memory/*.hpp" "${CORE_INCLUDE}/DE/memory/*.h"
    "${CORE_INCLUDE}/DE/rendering/*.hpp" "${CORE_INCLUDE}/DE/rendering/*.h"
    "${CORE_INCLUDE}/DE/resource/*.hpp" "${CORE_INCLUDE}/DE/resource/*.h"
    "${CORE_INCLUDE}/DE/file/*.hpp" "${CORE_INCLUDE}/DE/file/*.h"
)

file(GLOB CORE_SOURCES
    "${IMGUI_INCLUDE}/*.cpp" "${IMGUI_INCLUDE}/*.c"
    "${IMGUI_INCLUDE}/backends/imgui_impl_sdl2.cpp"
    "${IMGUI_INCLUDE}/backends/imgui_impl_opengl3.cpp"

    "${CORE_SOURCE}/*.cpp" "${CORE_SOURCE}/*.c"
    "${CORE_SOURCE}/audio/*.cpp" "${CORE_SOURCE}/audio/*.c"
    "${CORE_SOURCE}/drivers/opengl/*.cpp" "${CORE_SOURCE}/drivers/opengl/*.c"
    "${CORE_SOURCE}/ecs/*.cpp" "${CORE_SOURCE}/ecs/*.c"
    "${CORE_SOURCE}/glad/*.cpp" "${CORE_SOURCE}/glad/*.c"
    "${CORE_SOURCE}/graphics/*.cpp" "${CORE_SOURCE}/graphics/*.c"
    "${CORE_SOURCE}/image/*.cpp" "${CORE_SOURCE}/image/*.c"
    "${CORE_SOURCE}/imgui/*.cpp" "${CORE_SOURCE}/imgui/*.c"
    "${CORE_SOURCE}/memory/*.cpp" "${CORE_SOURCE}/memory/*.c" "${CORE_SOURCE}/src/memory/*.asm"
    "${CORE_SOURCE}/rendering/*.cpp" "${CORE_SOURCE}/rendering/*.c"
    "${CORE_SOURCE}/resource/*.cpp" "${CORE_SOURCE}/resource/*.c"
    "${CORE_SOURCE}/file/*.cpp" "${CORE_SOURCE}/file/*.c"
)


add_library(${CORE_PROJECT} SHARED ${CORE_HEADERS} ${CORE_SOURCES})

if(MSVC)
    target_compile_options(${CORE_PROJECT} PRIVATE /W4 /WX)
endif()

target_include_directories(${CORE_PROJECT} PRIVATE ${CORE_INCLUDE})
target_include_directories(${CORE_PROJECT} PUBLIC
    ${SDL_INCLUDE}
    ${IMGUI_INCLUDE}
    ${ZLIB_INCLUDE_2}
    ${ZLIB_INCLUDE}
    ${LIBPNG_INCLUDE}
    ${OPENAL_INCLUDE}
)

target_link_directories(${CORE_PROJECT} PUBLIC
    ${ZLIB_LIB}
    ${LIBPNG_LIB}
    ${OPENAL_LIB}
)

target_link_libraries(${CORE_PROJECT}
    SDL2-static     # Nom de la cible de la SDL2 pour que CMake crée une dépendance.
    zlibstatic      # Nom de la cible de zlib pour que CMake crée une dépendance.
    png_static      # Nom de la cible de libpng pour que CMake crée une dépendance.
    OpenGL32.lib
    OpenAL          # Nom de la cible de OpenAL pour que CMake crée une dépendance.
)

target_compile_definitions(${CORE_PROJECT} PRIVATE DEEPENGINE_LIB=1)