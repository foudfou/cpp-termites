cmake_minimum_required(VERSION 2.6)

project(Termites)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/${CMAKE_BUILD_TYPE}) # executables are put here

# create dirs
file(
  MAKE_DIRECTORY
  bin
  )

# Configuration des sources
set(TERMITES_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(
  GLOB
  source_files
  ${TERMITES_SRC_PATH}/*.cpp
  ${TERMITES_SRC_PATH}/*.hpp
  )

add_executable(
  termites
  ${source_files}
)


# distclean
add_custom_target (distclean @echo cleaning source distribution)
set(DISTCLEANED
  CMakeFiles
  Makefile
  cmake_install.cmake
  CMakeCache.txt
  CTestTestfile.cmake
  Testing
  bin
  build
  lib
  doc
  install_manifest.txt
  )

add_custom_command(
  DEPENDS clean
  COMMENT "distribution clean"
  COMMAND rm
  ARGS    -rf CMakeTmp ${DISTCLEANED}
  TARGET  distclean
  )
