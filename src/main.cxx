/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license
 *
 * src/main.cxx --
 *   Main entry point for the mage executable.
 */

#include <iostream>

#include <mana/regex.hxx>

int
main() {
    mana::regex re(R"xXx(ye*t)xXx");

    std::string str;
    std::cin >> str;

    std::cout << R"(std:match(std:patern:regex("ye++t"), ")" << str << "\") => " << re.match(str) << "\n";
}
