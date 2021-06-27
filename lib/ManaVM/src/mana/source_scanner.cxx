/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * /source_scanner.cxx --
 *   DOCS WIP
 */

#include <algorithm>
#include <charconv>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include <mana/_macros.hxx>
#include <mana/db.hxx>
#include <mana/source_scanner.hxx>

#include <boost/nowide/convert.hpp>

#include <sqlite3.h>

namespace fs = std::filesystem;
namespace chr = std::chrono;
using std::begin, std::end;
using namespace std::literals;

namespace {
    template<class... Ts>
    struct overload_set : Ts... { using Ts::operator()...; };
    template<class... Ts>
    overload_set(Ts...) -> overload_set<Ts...>;

    void ch_path_sep(std::string& str) {
        if constexpr (fs::path::preferred_separator
                      != decltype(fs::path::preferred_separator)('/')) {
            std::string utf8_sep;
            if constexpr (std::is_same_v<wchar_t, fs::path::value_type>) {
                utf8_sep = boost::nowide::narrow(&fs::path::preferred_separator, 1);
            } else {
                utf8_sep = fs::path::preferred_separator;
            }

            for (std::size_t slash_pos = str.find('/');
                 slash_pos != str.npos;) {
                str.erase(slash_pos, 1);
                str.insert(slash_pos, utf8_sep.c_str(), utf8_sep.size());
                slash_pos = str.find('/', slash_pos);
            }
        }
    }

    struct glob_tree {
        std::unique_ptr<glob_tree> next;

        glob_tree(const glob_tree& cp)
             : next(cp.next->clone()) { }
        glob_tree&
        operator=(const glob_tree& cp) {
            next = cp.next->clone();
            return *this;
        }

        glob_tree(glob_tree&& mv) noexcept = default;
        glob_tree&
        operator=(glob_tree&& mv) noexcept = default;

        [[nodiscard]] virtual std::size_t
        match(std::string_view str) const = 0;

        virtual ~glob_tree() = default;

    protected:
        [[nodiscard]] virtual std::unique_ptr<glob_tree>
        clone() = 0;

        glob_tree() = default;
    };

    struct glob_any final : glob_tree {
        glob_any() = default;

        glob_any(const glob_any& cp) noexcept = default;
        glob_any&
        operator=(const glob_any& cp) noexcept = default;

        glob_any(glob_any&& mv) noexcept = default;
        glob_any&
        operator=(glob_any&& mv) noexcept = default;

        [[nodiscard]] MANA_PURE() std::size_t
               match(std::string_view str) const final {
            return str.empty() ? static_cast<std::size_t>(-1)
                               : 1;
        }

        ~glob_any() override = default;

    protected:
        [[nodiscard]] std::unique_ptr<glob_tree>
        clone() override {
            return std::make_unique<glob_any>();
        }
    };

    struct glob_substr final : glob_tree {
        explicit glob_substr(std::string_view substr)
             : _str(substr.data(), substr.size()) { }

        glob_substr(const glob_substr& cp) = default;
        glob_substr&
        operator=(const glob_substr& cp) = default;

        glob_substr(glob_substr&& mv) noexcept = default;
        glob_substr&
        operator=(glob_substr&& mv) noexcept = default;

        [[nodiscard]] std::size_t
        match(std::string_view str) const override {
            auto str_len = _str.size();
            return str.substr(0, str_len) == _str ? str_len
                                                  : static_cast<std::size_t>(-1);
        }

        ~glob_substr() override = default;

    protected:
        [[nodiscard]] std::unique_ptr<glob_tree>
        clone() override {
            return std::make_unique<glob_substr>(_str);
        }

    private:
        std::string _str;
    };

    struct glob_star final : glob_tree {
        glob_star()
             : _until() { }
        explicit glob_star(char until)
             : _until(1, until) { }
        explicit glob_star(std::string until)
             : _until(std::move(until)) { }

        glob_star(const glob_star& cp) = default;
        glob_star&
        operator=(const glob_star& cp) = default;

