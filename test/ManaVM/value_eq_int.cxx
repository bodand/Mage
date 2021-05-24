/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/value_create_int.cxx --
 *   Tests if value::make_value properly creates int_value.
 */

#include <mana/value.hxx>
#include <mana/value/int_value.hxx>

int
main() {
    mana::int_value i1(42), i2(42), i3(420);

    return i1 != i2 || i1 == i3;
}
