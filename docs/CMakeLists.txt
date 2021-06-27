# Mage project
#
# Copyright (c) 2021, Bodor András <bodand@pm.me>
# Licensed under the BSD 3-Clause license
#
# docs/CMakeLists.txt --
#   Builds the Mage and ManaVM documentation from Mana::Doc format to HTML.

find_program(MANADOC_EXE "manadoc")

set(DOC_SRC mana_glob.manadoc mana_macros.manadoc mana_regex.manadoc)
set(_DOC_OBJ)

foreach (src IN LISTS DOC_SRC)
    set(abs_src "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    cmake_path(GET abs_src STEM src_stem)
    set(OUT "${CMAKE_CURRENT_BINARY_DIR}/${src_stem}.html")
    list(APPEND _DOC_OBJ "${OUT}")
    add_custom_command(
            COMMENT "Compiling ${src} Mana::Doc into HTML"
            OUTPUT "${OUT}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${src}"
            COMMAND "${MANADOC_EXE}"
                    -T HTML
                    -o ${OUT}
                    "${CMAKE_CURRENT_SOURCE_DIR}/${src}"
    )
endforeach ()

add_custom_target(build-docs
                  DEPENDS ${_DOC_OBJ}
                  )
