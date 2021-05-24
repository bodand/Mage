/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/value/sym_value.hxx --
 *   A symbol type which can only compare true to itself, and compares false
 *   to everything else.
 */
#pragma once

#include <mana/value.hxx>

namespace mana {
    struct sym_value : value {
        friend bool
        operator==(const sym_value& s1, const sym_value& s2) noexcept;

        std::unique_ptr<value>
        accept_visitor(basic_visitor* visitor) override;
        std::unique_ptr<value>
        accept_visitor(const basic_visitor* visitor) const override;
    };
}
