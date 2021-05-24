# Mage project 
# 
# Copyright (c) 2021, Bodor András <bodand@pm.me>
# Licensed under the BSD 3-Clause license
# 
# lib/PCRE2/CMakeLists.txt --
#   Handles the dependency on PCRE2 for the regex engine. If the system already
#   has a PCRE2 which CMake is capable of finding, it uses that, otherwise
#   dowloads sources and builds them locally.

## GetDependency
if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/GetDependency.cmake")
    file(DOWNLOAD
         https://raw.githubusercontent.com/bodand/GetDependency/master/cmake/GetDependency.cmake
         "${CMAKE_CURRENT_BINARY_DIR}/GetDependency.cmake"
         )
endif ()
include("${CMAKE_CURRENT_BINARY_DIR}/GetDependency.cmake")

set (PCRE2_SHOW_REPORT ON CACHE BOOL "" FORCE)
set (PCRE2_BUILD_PCRE2GREP OFF CACHE BOOL "" FORCE)
set (PCRE2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set (PCRE2_SUPPORT_JIT ON CACHE BOOL "" FORCE)

message(CHECK_START "[${PROJECT_NAME}] 'PCRE2'")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
GetDependency(PCRE2
              REPOSITORY_URL svn://vcs.pcre.org/pcre2/code/trunk
              VERSION -r1288
              )
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
