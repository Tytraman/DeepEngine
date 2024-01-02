# Indique la version minimum de CMake requise pour exécuter le script.
cmake_minimum_required(VERSION 3.15)

# Indique les versions à utiliser des langages.
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Build")

if(MSVC)
    #add_compile_options(/fsanitize=address)
    # Enable Hot Reload for MSVC compilers if supported.
    if(POLICY CMP0141)
      cmake_policy(SET CMP0141 NEW)
      set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endif()

enable_language(C ASM_MASM)

include("variables.cmake")

project(${DE_PROJECT} VERSION 0.0.1)

set(CORE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Core")
set(CORE_BUILD_DIRECTORY "${CORE_DIRECTORY}/Build")

# Crée un dossier au moment de la génération du projet CMake.
file(MAKE_DIRECTORY "${CORE_BUILD_DIRECTORY}")
add_subdirectory("${CORE_DIRECTORY}" "${CORE_BUILD_DIRECTORY}")

add_compile_definitions(PRIVATE DE_AUTHORS=${DE_AUTHORS})
add_compile_definitions(PRIVATE DE_VERSION=${DE_VERSION})

file(GLOB DE_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/Source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/Source/*.c"
)

add_executable(${DE_PROJECT} ${DE_SOURCES})

if(MSVC)
    target_compile_options(${DE_PROJECT} PRIVATE /W4 /WX)
endif()


target_include_directories(${DE_PROJECT} PUBLIC
    ${CORE_INCLUDE}
)

target_link_directories(${DE_PROJECT} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/out/build/x64-Debug")

target_link_libraries(${DE_PROJECT}
    ${CORE_PROJECT}
)
