cmake_minimum_required(VERSION 2.6)
project(bomns)
set(bomns_VERSION "0.99.3")

set(
  SOURCES
  src/bomns.cpp
  src/configfile.cpp
  src/level.cpp
  src/player.cpp
)

# SDL shit
find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
if(NOT SDL_FOUND OR NOT SDLMIXER_FOUND)
  message(FATAL_ERROR "Please install SDL and SDL_mixer and their respective -dev packages!")
endif()

# Config shit
set(bomns_RESOURCE_PATH "${CMAKE_INSTALL_PREFIX}/share/bomns")
configure_file(
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# Build shit
add_executable(bomns ${SOURCES})
target_link_libraries(bomns ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})

# Editor & launcher shit
add_subdirectory("src/editor")
add_subdirectory("src/launcher")

# Install shit
file(GLOB bomns_GFX "graphics/*.bmp")
file(GLOB bomns_SFX "sounds/*.wav")
file(GLOB bomns_LVL "levels/*.lvl")
install(TARGETS bomns DESTINATION "bin")
install(FILES ${bomns_GFX} DESTINATION "share/bomns/graphics")
install(FILES ${bomns_SFX} DESTINATION "share/bomns/sounds")
install(FILES ${bomns_LVL} DESTINATION "share/bomns/levels")
install(FILES "bomns.desktop" DESTINATION "share/applications")
install(DIRECTORY "icons" DESTINATION "share")
