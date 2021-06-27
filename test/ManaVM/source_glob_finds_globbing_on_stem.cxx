/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/source_glob_finds_globbing_on_suffixes.cxx --
 *   Checks if expressions like "foo.*" return correct set of files from
 *   globbing.
 */

#include <vector>
#include <filesystem>
#include <algorithm>
namespace fs = std::filesystem;

#include <mana/source_scanner.hxx>

int
main() {
    mana::source_glob glob("glob-test/a.*", {});
    auto texts = glob();
    auto expected = std::vector<fs::path>{
           fs::absolute("glob-test/a.cxx"),
           fs::absolute("glob-test/a.txt"),
    };

    return !std::ranges::equal(texts, expected);
}
