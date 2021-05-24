/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/visitors/basic_visitor.cxx --
 *   Implements the basic_visitor class, the super-class of all value visitors.
 */

#include <mana/visitors/basic_visitor.hxx>

std::unique_ptr<mana::value>
mana::basic_visitor::visit(mana::int_value*) {
    return nullptr;
}

std::unique_ptr<mana::value>
mana::basic_visitor::visit(const mana::int_value*) const {
    return nullptr;
}

std::unique_ptr<mana::value>
mana::basic_visitor::visit(mana::sym_value*) {
    return nullptr;
}

std::unique_ptr<mana::value>
mana::basic_visitor::visit(const mana::sym_value*) const {
    return nullptr;
}

std::unique_ptr<mana::value>
mana::basic_visitor::visit(mana::bool_value*) {
    return nullptr;
}

std::unique_ptr<mana::value>
mana::basic_visitor::visit(const mana::bool_value*) const {
    return nullptr;
}
