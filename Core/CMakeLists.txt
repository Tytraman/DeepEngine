# Indique la version minimum de CMake requise pour exécuter le script.
cmake_minimum_required(VERSION 3.15)

message(STATUS "=====[ DeepEngineCore ]=====")

# Indique les versions à utiliser des langages.
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build")

project(${CORE_PROJECT} VERSION 0.0.1 LANGUAGES C CXX)

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
set(LIBZIP_BUILD_DIRECTORY "${THIRD_PARTY_LIBS}/Build/libzip")
set(LIBPNG_BUILD_DIRECTORY "${THIRD_PARTY_LIBS}/Build/libpng")
set(SDL_BUILD_DIRECTORY "${THIRD_PARTY_LIBS}/Build/SDL")
set(OPENALSOFT_BUILD_DIRECTORY "${THIRD_PARTY_LIBS}/Build/openal-soft")

# Crée un dossier au moment de la génération du projet CMake.
file(MAKE_DIRECTORY "${ZLIB_BUILD_DIRECTORY}")
file(MAKE_DIRECTORY "${LIBZIP_BUILD_DIRECTORY}")
file(MAKE_DIRECTORY "${LIBPNG_BUILD_DIRECTORY}")
file(MAKE_DIRECTORY "${SDL_BUILD_DIRECTORY}")
file(MAKE_DIRECTORY "${OPENALSOFT_BUILD_DIRECTORY}")

# Certaines dépendances ont besion de zlib, généralement ce sont ces 2 variables qui sont utilisées.
set(ZLIB_INCLUDE_DIR "${THIRD_PARTY_LIBS}/zlib" CACHE INTERNAL "zlib_include" FORCE)
set(ZLIB_LIBRARY "${ZLIB_BUILD_DIRECTORY}/zlibd.lib")

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

set(LIBZIP_INCLUDE "${THIRD_PARTY_LIBS}/libzip/lib")

set(LIBPNG_INCLUDE "${THIRD_PARTY_LIBS}/libpng")
set(LIBPNG_LIB "${LIBPNG_BUILD_DIRECTORY}")
set(LIBPNG_INCLUDE_2 "${LIBPNG_BUILD_DIRECTORY}")

set(OPENAL_INCLUDE "${THIRD_PARTY_LIBS}/openal-soft/include")
set(OPENAL_LIB "${OPENALSOFT_BUILD_DIRECTORY}")

message(STATUS "===== Add subdir zlib =====")
# Indique à CMake qu'un sous projet CMake est présent et doit être compilé.
add_subdirectory("${THIRD_PARTY_LIBS}/zlib" "${ZLIB_BUILD_DIRECTORY}")
# zlib génère une config qui est nécessaire par d'autres dépendances.
file(COPY "${ZLIB_BUILD_DIRECTORY}/zconf.h" DESTINATION "${ZLIB_INCLUDE}")

message(STATUS "===== Add subdir libzip =====")
add_subdirectory("${THIRD_PARTY_LIBS}/libzip" "${LIBZIP_BUILD_DIRECTORY}")
file(COPY "${LIBZIP_BUILD_DIRECTORY}/zipconf.h" DESTINATION "${LIBZIP_INCLUDE}")

message(STATUS "===== Add subdir libpng =====")
add_subdirectory("${THIRD_PARTY_LIBS}/libpng" "${LIBPNG_BUILD_DIRECTORY}")

message(STATUS "===== Add subdir SDL =====")
add_subdirectory("${THIRD_PARTY_LIBS}/SDL" "${SDL_BUILD_DIRECTORY}")

message(STATUS "===== Add subdir openal-soft =====")
add_subdirectory("${THIRD_PARTY_LIBS}/openal-soft" "${OPENALSOFT_BUILD_DIRECTORY}")

# add_folders est le nom de la fonction.
# dest_list est un paramètre qui correspond à la liste dans laquelle ajouter tous les fichiers.
function(deep_add_folders dest_list)
    set(options)
    set(args INCLUDE_DIR)
    set(list_args FOLDERS EXTENSIONS)
    cmake_parse_arguments(
        PARSE_ARGV 0
        DEEP_FOLDER
        "${options}"
        "${args}"
        "${list_args}"
    )

    # Affiche un warning pour tous les paramètres qui n'ont pas de nom associé lors de l'appel.
    # foreach(arg IN LISTS DEEP_FOLDER_UNPARSED_ARGUMENTS)
        # message(WARNING "Unparsed argument: ${arg}")
    # endforeach()

    set(files ${${dest_list}})
    # message(STATUS "current list: ${files}")

    #message(STATUS "Add folders: ${DEEP_FOLDER_FOLDERS} in ${DEEP_FOLDER_INCLUDE_DIR} with extensions: ${DEEP_FOLDER_EXTENSIONS}")

    # Pour chaque dossier que l'on ajoute.
    foreach(folder IN LISTS DEEP_FOLDER_FOLDERS)
        if(NOT ${folder} EQUAL "")
            if("${folder}" MATCHES "[ \t\n]+")
                set(temp "")
                set(folder "")
            else()
                set(temp "/")
            endif()
        endif()

        set(file "${DEEP_FOLDER_INCLUDE_DIR}/${folder}")
        string(APPEND file ${temp})

        # Pour chaque extension que l'on veut ajouter.
        foreach(extension IN LISTS DEEP_FOLDER_EXTENSIONS)
            set(final_file "${file}*${extension}")

            #message(STATUS "Final File: ${final_file}")

            list(APPEND globbing_files "${final_file}")
        endforeach()
    endforeach()

    #message(STATUS "globbing_files: ${globbing_files}")

    # Cette fonction n'ajoute pas les fichiers dans la liste mais l'écrase entièrement.
    file(
        GLOB new_files
        ${globbing_files}
    )

    #message(STATUS "new_files: ${new_files}")

    # Il faut donc itérer chaque fichier trouvé et l'ajouter dans la liste destination.
    foreach(f IN LISTS new_files)
        list(APPEND files ${f})
    endforeach()

    # Il n'est pas possible de modifier la liste référencée autrement que par un 'set', d'où les copies et l'itération précédentes.
    set(${dest_list} ${files} PARENT_SCOPE)
