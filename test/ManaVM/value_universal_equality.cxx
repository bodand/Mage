/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * /value_universal_equality.cxx --
 *   DOCS WIP
 */

#include <iostream>

#include <mana/value.hxx>

int
main() {
    auto retval = EXIT_SUCCESS;

    // eq type, eq value
    {
        auto i1 = mana::value::make_value(42);
        auto i2 = mana::value::make_value(42);
        if (*i1 != *i2) {
            std::cerr << "i1[Int](42) didn't test equal to i2[Int](42)\n";
            retval = EXIT_FAILURE;
        }
    }

    // eq type, diff value
    {
        auto b1 = mana::value::make_value(mana::logical::yes);
        auto b2 = mana::value::make_value(mana::logical::no);
        if (*b1 == *b2) {
            std::cerr << "b1[Bool](true) tested equal to b2[Bool](false)\n";
            retval = EXIT_FAILURE;
        }
    }

    // diff type
    {
        auto b = mana::value::make_value(mana::logical::yes);
        auto s = mana::value::make_value();
        if (*b == *s) {
            std::cerr << "b[Bool](true) tested equal to s[Symbol]()\n";
            retval = EXIT_FAILURE;
        }
    }

    return retval;
}
