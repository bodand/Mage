/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * /source_scanner.hxx --
 *   DOCS WIP
 */
#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

#include <mana/db.hxx>

namespace mana {
    struct bad_glob : std::invalid_argument {
        bad_glob(std::string_view glob,
                 std::size_t idx,
                 std::string_view diag);
    };

    struct source_glob {
        source_glob(std::string_view glob_str,
                    std::filesystem::file_time_type last_globbed);

        [[nodiscard]] std::vector<std::filesystem::path>
        operator()() const;

        [[nodiscard]] bool
        cacheable() const;

        std::string glob;
        std::filesystem::path in_dir;
        mutable std::filesystem::file_time_type last_globbed;

    private:
        std::ptrdiff_t _dir_depth;
    };

    using source_t = std::variant<source_glob, std::filesystem::path>;

    struct source_scanner {
        source_scanner();

        std::vector<std::filesystem::path>
        get_sources(std::string_view target);

    private:
        std::unique_ptr<sqlite3, _aux::deleter_t<sqlite3_close>>
               _cache;
        std::unordered_map<std::string, std::vector<source_t>>
               _targets;

        static int
        target_fill(void* self, int argc, char** argv, char** col_name);
        void
        insert_into_table(std::string_view target, std::string_view src);
    };
}
