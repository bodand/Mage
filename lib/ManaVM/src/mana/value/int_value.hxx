/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/value/int_value.hxx --
 *   Implements the integer value representation of the ManaVM value subsystem.
 */
#pragma once

#include <cstdint>

#include <mana/value.hxx>

namespace mana {
    struct int_value : value {
        explicit int_value(std::int64_t i) noexcept;

        friend bool
        operator==(const int_value& i1, const int_value& i2) noexcept;
        friend bool
        operator==(std::int64_t i1, const int_value& i2) noexcept;
        friend bool
        operator==(const int_value& i1, std::int64_t i2) noexcept;

        std::unique_ptr<value>
        accept_visitor(basic_visitor* visitor) override;
        std::unique_ptr<value>
        accept_visitor(const basic_visitor* visitor) const override;

        std::int64_t _value;
    };
}
