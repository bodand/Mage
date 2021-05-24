/* Mage project
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/regex.hxx --
 *   This file provides a wrapper around PCRE2 with RAII, and all other fancy
 *   requirements we developers have.
 */
#pragma once

#include <exception>
#include <memory>
#include <string>
#include <string_view>

#include <mana/_aux/deleter.hxx>

#define PCRE2_STATIC 1
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

namespace mana {
    struct regex_exception : std::exception {
        [[nodiscard]] const char*
        what() const noexcept override;

        regex_exception(std::string_view regex,
                        int ec,
                        PCRE2_SIZE err_pos);

    private:
        std::string _regex;
        int _error_code;
        PCRE2_SIZE _error_pos;
    };

    struct regex_jit_exception : std::exception {
        [[nodiscard]] const char*
        what() const noexcept override;

        regex_jit_exception(int error_code);

    private:
        int _error_code;
    };

    struct regex {
        struct match_result {
            struct iterator {
                [[nodiscard]] std::string_view
                operator*() const noexcept;

                iterator&
                operator+=(std::size_t n);
                iterator&
                operator-=(std::size_t n);

                iterator&
                operator++() { return operator+=(1); }
                iterator&
                operator--() { return operator-=(1); }

                iterator
                operator++(int);
                iterator
                operator--(int);

                [[nodiscard]] friend auto
                operator<=>(const iterator& it1,
                            const iterator& it2) { return it1._ovec <=> it2._ovec; }
                [[nodiscard]] friend bool
                operator==(const iterator& it1,
                           const iterator& it2) { return it1._ovec == it2._ovec; }

                iterator();

                iterator(const iterator& cp) noexcept = default;
                iterator&
                operator=(const iterator& cp) noexcept = default;
                iterator(iterator&& mv) noexcept = default;
                iterator&
                operator=(iterator&& mv) noexcept = default;

            private:
                friend match_result;

                iterator(std::size_t ovec_sz,
                         PCRE2_SIZE* ovec,
                         std::string_view str);

                std::size_t _ovec_sz;
                PCRE2_SIZE* _ovec;
                std::string_view _str;
            };

            [[nodiscard]] iterator
            begin();
            [[nodiscard]] iterator
            end();

            [[nodiscard]]
            operator bool() const noexcept;

            [[nodiscard]] std::pair<bool, std::string>
            operator[](std::size_t idx);
            [[nodiscard]] std::pair<bool, std::string>
            operator[](const char* name);
            [[nodiscard]] std::pair<bool, std::string>
            operator[](std::string_view name);

        private:
            friend regex;
            std::pair<bool, std::string>
            captured_name(std::string_view name);

            explicit match_result(MAGE_C_UNIQ(pcre2_match_data) && md, std::string_view str);
            // clang-format off
            MAGE_C_UNIQ(pcre2_match_data) _md;
            // clang-format on
            std::string_view _str;
        };

        regex(std::string_view regex,
              int flags = 0,
              bool do_jit = true);

        regex(const regex& cp);
        regex&
        operator=(const regex& cp);
        regex(regex&& mv) noexcept;
        regex&
        operator=(regex&& mv) noexcept;

        [[nodiscard]] match_result
        match(std::string_view str, int flags = 0);

        ~regex() noexcept;

    private:
        void
        compile_jit();

        pcre2_code* _expr;
        std::uint32_t _capture_cnt;
    };

}
