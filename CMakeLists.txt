cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(wolfssh VERSION 1.4.15 LANGUAGES C)

add_library(wolfssh-obj OBJECT
  src/agent.c wolfssh/agent.h
  src/certman.c wolfssh/certman.h
  src/internal.c wolfssh/internal.h
  src/keygen.c wolfssh/keygen.h
  src/log.c wolfssh/log.h
  src/misc.c wolfssh/misc.h
  src/port.c wolfssh/port.h
  src/ssh.c wolfssh/ssh.h
  src/wolfscp.c wolfssh/wolfscp.h
  src/wolfsftp.c wolfssh/wolfsftp.h
  src/io.c
  src/wolfterm.c
  wolfssh/certs_test.h
  wolfssh/error.h
  wolfssh/settings.h
  wolfssh/test.h
  wolfssh/version.h
  wolfssh/visibility.h
)
target_include_directories(wolfssh-obj PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_library(wolfssh-shared SHARED $<TARGET_OBJECTS:wolfssh-obj>)
set_target_properties(wolfssh-shared PROPERTIES OUTPUT_NAME "wolfssh")

add_library(wolfssh-static STATIC $<TARGET_OBJECTS:wolfssh-obj>)
set_target_properties(wolfssh-static PROPERTIES OUTPUT_NAME "wolfssh")
