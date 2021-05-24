/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/value.cxx --
 *   Implements the ManaVM value object's functionality.
 */

#include <mana/value.hxx>
#include <mana/value/int_value.hxx>
#include <mana/value/sym_value.hxx>
#include <mana/value/bool_value.hxx>
#include <mana/visitors/equality_visitor.hxx>

std::unique_ptr<mana::value>
mana::value::make_value(std::int64_t i) {
    return std::make_unique<int_value>(i);
}

std::unique_ptr<mana::value>
mana::value::make_value() {
    return std::make_unique<sym_value>();
}

std::unique_ptr<mana::value>
mana::value::make_value(logical b) {
    return std::make_unique<bool_value>(b);
}

bool
mana::operator==(const value& v1, const value& v2) noexcept {
    const equality_visitor eq(&v2);
    return v1.accept_visitor(&eq) != nullptr;
}
