/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * test/ManaMV/source_glob_star_autoescape_happens_as_docs_say.cxx --
 *   As per the docs, in ManaVM *? matches a file whose last character is a '?'.
 *   This is of course unlikely as ? is illegal in a filename in a lot of places.
 */

#include <mana/source_scanner.hxx>

int
main() {
    mana::source_glob glob("*?", {});
    auto res = glob();
    return !res.empty();
}
