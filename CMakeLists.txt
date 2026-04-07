cmake_minimum_required(VERSION 4.1)
project(cosmo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendor/entt EXCLUDE_FROM_ALL)

SET(glaze_ENABLE_AVX2 OFF)


include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
        OVERRIDE_FIND_PACKAGE)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.91.9
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG v3.0
)
FetchContent_MakeAvailable(imgui-sfml)

FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG v4.3.0
)
FetchContent_MakeAvailable(glaze)

add_executable(cosmo src/main.cpp
        src/orbital/KeplerSolver.cpp
        src/orbital/Simulation.cpp
        src/orbital/SystemFactory.cpp
        src/tactical/Tacview.cpp
        src/tactical/Tacicon.cpp
        src/ui/DebugContactUi.cpp
        src/ui/OrbitalCursor.cpp)

target_compile_features(cosmo PRIVATE cxx_std_20)
target_link_libraries(cosmo PRIVATE ImGui-SFML::ImGui-SFML gmp EnTT::EnTT glaze::glaze)

set(FONT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/fonts")
set(FONT_DEST_DIR "$<TARGET_FILE_DIR:cosmo>/bin/fonts")

add_custom_command(
        TARGET cosmo
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${FONT_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${FONT_SOURCE_DIR} ${FONT_DEST_DIR}
        COMMENT "Copying fonts to ${FONT_DEST_DIR}"
)

set(DATA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(DATA_DEST_DIR "$<TARGET_FILE_DIR:cosmo>/data")

add_custom_command(
        TARGET cosmo
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DATA_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${DATA_SOURCE_DIR} ${DATA_DEST_DIR}
        COMMENT "Copying data contents to ${DATA_DEST_DIR}"
)
