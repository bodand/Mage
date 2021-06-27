# Mage project
#
# Copyright (c) 2021, Bodor András <bodand@pm.me>
# Licensed under the BSD 3-Clause license
#
# test/CMakeLists.txt --
#   Provides the drivers for using CTest with the MageTest test suite.

include(../cmake/MageTest.cmake)

## Tests #######################################################################
### RegEx ###
MageTest("ManaVM regex accepts correct regex"
         ManaVM/regex_correct.cxx)
MageTest("ManaVM regex does not accept incorrect regex"
         ManaVM/regex_incorrect.cxx)
MageTest("ManaVM regexes return match results correctly"
         ManaVM/regex_match.cxx)
MageTest("ManaVM regexes allow extracting capture groups by index"
         ManaVM/regex_capture_idx.cxx)
MageTest("ManaVM regexes allow extracting capture groups by name"
         ManaVM/regex_capture_name.cxx)
MageTest("ManaVM regexes iterate through match with all capturing groups filled"
         ManaVM/regex_capture_full_iterator.cxx)
MageTest("ManaVM regexes iterate through match without all capturing groups filled"
         ManaVM/regex_capture_holey_iterator.cxx)

### Value ###
MageTest("ManaVM value::make_value can properly create symbol value"
         ManaVM/value_create_sym.cxx)
MageTest("ManaVM value::make_value can properly create integer value"
         ManaVM/value_create_int.cxx)
MageTest("ManaVM value::make_value can properly create boolean value"
         ManaVM/value_create_bool.cxx)
MageTest("ManaVM sym_value checks equality properly"
         ManaVM/value_eq_sym.cxx)
MageTest("ManaVM int_value checks equality properly"
         ManaVM/value_eq_int.cxx)
MageTest("ManaVM bool_value checks equality properly"
         ManaVM/value_eq_bool.cxx)
MageTest("ManaVM values can check equality through references"
         ManaVM/value_universal_equality.cxx)

### Source Scan ###
MageTest("ManaVM source_glob can dissect relative glob expressions into path and glob"
         ManaVM/source_glob_properly_dissects_relative_glob.cxx)
MageTest("ManaVM source_glob can dissect absolute glob expressions into path and glob"
         ManaVM/source_glob_properly_dissects_absolute_glob.cxx)
MageTest("ManaVM source_glob can find fixed suffix searching globs"
         ManaVM/source_glob_finds_globbing_on_suffixes.cxx)
MageTest("ManaVM source_glob can find fixed stem searching globs"
         ManaVM/source_glob_finds_globbing_on_stem.cxx)
MageTest("ManaVM source_glob can handle '*.?xx'"
         ManaVM/source_glob_finds_globbing_on_complexish_suffixes.cxx)
MageTest("ManaVM source_glob errors on empty classes '[]'"
         ManaVM/source_glob_errors_on_empty_classes.cxx)
MageTest("ManaVM source_glob * auto-escapes following meta"
         ManaVM/source_glob_star_autoescape_happens_as_docs_say.cxx)
MageTest("ManaVM source_glob globs according to directory depth"
         ManaVM/source_glob_globs_directory_depth_correctly.cxx)
#MageTest("ManaVM source_scanner remembers target source glob"
#         ManaVM/source_scanner_remembers_target_source_glob.cxx)

foreach (file a.txt b.txt a.cxx
              foobar.txt barfoo.txt
              d.hxx foo.d
              foo/a.txt bar/a.txt)
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/glob-test/${file}"
         [[/* THIS FILE IS USED FOR TESTING GLOBBING - DO NOT REMOVE OR RENAME */]])
endforeach ()
