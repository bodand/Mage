# Mage project
#
# Copyright (c) 2021, Bodor András <bodand@pm.me>
# Licensed under the BSD 3-Clause license
#
# lib/ManaVM/CMakeLists.txt --
#   Builds the ManaVM, the backend powering Mage. This handles the low level
#   details of Mage, like calling the compiler, performing regex matches, or
#   unpacking archives;
#   anything really, since Chalk is mostly just a macro language. Basically Mage
#   just "interprets" the Chalk source into ManaVM function calls.

project(ManaVM
        VERSION 0.1.0
        DESCRIPTION "The Mage backend \"virtual machine\""
        LANGUAGES CXX
        )

## Targets #####################################################################
add_library(ManaVM SHARED
            src/mana/regex.cxx src/mana/value.cxx src/mana/value/int_value.cxx
            src/mana/value/sym_value.cxx src/mana/value/bool_value.cxx
            src/mana/visitors/equality_visitor.cxx src/mana/db.cxx
            src/mana/visitors/basic_visitor.cxx
            src/mana/source_scanner.cxx)
target_include_directories(ManaVM PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

## Dependencies ################################################################
target_link_libraries(ManaVM PRIVATE pcre2-8 pcre2-posix ManaSQL)
