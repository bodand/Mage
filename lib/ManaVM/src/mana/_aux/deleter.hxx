/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/_aux/deleter.hxx --
 *   Provides an short syntax for declaring unique_ptr for C API entities using
 *   a macro and templates.
 */
#pragma once

#define MAGE_C_UNIQ(typ) ::std::unique_ptr<typ, ::mana::_aux::deleter_t<typ##_free>>

namespace mana::_aux {
    template<auto>
    struct universal_deleter;

    template<class T, class R, R (*Fn)(T*)>
    struct universal_deleter<Fn> {
        void
        operator()(T* obj) const { Fn(obj); }
    };

    template<auto DeleteFn>
    using deleter_t = universal_deleter<DeleteFn>;
}
