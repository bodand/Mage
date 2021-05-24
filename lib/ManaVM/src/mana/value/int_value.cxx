/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/value/int_value.cxx --
 *   Implements mana::int_value.
 */

#include <mana/value/int_value.hxx>
#include <mana/visitors/basic_visitor.hxx>

mana::int_value::int_value(std::int64_t i) noexcept
     : _value(i) { }

[[nodiscard]] bool
mana::operator==(const mana::int_value& i1, const mana::int_value& i2) noexcept {
    return i1._value == i2._value;
}

[[nodiscard]] bool
mana::operator==(std::int64_t i1, const mana::int_value& i2) noexcept {
    return i1 == i2._value;
}

[[nodiscard]] bool
mana::operator==(const mana::int_value& i1, std::int64_t i2) noexcept {
    return i1._value == i2;
}

std::unique_ptr<mana::value>
mana::int_value::accept_visitor(mana::basic_visitor* visitor) {
    return visitor->visit(this);
}

std::unique_ptr<mana::value>
mana::int_value::accept_visitor(const mana::basic_visitor* visitor) const {
    return visitor->visit(this);
}
