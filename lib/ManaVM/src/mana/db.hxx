/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/db.hxx --
 *   Defines the constant for the database connection used everywhere in ManaVM.
 *   Also contains other database related helper utilities.
 */
#pragma once

#include <filesystem>
#include <stdexcept>

// create properly destructing unique_ptr
#include <mana/_aux/deleter.hxx>

extern "C" {
    using sqlite3 = struct sqlite3;
    extern int
    sqlite3_close(sqlite3*);
}

namespace mana {
    using db_type = std::unique_ptr<sqlite3, _aux::deleter_t<sqlite3_close>>;

    struct sqlite_error : std::runtime_error {
        using runtime_error::runtime_error;
    };

    namespace _aux {

        struct db_object_type {
            inline static std::filesystem::path db_connection;
            db_object_type();

            [[nodiscard]] static db_type
            get_new();
        };
    }
    [[maybe_unused]] static _aux::db_object_type study;
}
