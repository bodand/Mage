/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/value_create_int.cxx --
 *   Tests if value::make_value properly creates int_value.
 */

#include <mana/value.hxx>
#include <mana/value/bool_value.hxx>

int
main() {
    mana::bool_value bt1(true), bt2(true);
    mana::bool_value bf1(false), bf2(false);

    return bt1 != bt2 || bf1 != bf2;
}
