/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/regex.cxx --
 *   Implements the functionality of the class described in F<regex.hxx>.
 */

#include <cstring>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <mana/_macros.hxx>
#include <mana/regex.hxx>

mana::regex::regex(std::string_view regex,
                   int flags,
                   bool do_jit)
     : _expr(nullptr),
       _capture_cnt(0) {
    int ec;
    PCRE2_SIZE err_pos;
    _expr = pcre2_compile(
           reinterpret_cast<PCRE2_SPTR>(regex.data()),
           regex.size() * 8 / PCRE2_CODE_UNIT_WIDTH,
           flags,
           &ec,
           &err_pos,
           nullptr);
    if (!_expr) throw mana::regex_exception(regex, ec, err_pos);

    if (do_jit) compile_jit();

    pcre2_pattern_info(_expr, PCRE2_INFO_CAPTURECOUNT, &_capture_cnt);
}

void
mana::regex::compile_jit() {
    static int have_jit = -1;
    MANA_UNLIKELY_IF(have_jit == -1) {
        auto ret = pcre2_jit_compile(_expr, 0);
        have_jit = ret != PCRE2_ERROR_JIT_BADOPTION;
    }

    if (!have_jit) return;

    if (auto err = pcre2_jit_compile(_expr, PCRE2_JIT_COMPLETE); err)
        throw mana::regex_jit_exception(err);
}

mana::regex::match_result
mana::regex::match(std::string_view str,
                   int flags) {
    auto md = MAGE_C_UNIQ(pcre2_match_data)(
           pcre2_match_data_create((_capture_cnt + 1) * 2, nullptr));
    auto ret = pcre2_match(_expr,
                           reinterpret_cast<PCRE2_SPTR>(str.data()),
                           str.size() * 8 / PCRE2_CODE_UNIT_WIDTH,
                           0,
                           flags,
                           md.get(),
                           nullptr);
    if (ret > 0) { // match
        return match_result(std::move(md), str);
    }
    return match_result(nullptr, str);
}

mana::regex::~regex() noexcept {
    pcre2_code_free(_expr);
}

mana::regex::regex(const mana::regex& cp)
     : _expr(pcre2_code_copy(cp._expr)),
       _capture_cnt(cp._capture_cnt) { }

mana::regex&
mana::regex::operator=(const mana::regex& cp) {
    if (&cp == this) return *this;
    _expr = pcre2_code_copy(cp._expr);
    _capture_cnt = cp._capture_cnt;
    return *this;
}

mana::regex::regex(mana::regex&& mv) noexcept
     : _expr(pcre2_code_copy(mv._expr)),
       _capture_cnt(mv._capture_cnt) { }

mana::regex&
mana::regex::operator=(mana::regex&& mv) noexcept {
    _expr = pcre2_code_copy(mv._expr);
    _capture_cnt = mv._capture_cnt;
    return *this;
}

mana::regex_exception::regex_exception(std::string_view regex,
                                       int ec,
                                       size_t err_pos)
     : _regex(regex.data(), regex.size()),
       _error_code(ec),
       _error_pos(err_pos) { }

const char*
mana::regex_exception::what() const noexcept {
    static char err_buf[128 * 8 / PCRE2_CODE_UNIT_WIDTH];
    static std::string str;
    std::ostringstream oss;
    if (pcre2_get_error_message(_error_code,
                                reinterpret_cast<PCRE2_UCHAR*>(err_buf),
                                sizeof err_buf)
        == PCRE2_ERROR_BADDATA) {
        std::strcpy(err_buf, "unknown error");
    }
    auto w = oss.width();
    oss << "error code: " << _error_code << "\n";
    oss << "regex error: " << err_buf << "\n";
    oss << "             in '" << _regex << "' at position " << _error_pos << "\n";
    oss << std::setw(static_cast<int>(_error_pos + 1 + 17)) << "^"
        << std::setw(static_cast<int>(w)) << "-- around here";
    str = oss.str();
    return str.c_str();
}

mana::regex_jit_exception::regex_jit_exception(int error_code)
     : _error_code(error_code) { }

const char*
mana::regex_jit_exception::what() const noexcept {
    static std::string str;
    std::ostringstream oss;
    oss << "error code: " << _error_code << "\n";
    oss << "jit error: couldn't JIT compile regular expression\n";
    str = oss.str();
    return str.c_str();
}

mana::regex::match_result::match_result(MAGE_C_UNIQ(pcre2_match_data) && md,
                                        std::string_view str)
     : _md(std::move(md)),
       _str(str) { }

mana::regex::match_result::operator bool() const noexcept {
    return _md != nullptr;
}

