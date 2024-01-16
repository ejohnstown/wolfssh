cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(wolfssh VERSION 1.4.15 LANGUAGES C)

option(BUILD_SHARED_LIBS "Build shared library" OFF)

find_package(PkgConfig REQUIRED QUIET)

pkg_search_module(wolfSSL REQUIRED IMPORTED_TARGET wolfssl)
if(TARGET PkgConfig::wolfSSL)
  message(STATUS "Found wolfssl")
endif()

add_library(wolfssh-obj OBJECT
  src/agent.c wolfssh/agent.h
  src/certman.c wolfssh/certman.h
  src/internal.c wolfssh/internal.h
  src/keygen.c wolfssh/keygen.h
  src/log.c wolfssh/log.h
  src/port.c wolfssh/port.h
  src/ssh.c wolfssh/ssh.h
  src/wolfscp.c wolfssh/wolfscp.h
  src/wolfsftp.c wolfssh/wolfsftp.h
  src/io.c
  src/wolfterm.c
  wolfssh/certs_test.h
  wolfssh/error.h
  wolfssh/misc.h
  wolfssh/settings.h
  wolfssh/test.h
  wolfssh/version.h
  wolfssh/visibility.h
)
set_target_properties(wolfssh-obj PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_include_directories(wolfssh-obj PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_library(wolfssh $<TARGET_OBJECTS:wolfssh-obj>)
set_target_properties(wolfssh PROPERTIES OUTPUT_NAME "wolfssh")