        glob_star(glob_star&& mv) noexcept = default;
        glob_star&
        operator=(glob_star&& mv) noexcept = default;

        [[nodiscard]] MANA_PURE() std::size_t
               match(std::string_view str) const override {
            if (_until.empty()) { // * at end of glob -> yank everything
                return str.size();
            }

            auto end_char = str.find(_until);
            if (end_char == str.npos) return static_cast<std::size_t>(-1);
            return end_char + 1;
        }

    protected:
        [[nodiscard]] std::unique_ptr<glob_tree>
        clone() override {
            return std::make_unique<glob_star>(_until);
        }

    private:
        std::string _until;
    };

    struct glob_class final : glob_tree {
        explicit glob_class(std::string_view ch_class)
             : _class(ch_class.data(), ch_class.size()) {
            std::sort(begin(_class), end(_class));
            _class.erase(std::unique(begin(_class), end(_class)), end(_class));
        }

        glob_class(const glob_class& cp) = default;
        glob_class&
        operator=(const glob_class& cp) = default;

        glob_class(glob_class&& mv) noexcept = default;
        glob_class&
        operator=(glob_class&& mv) noexcept = default;

        [[nodiscard]] std::size_t
        match(std::string_view str) const override {
            auto found = std::binary_search(begin(_class), end(_class), str[0]);
            return found ? 1
                         : static_cast<std::size_t>(-1);
        }

        ~glob_class() override = default;

    protected:
        [[nodiscard]] std::unique_ptr<glob_tree>
        clone() override {
            return std::make_unique<glob_class>(_class);
        }

    private:
        std::string _class;
    };

    [[nodiscard]] glob_tree*
    make_glob(std::string_view glob_str) {
        static thread_local std::unordered_map<std::string_view,
                                               std::unique_ptr<glob_tree>>
               _glob_cache;
        if (auto entry_it = _glob_cache.find(glob_str);
            entry_it != end(_glob_cache)) {
            return entry_it->second.get();
        }

        if (glob_str.empty()) {
            auto [it, _] = _glob_cache.emplace(glob_str, std::make_unique<glob_substr>(glob_str));
            return it->second.get();
        }

        std::unique_ptr<glob_tree> glob_expr;
        auto append_to_expr = [last_expr = &glob_expr](std::unique_ptr<glob_tree>&& subtree) mutable {
            *last_expr = std::move(subtree);
            last_expr = std::addressof(last_expr->get()->next);
        };

        constexpr const static auto meta_chars = "*?[\\";
        for (std::size_t i = 0;;) {
            MANA_UNLIKELY_IF(glob_str.size() <= i) break;

            switch (glob_str[i]) {
            case '*':
                ++i;
                if (glob_str.size() == i) {
                    append_to_expr(std::make_unique<glob_star>());
                } else {
                    std::string until_str(1, glob_str[i]);
                    ch_path_sep(until_str);
                    append_to_expr(std::make_unique<glob_star>(until_str));
                }
                ++i;
                break;
            case '?':
                append_to_expr(std::make_unique<glob_any>());
                ++i;
                break;
            case '[': {
                auto close_pos = glob_str.find(']', i + 1);
                if (close_pos == i + 1) close_pos = glob_str.find(']', i + 2);
                if (close_pos == glob_str.npos) {
                    // never ending [...
                    throw mana::bad_glob(glob_str, i, "unclosed character group");
                }
                append_to_expr(std::make_unique<glob_class>(glob_str.substr(i + 1,
                                                                            close_pos - 1 - i)));
                i = close_pos + 1;
                break;
            }
            case '\\':
                ++i; // \x: ignore \ and treat x as a literal char
            default: {
                std::string str;
                for (;;) {
                    auto next_meta = glob_str.find_first_of(meta_chars, i);
                    if (next_meta == glob_str.npos) { // 🦀🦀🦀 meta is no more 🦀🦀🦀
                        str = glob_str.substr(i);
                        i = glob_str.size();
                    } else {
                        str += glob_str.substr(i, next_meta - i);
                        i = next_meta;
                        if (glob_str[next_meta] == '\\') continue;
                    }
                    break;
                }


                append_to_expr(std::make_unique<glob_substr>(str));
                break;
            }
            }
        }

        auto [it, _] = _glob_cache.emplace(glob_str, std::move(glob_expr));
        return it->second.get();
    }

