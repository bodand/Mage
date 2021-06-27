/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaVM/source_glob_properly_dissects_relative_glob.cxx --
 *   Tests whether the mana::source_glob can deal with absolute glob paths like
 *   /a/*.txt, and C:\b\a\foo* by figuring out the directory to glob and the glob
 *   expression itself.
 *   Tests are written for
 *    - DOS-like multi-rooted, \ separated filesystems
 *    - MacOS 9 one-rooted, : separated filesystem
 *    - VMS multi-rooted, complex syntax filesystem (Files-11)
 *    - VOS multi-rooted, complex syntax filesystem
 *    - unixish one-rooted, / separated filesystems
 */

#include <iostream>

#include <mana/source_scanner.hxx>

int
main() {
    int ret = EXIT_SUCCESS;
    auto glob = mana::source_glob("C:/some/dir/*.txt", {});
    if (glob.glob != "*.txt") {
        std::cerr << "error(C:/some/dir/*.txt): glob != '*.txt'\n\twas: " << glob.glob << '\n';
        ret = EXIT_FAILURE;
    }
    if (glob.in_dir != std::filesystem::path("C:/some/dir")) {
        std::cerr << "error(C:/some/dir/*.txt): in_dir != C:/some/dir\n\twas: " << glob.in_dir << '\n';
        ret = EXIT_FAILURE;
    }
    return ret;
}
