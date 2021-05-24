/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/visitors/basic_visitor.hxx --
 *   The interface for the base visitor class of all visitors.
 */
#pragma once

#include <memory>

#include <mana/value.hxx>

namespace mana {
    struct int_value;
    struct sym_value;
    struct bool_value;

    struct basic_visitor {
        virtual std::unique_ptr<value>
        visit(int_value*);
        virtual std::unique_ptr<value>
        visit(const int_value*) const;
        virtual std::unique_ptr<value>
        visit(sym_value*);
        virtual std::unique_ptr<value>
        visit(const sym_value*) const;
        virtual std::unique_ptr<value>
        visit(bool_value*);
        virtual std::unique_ptr<value>
        visit(const bool_value*) const;
    };
}
