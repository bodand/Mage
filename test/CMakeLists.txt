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