std::pair<bool, std::string>
mana::regex::match_result::operator[](std::size_t idx) {
    if (!_md) return {};

    std::string str;
    std::size_t match_sz = 0;

    auto ret = pcre2_substring_length_bynumber(_md.get(), idx, &match_sz);
    switch (ret) {
    case PCRE2_ERROR_NOSUBSTRING:
    case PCRE2_ERROR_UNAVAILABLE: {
        std::ostringstream ss;
        ss << "error: capture group index " << idx << " is out of range";
        throw std::out_of_range(ss.str().c_str());
    }
    case PCRE2_ERROR_UNSET:
        return {false, ""};
    default:;
    }
    str.resize(++match_sz);

    pcre2_substring_copy_bynumber(_md.get(), idx, reinterpret_cast<PCRE2_UCHAR*>(str.data()), &match_sz);
    // ^ no error can occur here
    str.resize(match_sz); // strip the superfluous \0
    return {true, str};
}
std::pair<bool, std::string>
mana::regex::match_result::captured_name(std::string_view name) {
    if (!_md) return {};

    std::string str;
    std::size_t match_sz = 0;

    auto ret = pcre2_substring_length_byname(_md.get(),
                                             reinterpret_cast<PCRE2_SPTR>(name.data()),
                                             &match_sz);
    switch (ret) {
    case PCRE2_ERROR_NOSUBSTRING:
    case PCRE2_ERROR_UNAVAILABLE: {
        std::ostringstream ss;
        ss << "error: capture group named '" << name << "' is undefined";
        throw std::out_of_range(ss.str().c_str());
    }
    case PCRE2_ERROR_UNSET:
        return {false, ""};
    default:;
    }
    str.resize(++match_sz);

    pcre2_substring_copy_byname(_md.get(),
                                reinterpret_cast<PCRE2_SPTR>(name.data()),
                                reinterpret_cast<PCRE2_UCHAR*>(str.data()),
                                &match_sz);
    // ^ no error can occur here
    str.resize(match_sz); // strip the superfluous \0
    return {true, str};
}

std::pair<bool, std::string>
mana::regex::match_result::operator[](std::string_view name) {
    return captured_name(name);
}

std::pair<bool, std::string>
mana::regex::match_result::operator[](const char* name) {
    return captured_name(name);
}

mana::regex::match_result::iterator
mana::regex::match_result::begin() {
    std::size_t ovec_sz = pcre2_get_ovector_count(_md.get());
    auto ovec = pcre2_get_ovector_pointer(_md.get());

    return mana::regex::match_result::iterator(ovec_sz,
                                               ovec,
                                               _str);
}

mana::regex::match_result::iterator
mana::regex::match_result::end() {
    std::size_t ovec_sz = pcre2_get_ovector_count(_md.get());
    auto ovec_end = pcre2_get_ovector_pointer(_md.get());
    ovec_end += ovec_sz;

    return mana::regex::match_result::iterator(0,
                                               ovec_end,
                                               _str);
}

mana::regex::match_result::iterator::iterator(std::size_t ovec_sz,
                                              size_t* ovec,
                                              std::string_view str)
     : _ovec_sz(ovec_sz),
       _ovec(ovec),
       _str(str) { }

std::string_view
mana::regex::match_result::iterator::operator*() const noexcept {
    auto begin = _str.data() + _ovec[0];
    auto end = _str.data() + _ovec[1];

    return {begin, static_cast<std::size_t>(end - begin)};
}

mana::regex::match_result::iterator&
mana::regex::match_result::iterator::operator+=(std::size_t n) {
    if (_ovec_sz == 0) return *this;

    for (std::size_t shifted = 0;
         shifted != n;) {
        _ovec_sz -= 2;
        _ovec += 2;

        if (_ovec[0] != PCRE2_UNSET) ++shifted;
    }

    return *this;
}

mana::regex::match_result::iterator&
mana::regex::match_result::iterator::operator-=(std::size_t n) {
    for (std::size_t shifted = 0;
         shifted != n;) {
        _ovec_sz += 2;
        _ovec -= 2;

        if (_ovec[0] != PCRE2_UNSET) ++shifted;
    }

    return *this;
}

mana::regex::match_result::iterator
mana::regex::match_result::iterator::operator++(int) {
    auto cp = *this;
    this->operator++();
    return cp;
}

mana::regex::match_result::iterator
mana::regex::match_result::iterator::operator--(int) {
    auto cp = *this;
    this->operator--();
    return cp;
}

mana::regex::match_result::iterator::iterator()
     : _ovec(nullptr),
       _ovec_sz(0),
       _str() { }