endfunction()

function(deep_add_files dest_list)
    set(options)
    set(args INCLUDE_DIR)
    set(list_args FILES)
    cmake_parse_arguments(
        PARSE_ARGV 0
        DEEP_FILE
        "${options}"
        "${args}"
        "${list_args}"
    )

    set(files ${${dest_list}})

    foreach(file IN LISTS DEEP_FILE_FILES)
        list(APPEND files "${DEEP_FILE_INCLUDE_DIR}/${file}")
    endforeach()

    set(${dest_list} ${files} PARENT_SCOPE)
endfunction()

deep_add_folders(
    DEEP_HEADER_FILES
    INCLUDE_DIR ${CORE_INCLUDE}
    FOLDERS "glad" "KHR"
    EXTENSIONS ".hpp" ".h"
)

deep_add_folders(
    DEEP_HEADER_FILES
    INCLUDE_DIR ${IMGUI_INCLUDE}
    FOLDERS " " "backends"
    EXTENSIONS ".hpp" ".h"
)

deep_add_folders(
    DEEP_HEADER_FILES
    INCLUDE_DIR ${CORE_INCLUDE}/DE
    FOLDERS "audio" "core" "ecs" "file" "graphics" "gui" "hardware" "image" "io" "maths" "os" "modules"
    EXTENSIONS ".hpp" ".h"
)

deep_add_folders(
    DEEP_HEADER_FILES
    INCLUDE_DIR ${CORE_INCLUDE}/DE/drivers
    FOLDERS "opengl"
    EXTENSIONS ".hpp" ".h"
)

deep_add_folders(
    DEEP_HEADER_FILES
    INCLUDE_DIR ${CORE_INCLUDE}/DE/modules
    FOLDERS "zip"
    EXTENSIONS ".hpp" ".h"
)

message(STATUS "DEEP_HEADER_FILES: ${DEEP_HEADER_FILES}")

deep_add_folders(
    DEEP_SOURCE_FILES
    INCLUDE_DIR ${IMGUI_INCLUDE}
    FOLDERS " "
    EXTENSIONS ".c" ".cpp"
)

deep_add_files(
    DEEP_SOURCE_FILES
    INCLUDE_DIR ${IMGUI_INCLUDE}/backends
    FILES "imgui_impl_sdl2.cpp" "imgui_impl_opengl3.cpp"
)

deep_add_folders(
    DEEP_SOURCE_FILES
    INCLUDE_DIR ${CORE_SOURCE}
    FOLDERS "" "core" "maths" "os" "hardware" "audio" "ecs" "glad" "graphics" "image" "gui" "file" "io" "modules"
    EXTENSIONS ".c" ".cpp"
)

deep_add_folders(
    DEEP_SOURCE_FILES
    INCLUDE_DIR ${CORE_SOURCE}/drivers
    FOLDERS "opengl"
    EXTENSIONS ".c" ".cpp"
)

deep_add_folders(
    DEEP_SOURCE_FILES
    INCLUDE_DIR ${CORE_SOURCE}/modules
    FOLDERS "zip"
    EXTENSIONS ".c" ".cpp"
)

message(STATUS "DEEP_SOURCE_FILES: ${DEEP_SOURCE_FILES}")

add_library(${CORE_PROJECT} SHARED ${DEEP_HEADER_FILES} ${DEEP_SOURCE_FILES})

if(MSVC)
    # Le problème de cette commande est qu'elle applique le niveau de warning même aux dépendances, ce qui empêche parfois la compilation du projet.
    # target_compile_options(${CORE_PROJECT} PRIVATE /W4 /WX)
    set_source_files_properties(${DEEP_SOURCE_FILES} PROPERTIES COMPILE_OPTIONS /W4 COMPILE_OPTIONS /WX)
endif()

target_include_directories(${CORE_PROJECT} PRIVATE ${CORE_INCLUDE})
target_include_directories(${CORE_PROJECT} PUBLIC
    ${SDL_INCLUDE}
    ${IMGUI_INCLUDE}
    ${ZLIB_INCLUDE}
    ${ZLIB_INCLUDE_2}
    ${LIBZIP_INCLUDE}
    ${LIBPNG_INCLUDE}
    ${LIBPNG_INCLUDE_2}
    ${OPENAL_INCLUDE}
    ${THIRD_PARTY_LIBS}
)

target_link_directories(${CORE_PROJECT} PUBLIC
    ${ZLIB_LIB}
    ${LIBPNG_LIB}
    ${OPENAL_LIB}
)

target_link_libraries(${CORE_PROJECT}
    Bcrypt.lib
    SDL2-static     # Nom de la cible de la SDL2 pour que CMake crée une dépendance.
    zlibstatic      # Nom de la cible de zlib pour que CMake crée une dépendance.
    zip
    png_static      # Nom de la cible de libpng pour que CMake crée une dépendance.
    OpenGL32.lib    # Nom de la cible OpenGL pour que CMake le considère comme une dépendance.
    OpenAL          # Nom de la cible de OpenAL pour que CMake crée une dépendance.
)

target_compile_definitions(${CORE_PROJECT} PRIVATE DEEPENGINE_LIB=1)