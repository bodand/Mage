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
    auto iv = mana::value::make_value(42);
    return dynamic_cast<mana::int_value*>(iv.get())->_value != 42;
}
