/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/source_glob_properly_dissects_relative_glob.cxx --
 *   Tests whether the mana::source_glob can deal with relative glob paths like
 *   *.txt, and a/foo* by figuring out the directory to glob and the glob
 *   expression itself. (pairs {$cwd, *.txt} and {$cwd/a, foo*} respectively)
 */

#include <iostream>

#include <mana/source_scanner.hxx>

int
main() {
    int ret = EXIT_SUCCESS;
    { // SECTION 1: *.txt
        auto glob = mana::source_glob("*.txt", {});
        if (glob.glob != "*.txt") {
            std::cerr << "error(*.txt): glob != '*.txt'\n\twas: " << glob.glob << '\n';
            ret = EXIT_FAILURE;
        }
        if (glob.in_dir != std::filesystem::current_path()) {
            std::cerr << "error(*.txt): in_dir != $cwd\n\twas: " << glob.in_dir << '\n';
            ret = EXIT_FAILURE;
        }
    }
    { // SECTION 1: a/foo*
        auto glob = mana::source_glob("a/foo*", {});
        if (glob.glob != "foo*") {
            std::cerr << "error(a/foo*): glob != 'foo*'\n\twas: " << glob.glob << '\n';
            ret = EXIT_FAILURE;
        }
        if (glob.in_dir != std::filesystem::current_path() / "a") {
            std::cerr << "error(a/foo*): in_dir != $cwd/a\n\twas: " << glob.in_dir << '\n';
            ret = EXIT_FAILURE;
        }
    }
    return ret;
}
