/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/regex_capture_idx.cxx --
 *   Tests if the capture groups in regexes can be properly extracted through
 *   the ManaVM abstraction layer by indexes.
 */


#include <cstdlib>
#include <iostream>
#include <string_view>

#include <mana/regex.hxx>

int
main() {
    std::string_view regex = R"(^a:\s(\d+),\s*b:\s(\d+),\s*c:\s(\d+))";

    try {
        [[maybe_unused]] mana::regex re(regex);

        auto mch = re.match("a: 42, b: 420, c: 69");

        auto [matched_0, val_0] = mch[1];
        if (!matched_0 || val_0 != "42") {
            std::cerr << "capture group 1 didn't match or was not 42\n";
            return EXIT_FAILURE;
        }

        auto [matched_1, val_1] = mch[2];
        if (!matched_1 || val_1 != "420") {
            std::cerr << "capture group 2 didn't match or was not 420\n";
            return EXIT_FAILURE;
        }

        auto [matched_2, val_2] = mch[3];
        if (!matched_2 || val_2 != "69") {
            std::cerr << "capture group 3 didn't match or was not 69\n";
            return EXIT_FAILURE;
        }

    } catch (const std::exception& ex) {
        std::cerr << "regex '" << regex << "' failed: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
}
