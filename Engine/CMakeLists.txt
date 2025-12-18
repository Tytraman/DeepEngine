cmake_minimum_required(VERSION 3.15.7)

add_library(DeepEngine SHARED
    "${CMAKE_CURRENT_LIST_DIR}/DeepEngine/engine.cpp")
add_library(Deep::Engine ALIAS DeepEngine)

set_target_properties(DeepEngine PROPERTIES
    OUTPUT_NAME DeepEngine
    EXPORT_NAME Engine
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PREFIX ""   # Retire le prefix du nom du fichier généré ('lib' sous Linux par exemple).
    # Indique que les symboles sont cachés par défaut, permettant une uniformisation entre les
    # différents compilateurs et un meilleur contrôle sur la sortie générée par le linker.
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
    # Indique que les fonctions 'inline' sont cachées par défaut, permettant de réduire la taille
    # du fichier généré lors de l'utilisation de templates en C++.
    VISIBILITY_INLINES_HIDDEN TRUE)

set(DeepEngineExport "${CMAKE_CURRENT_BINARY_DIR}/export/DeepEngine/deep_engine_export.h")

include(GenerateExportHeader)
generate_export_header(DeepEngine
    EXPORT_FILE_NAME "${DeepEngineExport}"
    EXPORT_MACRO_NAME DEEP_ENGINE_API)

set_target_properties(DeepEngine PROPERTIES
    DEEP_EXPORT_HEADER_FILE "${DeepEngineExport}")

if(MSVC)
    # add_compile_options(/fsanitize=address)
    # Enable Hot Reload for MSVC compilers if supported.
    if(POLICY CMP0141)
      cmake_policy(SET CMP0141 NEW)
      set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endif()

# Inclue les variables partagées.
include("${CMAKE_CURRENT_LIST_DIR}/../cmake/variables.cmake")

# Ajoute des constantes au préprocesseur.
add_compile_definitions(PRIVATE DE_AUTHORS=${DE_AUTHORS})   # Le nom des auteurs stockés dans les variables partagées.
add_compile_definitions(PRIVATE DE_VERSION=${DE_VERSION})   # Le nom de la version actuelle stockée dans les variables partagées.

find_package(libzip CONFIG REQUIRED)

target_include_directories(DeepEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>

        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(DeepEngine
    PUBLIC
        Deep::Core
        Deep::Lib
        libzip::zip)

target_compile_definitions(DeepEngine PRIVATE DEEPENGINE_LIB=1)