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
    auto iv = mana::value::make_value(mana::logical::yes);
    return !dynamic_cast<mana::bool_value*>(iv.get())->_value;
}
