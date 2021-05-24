/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * /bool_value.cxx --
 *   DOCS WIP
 */

#include <mana/value/bool_value.hxx>
#include <mana/visitors/basic_visitor.hxx>

std::unique_ptr<mana::value>
mana::bool_value::accept_visitor(mana::basic_visitor* visitor) {
    return visitor->visit(this);
}

std::unique_ptr<mana::value>
mana::bool_value::accept_visitor(const mana::basic_visitor* visitor) const {
    return visitor->visit(this);
}

mana::bool_value::bool_value(bool b) noexcept
     : _value(b) { }

mana::bool_value::bool_value(mana::logical b) noexcept
     : bool_value(static_cast<bool>(b)) { }

bool
mana::operator==(const mana::bool_value& b1, const mana::bool_value& b2) noexcept {
    return b1._value == b2._value;
}
