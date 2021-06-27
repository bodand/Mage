/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/value.hxx --
 *   The value class describes the universal value object used inside the ManaVM.
 *   Each object is uniquely owned, and has one singular type throughout its
 *   lifetime.
 */
#pragma once

#include <memory>

namespace mana {
    struct basic_visitor;

    enum class logical { no = false,
                         yes = true };

    struct int_value;
    struct sym_value;

    struct value {

        virtual std::unique_ptr<value>
        accept_visitor(basic_visitor*) = 0;
        virtual std::unique_ptr<value>
        accept_visitor(const basic_visitor*) const = 0;

        virtual ~value() = default;

        static std::unique_ptr<value>
        make_value();
        static std::unique_ptr<value>
        make_value(std::int64_t i);
        static std::unique_ptr<value>
        make_value(logical i);
    };

    bool
    operator==(const value& v1, const value& v2) noexcept;
}
