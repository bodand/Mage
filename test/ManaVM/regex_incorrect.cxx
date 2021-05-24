/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/regex_incorrect.cxx --
 *   Tests that the ManaVM regex wrapper rejects incorrect regexes.
 */

#include <exception>
#include <iostream>

#include <mana/regex.hxx>

int
main() {
    std::string_view regex = R"(+$)";

    try {
        [[maybe_unused]] mana::regex re(regex);
    } catch (const std::exception& ex) {
        return EXIT_SUCCESS;
    }

    std::cerr << "regex '" << regex << "' did not fail\n";
    return EXIT_FAILURE;
}
