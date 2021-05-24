/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/regex_capture_holey_iterator.cxx --
 *   Checks if iterating through a match_result using the provided iterators
 *   provides the expected values. The match_result contains capturing groups
 *   which didn't participate in the match.
 */

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string_view>

#include <mana/regex.hxx>

int
main() {
    std::string_view regex = R"(^a:\s(\d+),\s*(?:b:\s(\d+),\s*)?c:\s(\d+))";

    try {
        [[maybe_unused]] mana::regex re(regex);

        auto mch = re.match("a: 42, c: 69");
        auto expected = std::vector{
               std::string_view("a: 42, c: 69"),
               std::string_view("42"),
               std::string_view("69"),
        };
        auto ret = EXIT_SUCCESS;
        auto exp = std::begin(expected);

        for (auto m = std::begin(mch),
                  e = std::end(mch);
             m != e;
             ++m) {
            if (*m != *exp) {
                std::cerr << "mismatch in iterator values: '" << *m << "' != '" << *exp << "'\n";
                ret = EXIT_FAILURE;
            }
            ++exp;
        }

        return ret;
    } catch (const std::exception& ex) {
        std::cerr << "regex '" << regex << "' failed: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
}
