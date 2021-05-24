/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/value_create_int.cxx --
 *   Tests if value::make_value properly creates int_value.
 */

#include <mana/value.hxx>
#include <mana/value/sym_value.hxx>

int
main() {
    mana::sym_value s1, s2;

    return s1 != s1 || s1 == s2;
}
