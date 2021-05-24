# Mage project
#
# Copyright (c) 2021, Bodor András <bodand@pm.me>
# Licensed under the BSD 3-Clause license
#
# cmake/MageTest.txt --
#   Provides a CMake macro for defining a test case usable in CTest directly from
#   source file(s).

set(_MANAVM_LIBNAME "${CMAKE_SHARED_LIBRARY_PREFIX}ManaVM${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_MANAVM_LIBNAME}
                   DEPENDS ManaVM
                   COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:ManaVM>" "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(MageTestRuntime
                  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_MANAVM_LIBNAME})

macro(MageTest mt_Name)
    if (NOT DEFINED MT_SUFFIX)
        set (MT_SUFFIX 1)
    else ()
        math(EXPR MT_SUFFIX "${MT_SUFFIX} + 1")
    endif ()
    set(MT_EXECUTABLE MageTest-${MT_SUFFIX})

    add_executable("${MT_EXECUTABLE}"
                   ${ARGN})
    add_test(NAME "${mt_Name}"
             COMMAND "${MT_EXECUTABLE}")
    target_link_libraries("${MT_EXECUTABLE}" ManaVM)
    add_dependencies("${MT_EXECUTABLE}" MageTestRuntime)
endmacro()
