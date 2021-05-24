/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/value/bool_value.hxx --
 *   A boolean type value for logical operations.
 */
#pragma once

#include <mana/value.hxx>

namespace mana {
    struct bool_value : value {
        explicit bool_value(logical b) noexcept;
        explicit bool_value(bool b) noexcept;

        std::unique_ptr<value>
        accept_visitor(basic_visitor* visitor) override;
        std::unique_ptr<value>
        accept_visitor(const basic_visitor* visitor) const override;

        friend bool
        operator==(const bool_value& b1, const bool_value& b2) noexcept;

        bool _value;
    };
}