    bool
    glob_match(std::string_view glob_str,
               const fs::path& glob_dir,
               const fs::path& path) {
        auto glob = make_glob(glob_str);
        auto glob_path = glob_dir.string();
        auto path_str = path.string();
        if (!std::equal(begin(glob_path), end(glob_path), begin(path_str))) {
            return false;
        }
        path_str.erase(0, std::size(glob_path) + 1 /* final separator */);

        while (glob != nullptr) {
            auto match_cnt = glob->match(path_str);
            if (match_cnt == static_cast<std::size_t>(-1)) {
                return false;
            }

            path_str.erase(0, match_cnt);
            glob = glob->next.get();
        }
        return path_str.empty();
    }
}

mana::source_glob::source_glob(std::string_view glob_str,
                               std::filesystem::file_time_type last_globbed)
     : glob(),
       in_dir(),
       last_globbed(last_globbed),
       _dir_depth() {
    auto glob_start = glob_str.find_first_of("?*[");
    if (glob_start == glob_str.npos) {
        // not actually glob, yet somehow made it here
        glob = fs::path(glob_str).filename().string();
        in_dir = fs::path(glob_str).parent_path();
    } else {
        auto highest_globbed_dir_start = glob_str.rfind('/', glob_start);
        if (highest_globbed_dir_start == glob_str.npos) {
            // no directory above glob -> use cwd with dir_depth = 0
            glob = glob_str;
            in_dir = fs::current_path();
        } else {
            glob = glob_str.substr(highest_globbed_dir_start + 1);
            in_dir = fs::path(glob_str.substr(0, highest_globbed_dir_start));
            _dir_depth = std::ranges::count(glob, '/');
        }
    }

    if (in_dir.empty()) in_dir = fs::current_path();
    if (in_dir.is_relative()) in_dir = fs::absolute(in_dir);
}

[[nodiscard]] std::vector<std::filesystem::path>
mana::source_glob::operator()() const {
    std::vector<std::filesystem::path> ret;
    for (fs::recursive_directory_iterator file(in_dir);
         file != fs::recursive_directory_iterator();
         ++file) {
        if (file.depth() != _dir_depth) continue; // skip incorrect depth
        if (file->is_directory()) continue;       // skip dir entries in general
        if (glob_match(glob, in_dir, file->path()))
            ret.push_back(file->path());
    }
    last_globbed = decltype(last_globbed)::clock::now();
    return ret;
}

bool
mana::source_glob::cacheable() const {
    return fs::last_write_time(in_dir) <= last_globbed;
}

std::vector<std::filesystem::path>
mana::source_scanner::get_sources(std::string_view target) {
    using std::begin, std::end;

    auto&& srcs = _targets[target.data()];

    std::vector<std::filesystem::path> ret;
    ret.reserve(srcs.size());

    for (auto&& src : srcs) {
        std::visit(overload_set{
                          [&ret](const fs::path& path) {
                              ret.push_back(path);
                          },
                          [&ret](const source_glob& glob) {
                              // if (glob.cacheable()) // todo: cache
                              auto files = glob();
                              ret.insert(end(ret), begin(files), end(files));
                          }},
                   src);
    }

    return ret;
}

namespace {
    constexpr const char query_source_sets[] = R"__(
SELECT target.name,
       source_sets.path,
       source_sets.globbed_at
FROM
    target INNER JOIN source_sets USING(target_id);
)__";

    constexpr const char insert_source[] = R"__(
INSERT INTO source_sets(path, target_id)
  SELECT $path,
         target_id
  FROM target
  WHERE target.name = $target;
)__";
}

