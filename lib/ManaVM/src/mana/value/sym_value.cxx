/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/value/sym_value.cxx --
 *   Implements mana::sym_value.
 */

#include <mana/value/sym_value.hxx>
#include <mana/visitors/basic_visitor.hxx>

[[nodiscard]] bool
mana::operator==(const mana::sym_value& s1, const mana::sym_value& s2) noexcept {
    return &s1 == &s2;
}

std::unique_ptr<mana::value>
mana::sym_value::accept_visitor(mana::basic_visitor* visitor) {
    return visitor->visit(this);
}

std::unique_ptr<mana::value>
mana::sym_value::accept_visitor(const mana::basic_visitor* visitor) const {
    return visitor->visit(this);
}
