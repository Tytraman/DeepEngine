cmake_minimum_required(VERSION 3.15.7)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build")

project(DeepEngineProject
    VERSION 0.0.1
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17) # Utilise le standard C++17.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Génère un fichier 'compile_commands.json'.

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(DeepEngineEditor
    "${CMAKE_CURRENT_LIST_DIR}/Editor/main.cpp")

message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

if(MSVC)
    #add_compile_options(/fsanitize=address)
    # Enable Hot Reload for MSVC compilers if supported.
    if(POLICY CMP0141)
      cmake_policy(SET CMP0141 NEW)
      set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endif()

find_package(DeepLib REQUIRED)
find_package(imgui CONFIG REQUIRED)

include("cmake/variables.cmake")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Engine")

add_compile_definitions(PRIVATE DE_AUTHORS=${DE_AUTHORS})
add_compile_definitions(PRIVATE DE_VERSION=${DE_VERSION})

target_link_libraries(DeepEngineEditor
    PRIVATE
        Deep::Lib
        Deep::Engine)

add_custom_command(TARGET DeepEngineEditor PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Deep::Core>
    $<TARGET_FILE:Deep::Lib>
    $<TARGET_FILE_DIR:DeepEngineEditor>)

# Crée un lien symbolique vers 'compile_commands.json' généré dans le dossier de Build.
file(CREATE_LINK
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json   # Fichier pointé.
    ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json     # Destination.
    COPY_ON_ERROR   # S'il n'est pas possible de créer de lien symbolique, fait une copie du fichier à la place.
    SYMBOLIC)

add_custom_command(TARGET DeepEngineEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/Resources"
        "$<TARGET_FILE_DIR:DeepEngine>/Resources"
)