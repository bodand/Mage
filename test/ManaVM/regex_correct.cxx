/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/regex_correct.cxx --
 *   Tests that the ManaVM regex wrapper accepts correct regexes.
 */

#include <exception>
#include <iostream>

#include <mana/regex.hxx>

int
main() {
    std::string_view regex = R"(^4.+)";

    try {
        [[maybe_unused]] mana::regex re(regex);
    } catch (const std::exception& ex) {
        std::cerr << "regex '" << regex << "' failed: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
