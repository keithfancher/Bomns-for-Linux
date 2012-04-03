set(
  LAUNCHER_SOURCES
  main.c
  support.c
  interface.c
  callbacks.c
)

find_package(GTK2 2.0 REQUIRED gtk)

include_directories(${GTK2_INCLUDE_DIRS})
add_executable(bomnslauncher ${LAUNCHER_SOURCES})
target_link_libraries(bomnslauncher ${GTK2_LIBRARIES})

install(TARGETS bomnslauncher DESTINATION "bin")
