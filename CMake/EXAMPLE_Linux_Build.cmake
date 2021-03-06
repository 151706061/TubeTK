#############################################################################
#
# Library:   TubeTK
#
# Copyright 2010 Kitware Inc. 28 Corporate Drive,
# Clifton Park, NY, 12065, USA.
#
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#############################################################################

#############################################################################
#
# Configure the following variables and move this file to the directory above
#   the tubetk source directory.
#
set( SITE_NAME "Eternia.Kitware" )
set( SITE_PLATFORM "Ubuntu-12.04-64" )
set( SITE_BUILD_TYPE "Debug" )
set( SITE_CMAKE_GENERATOR "Unix Makefiles" )

set( TUBETK_GIT_REPOSITORY "http://tubetk.org/TubeTK.git" )
set( TUBETK_SOURCE_DIR "/Users/aylward/src/test/TubeTK" )
set( TUBETK_BINARY_DIR "/Users/aylward/src/test/TubeTK-${SITE_BUILD_TYPE}" )

set( ENV{DISPLAY} ":0" )

set( SITE_MAKE_COMMAND "make -j3" )
set( SITE_CMAKE_COMMAND "/usr/bin/cmake" )
set( SITE_QMAKE_COMMAND "/Users/aylward/QtSDK/Desktop/Qt/474/gcc/bin/qmake" )
set( SITE_CTEST_COMMAND "/usr/bin/ctest -j3" )

set( SITE_GIT_COMMAND "/usr/bin/git" )
set( SITE_SVN_COMMAND "/usr/bin/svn" )
#############################################################################

#############################################################################
#
# The following advanced variables should only be changed by experts
#
set( TUBETK_SCRIPT_DIR "${TUBETK_SOURCE_DIR}/CMake" )

set( SITE_CTEST_MODE "Experimental" )

set( SITE_BUILD_NAME "${SITE_PLATFORM}-${SITE_BUILD_TYPE}" )

set( SITE_UPDATE_COMMAND "${SITE_GIT_COMMAND}" )

set( CTEST_SITE "${SITE_NAME}" )

set( CTEST_BUILD_CONFIGURATION "${SITE_BUILD_TYPE}" )
set( CTEST_BUILD_COMMAND "${SITE_MAKE_COMMAND}" )

set( CTEST_SOURCE_DIRECTORY "${TUBETK_SOURCE_DIR}" )
set( CTEST_BINARY_DIRECTORY "${TUBETK_BINARY_DIR}/TubeTK-Build" )

set( CTEST_CMAKE_GENERATOR "${SITE_CMAKE_GENERATOR}" )
set( CTEST_TEST_TIMEOUT 1500 )

set( CTEST_CMAKE_COMMAND "${SITE_CMAKE_COMMAND}" )
set( CTEST_CTEST_COMMAND "${SITE_CTEST_COMMAND}" )
set( CTEST_UPDATE_COMMAND "${SITE_UPDATE_COMMAND}" )
set( CTEST_COMMAND "${SITE_CTEST_COMMAND}" )

set( SITE_EXECUTABLE_DIRS "${SITE_KWSTYLE_DIR}" )
set( ENV{PATH} "${SITE_EXECUTABLE_DIRS}:$ENV{PATH}" )

set( SITE_CXX_FLAGS
  "-fPIC -fdiagnostics-show-option -W -Wall -Wextra -Wshadow -Wno-system-headers -Wwrite-strings -Wno-deprecated -Woverloaded-virtual" )
set( SITE_C_FLAGS
  "-fPIC -fdiagnostics-show-option -W -Wall -Wextra -Wshadow -Wno-system-headers -Wwrite-strings" )
set( SITE_EXE_LINKER_FLAGS "" )
set( SITE_SHARED_LINKER_FLAGS "" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SITE_C_FLAGS}" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SITE_CXX_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS
  "${CMAKE_EXE_LINKER_FLAGS} ${SITE_EXE_LINKER_FLAGS}" )
set( CMAKE_SHARED_LINKER_FLAGS
  "${CMAKE_SHARED_LINKER_FLAGS} ${SITE_SHARED_LINKER_FLAGS}" )

set( GITCOMMAND "${SITE_GIT_COMMAND}" )
set( GIT_EXECUTABLE "${SITE_GIT_COMMAND}" )
set( SVNCOMMAND "${SITE_SVN_COMMAND}" )
set( CMAKE_GENERATOR ${SITE_CMAKE_GENERATOR} )
set( QT_QMAKE_EXECUTABLE "${SITE_QMAKE_COMMAND}" )

if( NOT EXISTS "${TUBETK_SOURCE_DIR}/CMakeLists.txt" )
  execute_process( COMMAND
    "${SITE_GIT_COMMAND}"
    clone "${TUBETK_GIT_REPOSITORY}" "${TUBETK_SOURCE_DIR}" )
  ctest_run_script()
else()
  set( CTEST_BUILD_NAME "${SITE_BUILD_NAME}-BuildTest-New" )
  configure_file(
    ${TUBETK_SCRIPT_DIR}/InitCMakeCache.cmake.in
    ${TUBETK_BINARY_DIR}/InitCMakeCache.cmake IMMEDIATE @ONLY )
  set( CTEST_NOTES_FILES "${TUBETK_BINARY_DIR}/InitCMakeCache.cmake" )

  ctest_start( "Experimental" )
  ctest_update( SOURCE "${TUBETK_SOURCE_DIR}" )
  ctest_configure( BUILD "${TUBETK_BINARY_DIR}"
    SOURCE "${TUBETK_SOURCE_DIR}"
    OPTIONS "-C${TUBETK_BINARY_DIR}/InitCMakeCache.cmake" )
  ctest_read_custom_files( "${TUBETK_BINARY_DIR}" )
  ctest_build( BUILD "${TUBETK_BINARY_DIR}" )
  ctest_test( BUILD "${TUBETK_BINARY_DIR}/TubeTK-Build" )
  ctest_submit( PARTS Notes Update Configure Build Test )
endif()

set(CTEST_RUN_CURRENT_SCRIPT 0)
