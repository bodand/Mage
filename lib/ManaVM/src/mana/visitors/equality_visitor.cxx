/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/visitors/equality_visitor.cxx --
 *   Implements the functionality of the untyped equality_visitor.
 */

#include <mana/value.hxx>
#include <mana/visitors/equality_visitor.hxx>

std::unique_ptr<mana::value>
mana::equality_visitor::visit(const mana::sym_value* value) const {
    const typed_equality_visitor sym_v(value);
    return _other->accept_visitor(&sym_v);
}

std::unique_ptr<mana::value>
mana::equality_visitor::visit(const mana::int_value* value) const {
    const typed_equality_visitor int_v(value);
    return _other->accept_visitor(&int_v);
}

std::unique_ptr<mana::value>
mana::equality_visitor::visit(const mana::bool_value* value) const {
    const typed_equality_visitor bool_v(value);
    return _other->accept_visitor(&bool_v);
}
