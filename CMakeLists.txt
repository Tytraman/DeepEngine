# Indique la version minimum de CMake requise pour exécuter le script.
cmake_minimum_required(VERSION "3.26.4")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build")

# Variables qui contiennent le nom des différents projets.
set(DE_PROJECT "DeepEngine")
set(CUI_PROJECT "CreationUI")

# Variables qui stockent les chemins des différentes librairies utilisées.
set(SDL_INCLUDE "C:/Users/tytra/Documents/libs/SDL2/include")
set(SDL_LIB "C:/Users/tytra/Documents/libs/SDL2/lib/x64")

set(IMGUI_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/imgui")

set(LIBPNG_INCLUDE "C:/Users/tytra/Documents/libs/libpng/1.6.40")
set(LIBPNG_LIB "C:/Users/tytra/Documents/libs/libpng/1.6.40/build/Debug")



# ==========[ DeepEngine ]========== #

set(DE_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/")

file(GLOB DE_HEADERS
	"${CMAKE_CURRENT_LIST_DIR}/include/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/audio/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/audio/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/c-wrapper/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/c-wrapper/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/ecs/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/ecs/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/graphics/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/graphics/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/image/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/image/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/imgui/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/imgui/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/memory/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/memory/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/DE/rendering/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/DE/rendering/*.h"

	"${CMAKE_CURRENT_LIST_DIR}/include/glad/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/glad/*.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/KHR/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include/KHR/*.h"

	"${IMGUI_INCLUDE}/*.hpp" "${IMGUI_INCLUDE}/*.h"
	"${IMGUI_INCLUDE}/backends/*.hpp" "${IMGUI_INCLUDE}/backends/*.h"
)

file(GLOB DE_SOURCES
	"${CMAKE_CURRENT_LIST_DIR}/src/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
	"${CMAKE_CURRENT_LIST_DIR}/src/audio/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/audio/*.c"
	"${CMAKE_CURRENT_LIST_DIR}/src/ecs/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/ecs/*.c"
	"${CMAKE_CURRENT_LIST_DIR}/src/glad/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/glad/*.c"
	"${CMAKE_CURRENT_LIST_DIR}/src/graphics/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/graphics/*.c"
	"${CMAKE_CURRENT_LIST_DIR}/src/image/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/image/*.c"
	"${CMAKE_CURRENT_LIST_DIR}/src/imgui/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/imgui/*.c"
	"${CMAKE_CURRENT_LIST_DIR}/src/memory/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/memory/*.c"
	"${CMAKE_CURRENT_LIST_DIR}/src/rendering/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/rendering/*.c"

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
)

target_link_directories(${DE_PROJECT} PUBLIC
	${SDL_LIB}
	${LIBPNG_LIB}
)

target_link_libraries(${DE_PROJECT}
	SDL2.lib
	libpng16d.lib
	OpenGL32.lib
)

target_compile_definitions(${DE_PROJECT} PRIVATE DEEPENGINE_LIB=1)



# ==========[ Creation UI ]========== #

file(GLOB CUI_SOURCES
	"${CMAKE_CURRENT_LIST_DIR}/Tester/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/Tester/*.c"
)

add_executable(${CUI_PROJECT} ${CUI_SOURCES})

target_include_directories(${CUI_PROJECT} PUBLIC
	${DE_INCLUDE}
	${SDL_INCLUDE}
)

target_link_directories(${CUI_PROJECT} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/out/build/x64-Debug")

target_link_libraries(${CUI_PROJECT} ${DE_PROJECT})
