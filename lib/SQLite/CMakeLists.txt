# Mage project
#
# Copyright (c) 2021, Bodor András <bodand@pm.me>
# Licensed under the BSD 3-Clause license
#
# lib/SQLite/CMakeLists.txt --
#   Handles the dependency on SQLite for the cache engine. Does the required
#   system checks for a full SQLite build and downloads the amalgamated sources
#   to build an OBJECT library available as ManaSQL.

# SQLite is C
enable_language(C)

check_include_file("dlfcn.h"     HAVE_DLFCN_H)
check_include_file("inttypes.h"  HAVE_INTTYPES_H)
check_include_file("malloc.h"    HAVE_MALLOC_H)
check_include_file("memory.h"    HAVE_MEMORY_H)
check_include_file("stdint.h"    HAVE_STDINT_H)
check_include_file("strings.h"   HAVE_STRINGS_H)
check_include_file("sys/stat.h"  HAVE_SYS_STAT_H)
check_include_file("sys/types.h" HAVE_SYS_TYPES_H)
check_include_file("unistd.h"    HAVE_UNISTD_H)

check_include_files("sys/types.h;time.h"  HAVE_POSIX_TIME_H)
check_include_files("sys/types.h;utime.h" HAVE_UTIME_H)

foreach(int_sz 8 16 32 64 ptr)
    check_type_size("int${int_sz}_t" "INT${int_sz}_T")
    check_type_size("uint${int_sz}_t" "UINT${int_sz}_T")
endforeach()

## fdatasync(), usleep() #######################################################
#  - unistd.h
################################################################################
if (HAVE_UNISTD_H)
    check_symbol_exists("fdatasync" "unistd.h" HAVE_FDATASYNC)
    check_symbol_exists("pread"     "unistd.h" HAVE_PREAD)
    check_symbol_exists("pread64"   "unistd.h" HAVE_PREAD64)
    check_symbol_exists("pwrite"    "unistd.h" HAVE_PWRITE)
    check_symbol_exists("pwrite64"  "unistd.h" HAVE_PWRITE64)
    check_symbol_exists("usleep"    "unistd.h" HAVE_USLEEP)
else ()
    set(HAVE_FDATASYNC FALSE CACHE BOOL "do not have unistd.h")
    set(HAVE_USLEEP    FALSE CACHE BOOL "do not have unistd.h")
endif ()

## gmtime_r(), localtime_r() ###################################################
#  - sys/types.h
#  - time.h
################################################################################
if (HAVE_POSIX_TIME_H)
    check_symbol_exists("gmtime_r"    "sys/types.h;time.h" HAVE_GMTIME_R)
    check_symbol_exists("localtime_r" "sys/types.h;time.h" HAVE_LOCALTIME_R)
else ()
    set(HAVE_GMTIME_R    FALSE CACHE BOOL "do not have sys/types.h, time.h")
    set(HAVE_LOCALTIME_R FALSE CACHE BOOL "do not have sys/types.h, time.h")
endif ()

## utime() #####################################################################
#  - sys/types.h
#  - utime.h
################################################################################
if (HAVE_UTIME_H)
    check_symbol_exists("utime" "sys/types.h;utime.h" HAVE_GMTIME_R)
else ()
    set(HAVE_UTIME FALSE CACHE BOOL "do not have sys/types.h, utime.h")
endif ()

## malloc_usable_size() ########################################################
#  - malloc.h
################################################################################
if (HAVE_MALLOC_H)
    check_symbol_exists("malloc_usable_size" "malloc.h" HAVE_MALLOC_USABLE_SIZE)
else ()
    set(HAVE_MALLOC_USABLE_SIZE FALSE CACHE BOOL "do not have malloc.h")
endif ()

## isnan() #####################################################################
check_symbol_exists("isnan" "math.h" HAVE_ISNAN)

## localtime_s() ###############################################################
check_symbol_exists("localtime_s" "time.h" HAVE_LOCALTIME_S) # Windows

## strchrnul ###################################################################
#  - _GNU_SOURCE
################################################################################
cmake_push_check_state()
list(APPEND CMAKE_REQUIRED_DEFINITIONS "-D_GNU_SOURCE=1")

check_symbol_exists("strchrnul" "string.h" HAVE_STRCHRNUL)

cmake_pop_check_state()

## Endianness test #############################################################
if (CMAKE_C_BYTE_ORDER STREQUAL "BIG_ENDIAN")
    set(SQLITE_BYTEORDER "4321" CACHE STRING "Byte order of machine for SQLite (set to big-endian)")
elseif (CMAKE_C_BYTE_ORDER STREQUAL "LITTLE_ENDIAN")
    set(SQLITE_BYTEORDER "1234" CACHE STRING "Byte order of machine for SQLite (set to little-endian)")
else()
    set(SQLITE_BYTEORDER "0" CACHE STRING "Byte order of machine for SQLite (set to runtime check)")
endif ()

# We always assume ANSI C support ##############################################
set(STDC_HEADERS  1 CACHE INTERNAL "ansi C support assumed")
set(HAVE_STDLIB_H 1 CACHE INTERNAL "ansi C support assumed")
set(HAVE_STRING_H 1 CACHE INTERNAL "ansi C support assumed")

configure_file(config.h.in "sqlite-src/config.h")

## Get sources #################################################################
set(MANA_SQLITE_VERSION_MAJOR 3)
set(MANA_SQLITE_VERSION_MINOR 36)
set(MANA_SQLITE_VERSION_PATCH 00)
set(MANA_SQLITE_VERSION "${MANA_SQLITE_VERSION_MAJOR}.${MANA_SQLITE_VERSION_MINOR}.${MANA_SQLITE_VERSION_PATCH}")
set(MANA_SQLITE_AMAL_VERSION sqlite-amalgamation-${MANA_SQLITE_VERSION_MAJOR}${MANA_SQLITE_VERSION_MINOR}${MANA_SQLITE_VERSION_PATCH}00)

message(STATUS "Downloading SQLite: https://sqlite.org/2021/${MANA_SQLITE_AMAL_VERSION}.zip")
file(DOWNLOAD "https://sqlite.org/2021/${MANA_SQLITE_AMAL_VERSION}.zip"
     "${CMAKE_CURRENT_BINARY_DIR}/sqlite.zip"
     TLS_VERIFY ON
     EXPECTED_HASH SHA3_256=d25609210ec93b3c8c7da66a03cf82e2c9868cfbd2d7d866982861855e96f972)
file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_BINARY_DIR}/sqlite.zip")
file(COPY
     "${CMAKE_CURRENT_BINARY_DIR}/${MANA_SQLITE_AMAL_VERSION}/sqlite3ext.h"
     "${CMAKE_CURRENT_BINARY_DIR}/${MANA_SQLITE_AMAL_VERSION}/sqlite3.h"
     "${CMAKE_CURRENT_BINARY_DIR}/${MANA_SQLITE_AMAL_VERSION}/sqlite3.c"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/sqlite-src")

add_library(SQLite OBJECT
            "${CMAKE_CURRENT_BINARY_DIR}/sqlite-src/sqlite3.c")
target_include_directories(SQLite PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/sqlite-src>
                           )
target_compile_definitions(SQLite PRIVATE _HAVE_SQLITE_CONFIG_H=1)
add_library(ManaSQL ALIAS SQLite)
