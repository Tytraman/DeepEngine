cmake_minimum_required(VERSION 3.15.7)

message(STATUS "=====[ DeepEngineCore ]=====")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build")

project(${CORE_PROJECT}
    VERSION 0.0.1
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

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

# Ajoute des constantes au préprocesseur.
add_compile_definitions(PRIVATE DE_AUTHORS=${DE_AUTHORS})   # Le nom des auteurs stockés dans les variables partagées.
add_compile_definitions(PRIVATE DE_VERSION=${DE_VERSION})   # Le nom de la version actuelle stockée dans les variables partagées.

set(CORE_SOURCE "${CMAKE_CURRENT_LIST_DIR}/Source" CACHE INTERNAL "")
set(CORE_INCLUDE "${CORE_SOURCE}" CACHE INTERNAL "")

find_package(SDL2 CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(libzip CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

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
    DEEP_SOURCE_FILES
    INCLUDE_DIR ${CORE_SOURCE}
    FOLDERS "" "core" "maths" "os" "hardware" "audio" "ecs" "graphics" "image" "gui" "file" "io" "modules" "modules/zip"
    EXTENSIONS ".c" ".cpp"
)

deep_add_folders(
    DEEP_SOURCE_FILES
    INCLUDE_DIR ${CORE_SOURCE}/core
    FOLDERS "resources" "string" "config" "templates" "nodes"
    EXTENSIONS ".c" ".cpp"
)

deep_add_folders(
    DEEP_SOURCE_FILES
    INCLUDE_DIR ${CORE_SOURCE}/drivers
    FOLDERS "opengl"
    EXTENSIONS ".c" ".cpp"
)

message(STATUS "DEEP_SOURCE_FILES: ${DEEP_SOURCE_FILES}")

add_library(${CORE_PROJECT} SHARED ${DEEP_SOURCE_FILES})

if(MSVC)
    # Le problème de cette commande est qu'elle applique le niveau de warning même aux dépendances, ce qui empêche parfois la compilation du projet.
    # target_compile_options(${CORE_PROJECT} PRIVATE /W4 /WX)
    set_source_files_properties(${DEEP_SOURCE_FILES} PROPERTIES COMPILE_OPTIONS /W4 COMPILE_OPTIONS /WX)
endif()

target_include_directories(${CORE_PROJECT} PRIVATE ${CORE_INCLUDE})

target_link_libraries(${CORE_PROJECT}
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        libzip::zip
        PNG::PNG
        OpenGL32.lib    # Nom de la cible OpenGL pour que CMake le considère comme une dépendance.
        OpenAL::OpenAL
        imgui::imgui
        glad::glad)

target_compile_definitions(${CORE_PROJECT} PRIVATE DEEPENGINE_LIB=1)