mana::source_scanner::source_scanner()
     : _cache(study.get_new()),
       _targets() {
    char* err_msg;
    auto ret = sqlite3_exec(_cache.get(),
                            query_source_sets,
                            target_fill,
                            this,
                            &err_msg);
    if (ret != SQLITE_OK) {
        auto err_str = std::string(err_msg);
        sqlite3_free(err_msg);
        throw mana::sqlite_error(err_str);
    }
}

int
mana::source_scanner::target_fill(void* v_self, int argc, char** argv, char**) {
    auto self = static_cast<source_scanner*>(v_self);
    auto insert_src = [self](std::string_view target,
                             std::string_view src,
                             const int* last_globbed) {
        auto target_it = self->_targets.find({target.data(), target.size()});
        if (target_it == end(self->_targets)) {
            auto [new_it, _] = self->_targets.emplace(std::string{target.data(), target.size()},
                                                      std::vector<source_t>{});
            target_it = new_it;
        }
        if (last_globbed) {
            auto glob_time = chr::file_clock::from_sys(
                   chr::sys_seconds() + chr::seconds(*last_globbed));

            target_it->second.emplace_back(source_glob(src, glob_time));
        } else {
            target_it->second.emplace_back(src);
        }
    };

    if (argc > 2) {
        auto&& target = argv[0];
        auto glob_or_src = std::string_view(argv[1]);
        auto maybe_glob_time = argv[2];
        int glob_secs = 0;
        int* glob_ptr = nullptr;
        if (maybe_glob_time) {
            if (auto [_, err] = std::from_chars(maybe_glob_time,
                                                maybe_glob_time + std::strlen(maybe_glob_time),
                                                glob_secs);
                err != std::errc{}) {
                std::cerr << "ignoring invalid entry: "
                          << "  target: " << target << "\n"
                          << "  glob: " << glob_or_src << "\n"
                          << "  seconds since globbed: " << glob_secs
                          << " <-- error: " << make_error_code(err).message() << " \n";
                return 0;
            }
            glob_ptr = &glob_secs;
        }

        insert_src(target, glob_or_src, glob_ptr);
    }
    return 0;
}

using stmt_type = std::unique_ptr<sqlite3_stmt, mana::_aux::deleter_t<sqlite3_finalize>>;

void
mana::source_scanner::insert_into_table(std::string_view target, std::string_view src) {
    stmt_type stmt;

    auto bind_text = [stmt = stmt.get(), db = _cache.get()](auto idx, std::string_view text) {
        auto res = sqlite3_bind_text(stmt,
                                     idx,
                                     text.data(),
                                     static_cast<int>(text.size()),
                                     SQLITE_STATIC);
        if (res != SQLITE_OK) throw mana::sqlite_error(sqlite3_errmsg(db));
    };


    sqlite3_stmt* raw_stmt; // !!RAII unstable!!
    auto res = sqlite3_prepare_v2(_cache.get(),
                                  insert_source,
                                  std::size(insert_source),
                                  &raw_stmt,
                                  nullptr);
    if (raw_stmt == nullptr) throw std::bad_alloc();
    stmt.reset(raw_stmt); // !!RAII stable!!

    if (res != SQLITE_OK) throw mana::sqlite_error(sqlite3_errstr(res));

    const auto path_idx = sqlite3_bind_parameter_index(stmt.get(), "path");
    const auto target_idx = sqlite3_bind_parameter_index(stmt.get(), "target");

    bind_text(path_idx, src);
    bind_text(target_idx, target);

    res = sqlite3_step(stmt.get());
    if (res != SQLITE_DONE) throw mana::sqlite_error(sqlite3_errstr(res));
}

mana::bad_glob::bad_glob(std::string_view glob,
                         std::size_t idx,
                         std::string_view diag)
     : std::invalid_argument([glob, idx, diag] {
           std::ostringstream oss;
           auto w = oss.width();
           auto fill = oss.fill();
           oss << "error in glob expression: " << glob << "\n"
               << std::setw(static_cast<int>(27 + idx)) << '^'
               << std::setw(static_cast<int>(glob.size() - idx)) << std::setfill('~') << ' '
               << std::setw(static_cast<int>(w)) << std::setfill(fill) << diag;
           return oss.str();
       }()) { }
