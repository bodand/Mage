# Mage project
#
# Copyright (c) 2021, Bodor András <bodand@pm.me>
# Licensed under the BSD 3-Clause license
#
# CMakeLists.txt --
#   This auxilary CMake build system is responsible for handling the
#   building of Mage on systems where Mage itself is not yet present.

cmake_minimum_required(VERSION 3.20)

## CMake system setup ##########################################################
## Languages ##
enable_language(CXX)

## Policies ##
cmake_policy(SET CMP0009 NEW) # Do not follow symlinks in GLOB_RECURSE
cmake_policy(SET CMP0048 NEW) # project sets VERSION vars
cmake_policy(SET CMP0083 NEW) # PIE
cmake_policy(SET CMP0091 NEW) # MSVC does not set the runtime library in CMAKE_<LANG>_FLAGS
cmake_policy(SET CMP0092 NEW) # MSVC does not set the default /W3 in CMAKE_<LANG>_FLAGS
cmake_policy(SET CMP0110 NEW) # add_test handles whitespace it test's name

## Modules #####################################################################
include(CMakePackageConfigHelpers)
include(CMakePushCheckState)
include(CheckCXXSymbolExists)
include(CheckIPOSupported)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckPIESupported)
include(CheckStructHasMember)
include(CheckSymbolExists)
include(CheckTypeSize)
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)

## System capability checks ####################################################
# Position independent code
check_pie_supported(LANGUAGES CXX)
# LTO
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DSH_HAVE_LTO OFF CACHE INTERNAL "" FORCE)
else ()
    check_ipo_supported(RESULT DSH_HAVE_LTO
                        LANGUAGES CXX)
endif ()

## Project #####################################################################
project(Mage
        VERSION 0.1.0
        DESCRIPTION "The Mage build system"
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

## Options #####################################################################
option(MAGE_BUILD_TESTS
       "Build Mage and ManaVM tests [ON]" ON)
option(MAGE_BUILD_DOCS
       "Build Mage and ManaVM tests [ON]" ON)

## Targets #####################################################################
## Warnings ##
#include(cmake/warnings.cmake)

## Platform specific file component
if (WIN32)
    set(MAGE_PLATFORM_FILE_COMPONENT3 win32)
    set(MAGE_PLATFORM_FILE_COMPONENT2 win32)
elseif (APPLE)
    set(MAGE_PLATFORM_FILE_COMPONENT3 macos)
    set(MAGE_PLATFORM_FILE_COMPONENT2 posix)
else ()
    set(MAGE_PLATFORM_FILE_COMPONENT3 posix)
    set(MAGE_PLATFORM_FILE_COMPONENT2 posix)
endif ()

## Dependencies ##
find_package(Boost REQUIRED)
add_subdirectory(lib/PCRE2)
add_subdirectory(lib/SQLite)

## ManaVM ##
add_subdirectory(lib/ManaVM)

## Mage ##
add_executable(mage
               src/main.cxx)
target_link_libraries(mage ManaVM)

## Tests #######################################################################
if (MAGE_BUILD_TESTS)
    include(CTest)
    add_subdirectory(test)
endif ()

## Docs ########################################################################
if (MAGE_BUILD_DOCS)
    add_subdirectory(docs)
endif ()
