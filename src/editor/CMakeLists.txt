set(
  EDITOR_SOURCES
  cursor.cpp
  editor.cpp
  hud.cpp
  level.cpp
  object.cpp
)

add_executable(bomnsedit ${EDITOR_SOURCES})
target_link_libraries(bomnsedit ${SDL_LIBRARY})

install(TARGETS bomnsedit DESTINATION "bin")
