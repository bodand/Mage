/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/regex_match.cxx --
 *   Test case for checking whether the the regex wrapper correctly reports
 *   matches.
 */

#include <cstdlib>
#include <iostream>
#include <string_view>

#include <mana/regex.hxx>

int
main() {
    std::string_view regex = R"(^(a(?:(?1)|\|)b)$)";

    try {
        [[maybe_unused]] mana::regex re(regex);

        if (re.match("aaa|bbb")
            && !re.match("aa|b")) return EXIT_SUCCESS;
        return EXIT_FAILURE;

    } catch (const std::exception& ex) {
        std::cerr << "regex '" << regex << "' failed: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
}
