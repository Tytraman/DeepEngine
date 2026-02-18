cmake_minimum_required(VERSION 3.15.7)

find_program(FXC_EXECUTABLE fxc.exe)

function(deep_compile_shader SHADER_FILE TARGET_PROFILE ENTRY_POINT)
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
    set(OUTPUT_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SHADER_NAME}_${TARGET_PROFILE}.cso")

    add_custom_command(
        OUTPUT "${OUTPUT_FILE}"
        COMMAND ${FXC_EXECUTABLE} /T "${TARGET_PROFILE}_5_0" /E ${ENTRY_POINT} /Fo ${OUTPUT_FILE} ${SHADER_FILE}
        DEPENDS ${SHADER_FILE}
    )

    set(SHADER_CSO_FILES ${SHADER_CSO_FILES} ${OUTPUT_FILE} PARENT_SCOPE)
endfunction()

deep_compile_shader("${CMAKE_CURRENT_LIST_DIR}/DeepEngine/Shaders/test_triangle.hlsl" "vs" "main_vs")
deep_compile_shader("${CMAKE_CURRENT_LIST_DIR}/DeepEngine/Shaders/test_triangle.hlsl" "ps" "main_ps")

deep_compile_shader("${CMAKE_CURRENT_LIST_DIR}/DeepEngine/Shaders/test_cube.hlsl" "vs" "main_vs")
deep_compile_shader("${CMAKE_CURRENT_LIST_DIR}/DeepEngine/Shaders/test_cube.hlsl" "ps" "main_ps")

deep_compile_shader("${CMAKE_CURRENT_LIST_DIR}/DeepEngine/Shaders/textured_cube.hlsl" "vs" "main_vs")
deep_compile_shader("${CMAKE_CURRENT_LIST_DIR}/DeepEngine/Shaders/textured_cube.hlsl" "ps" "main_ps")

add_library(DeepEngine SHARED
    "${CMAKE_CURRENT_LIST_DIR}/DeepEngine/engine.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DeepEngine/camera.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DeepEngine/GUI/imgui_manager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DeepEngine/GUI/imgui_drawable.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DeepEngine/GUI/imgui_debug_panel.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DeepEngine/GUI/imgui_chat.cpp"
    ${SHADER_CSO_FILES})
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

set(DeepModules "${CMAKE_SOURCE_DIR}/Modules")

add_subdirectory("${DeepModules}/Renderer" "${CMAKE_CURRENT_BINARY_DIR}/Renderer")

# Ajoute des constantes au préprocesseur.
add_compile_definitions(PRIVATE DE_AUTHORS=${DE_AUTHORS})   # Le nom des auteurs stockés dans les variables partagées.
add_compile_definitions(PRIVATE DE_VERSION=${DE_VERSION})   # Le nom de la version actuelle stockée dans les variables partagées.

target_include_directories(DeepEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>

        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(DeepEngine
    PUBLIC
        Deep::Core
        Deep::Lib
        Deep::D3D
    PRIVATE
        imgui::imgui)

target_compile_definitions(DeepEngine PRIVATE DEEPENGINE_LIB=1)