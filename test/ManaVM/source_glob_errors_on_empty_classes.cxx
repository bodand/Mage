/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM//source_glob_errors_on_empty_classes.cxx --
 *   Checks if source_glob throws on [] as expected, but not on []], as
 *   expected.
 */

#include <iostream>

#include <mana/source_scanner.hxx>

int
main() {
    auto ret = EXIT_SUCCESS;

    try {
        mana::source_glob bad_glob("[]", {});
        (void) bad_glob();
        ret = EXIT_FAILURE;
    } catch (const mana::bad_glob& ex) {
        std::cerr << "expected error occurred:\n";
        std::cerr << ex.what() << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "unexpected error occurred:\n";
        ret = EXIT_FAILURE;
    }

    try {
        mana::source_glob glob1("[]]", {});
        mana::source_glob glob2("[]x]", {});

        (void) glob1();
        (void) glob2();
    } catch (const std::exception& ex) {
        std::cerr << "unexpected error occurred:\n";
        ret = EXIT_FAILURE;
    }
    return ret;
}
