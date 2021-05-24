/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * /equality_visitor.hxx --
 *   DOCS WIP
 */
#pragma once

#include <mana/value.hxx>
#include <mana/visitors/basic_visitor.hxx>

namespace mana {
    bool
    operator==(const sym_value&, const sym_value&) noexcept;
    bool
    operator==(const int_value&, const int_value&) noexcept;
    bool
    operator==(const bool_value&, const bool_value&) noexcept;

    template<class T>
    struct typed_equality_visitor : basic_visitor {
        std::unique_ptr<value>
        visit(const T* value) const override {
            if (static_cast<const T&>(*_value) == static_cast<const T&>(*value))
                return value::make_value(logical::yes);
            return nullptr;
        }

        explicit typed_equality_visitor(const T* value)
             : _value(value) { }

        const T* _value;
    };

    struct equality_visitor : basic_visitor {
        std::unique_ptr<value>
        visit(const sym_value* value) const override;

        std::unique_ptr<value>
        visit(const int_value* value) const override;

        std::unique_ptr<value>
        visit(const bool_value* value) const override;

        explicit equality_visitor(const value* value2)
             : _other(value2) { }

        const value* _other;
    };
}
