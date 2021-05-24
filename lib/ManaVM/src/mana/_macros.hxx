/* Mage project
 *
 * Copyright (c) 2021, András Bodor <bodand@pm.me>
 * Licensed under the BSD 3-Clause license.
 *
 * lib/ManaVM/src/mana/_macros.hxx --
 *   Extra macros that abstract over compiler specific extension or newer C++
 *   standard options, which are not yet used, but should be a one-click enable.
 *   Macros beginning with MANA_I are implementation details and are not to be
 *   relied upon.
 */
#pragma once

#if __has_cpp_attribute(likely) >= 201803L
#    define MANA_I_STD_LIKELY [[likely]]
#    define MANA_I_HAS_STD_LIKELY
#else
#    define MANA_I_STD_LIKELY
#endif

#if __has_cpp_attribute(unlikely) >= 201803L
#    define MANA_I_STD_UNLIKELY [[unlikely]]
#    define MANA_I_HAS_STD_UNLIKELY
#else
#    define MANA_I_STD_UNLIKELY
#endif

/* ===================== MANA_LIKELY()/MANA_UNLIKELY() ====================== */
#ifdef __GNUC__
#    define MANA_LIKELY(...) __builtin_expect(!!(__VA_ARGS__), 1)
#    define MANA_UNLIKELY(...) __builtin_expect(!!(__VA_ARGS__), 0)
#else
#    define MANA_LIKELY(...) (__VA_ARGS__)
#    define MANA_UNLIKELY(...) (__VA_ARGS__)
#endif

/* ============================ MANA_LIKELY_IF() ============================ */
#ifdef MANA_I_HAS_STD_LIKELY
#    define MANA_LIKELY_IF(...) if (__VA_ARGS__) MANA_I_STD_LIKELY
#else
#    define MANA_LIKELY_IF(...) if (MANA_LIKELY(__VA_ARGS__))
#endif

/* =========================== MANA_UNLIKELY_IF() =========================== */
#ifdef MANA_I_HAS_STD_UNLIKELY
#    define MANA_UNLIKELY_IF(...) if (__VA_ARGS__) MANA_I_STD_UNLIKELY
#else
#    define MANA_UNLIKELY_IF(...) if (MANA_UNLIKELY(__VA_ARGS__))
#endif

/* ============================== MANA_ABYSS() ============================== */
#ifdef __GNUC__
#    define MANA_ABYSS() __builtin_unreachable()
#elif defined(_MSC_VER)
#    define MANA_ABYSS() __assume(0)
#else
#    define MANA_ABYSS() ((void)0)
#endif
