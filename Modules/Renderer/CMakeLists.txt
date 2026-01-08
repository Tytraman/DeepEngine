cmake_minimum_required(VERSION 3.15.7)

add_library(DeepD3D SHARED
    "${CMAKE_CURRENT_LIST_DIR}/D3D/graphics.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/error.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/device_context.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/resource_factory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/buffer/vertex_buffer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/buffer/constant_buffer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/shader/vertex_shader.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/shader/pixel_shader.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/shader/shader_factory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/drawable/drawable_factory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/drawable/triangle.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/D3D/drawable/rectangle.cpp"
)
add_library(Deep::D3D ALIAS DeepD3D)

set(DeepD3DExport "${CMAKE_CURRENT_BINARY_DIR}/export/deep_d3d_export.h")

include(GenerateExportHeader)
generate_export_header(DeepD3D
    EXPORT_FILE_NAME ${DeepD3DExport}
    EXPORT_MACRO_NAME DEEP_D3D_API)

set_target_properties(DeepD3D PROPERTIES
    DEEP_EXPORT_HEADER_FILE "${DeepD3DExport}")

target_link_libraries(DeepD3D
    PUBLIC
        Deep::Core
        Deep::Lib)

set_target_properties(DeepD3D PROPERTIES
    OUTPUT_NAME DeepD3D
    EXPORT_NAME D3D
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

# Ajoute des options de compilation et de link selon le compilateur utilisé.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Active l'outil de détection d'erreurs liées à la mémoire.
    set(FSANITIZE_ADDR $<$<CONFIG:Debug>:-fsanitize=address>)

    target_compile_options(DeepLib PRIVATE
        -Werror                                 # Considère les warning comme des erreurs.
        -Wall                                   # Active plusieurs warning.
        -Wextra                                 # Active encore plus de warning.
        ${FSANITIZE_ADDR}
    )

    target_link_options(DeepLib PRIVATE
        ${FSANITIZE_ADDR}
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Active l'outil de détection d'erreurs liées à la mémoire.
    set(FSANITIZE_ADDR $<$<CONFIG:Debug>:/fsanitize=address>)

    target_compile_options(DeepD3D
        PRIVATE
            /WX     # Considère les warning comme des erreurs.
            /W4     # Niveau d'avertissement élevé.
            ${FSANITIZE_ADDR}
    )

    target_link_options(DeepD3D
        PRIVATE
            ${FSANITIZE_ADDR}
    )
endif()

target_link_libraries(DeepD3D
    PRIVATE
        d3d11
        D3DCompiler.lib)

target_include_directories(DeepD3D
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>

        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)