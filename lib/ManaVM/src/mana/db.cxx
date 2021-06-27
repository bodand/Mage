/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * /db.cxx --
 *   DOCS WIP
 */

#include <filesystem>
#include <string>
using namespace std::literals;
namespace fs = std::filesystem;

#include <mana/db.hxx>

#include <sqlite3.h>

namespace {
    constexpr const auto make_tables = R"__(
CREATE TABLE IF NOT EXISTS target(
    target_id INTEGER NOT NULL PRIMARY KEY,
    name      TEXT    NOT NULL
);

CREATE TABLE IF NOT EXISTS dependencies(
    target_id     INTEGER NOT NULL,
    dependency_id INTEGER,

    FOREIGN KEY(target_id)
    REFERENCES target(target_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
    FOREIGN KEY(dependency_id)
    REFERENCES target(target_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS source_sets(
    source_id  INTEGER NOT NULL PRIMARY KEY,
    path       TEXT    NOT NULL,
    target_id  INTEGER NOT NULL,
    globbed_at INTEGER,

    FOREIGN KEY(target_id)
    REFERENCES target(target_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);
)__";
}

mana::_aux::db_object_type::db_object_type() {
    db_connection = std::filesystem::current_path() / ".Build" / "Study.db";

    if (auto ret = sqlite3_initialize();
        ret != SQLITE_OK) {
        throw mana::sqlite_error("cannot init SQLite3: "s + sqlite3_errstr(ret));
    }
    fs::create_directories(db_connection.parent_path());
}

[[nodiscard]] mana::db_type
mana::_aux::db_object_type::get_new() {

    db_type ret;
    sqlite3* db; // !!RAII unstable!!
    auto res = sqlite3_open_v2(reinterpret_cast<const char*>(db_connection.u8string().c_str()),
                               &db,
                               SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX,
                               nullptr);
    if (db == nullptr) throw std::bad_alloc();
    ret.reset(db); // !!RAII stable!!

    if (res != SQLITE_OK) throw mana::sqlite_error(sqlite3_errmsg(ret.get()));

    (void) sqlite3_extended_result_codes(ret.get(), 1);

    char* err_msg;
    res = sqlite3_exec(ret.get(),
                       make_tables,
                       nullptr,
                       nullptr,
                       &err_msg);
    if (res != SQLITE_OK) throw mana::sqlite_error(sqlite3_errmsg(ret.get()));

    return ret;
}
