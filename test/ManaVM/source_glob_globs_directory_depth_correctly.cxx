/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * /source_glob_globs_directory_depth_correctly.cxx --
 *   DOCS WIP
 */

#include <vector>
#include <filesystem>
#include <algorithm>
namespace fs = std::filesystem;

#include <mana/source_scanner.hxx>

int
main() {
    mana::source_glob glob("glob-test/*/a.txt", {});
    auto texts = glob();
    auto expected = std::vector<fs::path>{
           fs::absolute("glob-test/bar/a.txt"),
           fs::absolute("glob-test/foo/a.txt"),
    };

    return !std::ranges::equal(texts, expected);
}
