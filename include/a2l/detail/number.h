#ifndef A2L_DETAIL_NUMBER_H
#define A2L_DETAIL_NUMBER_H

#include <charconv>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string_view>
#include <type_traits>

namespace a2lfile::detail {

enum class NumberOutcome { Exact, Defaulted, Truncated };

template<class T> struct NumberRead {
    T value{};
    NumberOutcome outcome = NumberOutcome::Defaulted;
};

// Shared conversion core for the standalone raw loader and typed extraction.
// Every successful parse consumes the whole token. Integer spellings retain all
// 64 bits; floating-to-integer casts occur only inside exact power-of-two bounds.
template<class T, class Item>
NumberRead<T> readNumber(const Item& item) {
    const auto type = item.type();
    if (type == Item::Invalid) return {0, NumberOutcome::Exact};
    if (type == Item::String || type == Item::Identifier) return {};
    const auto text = item.toText(false);
    std::string_view token(text);
    if (token.empty()) return {};
    bool negative = token.front() == '-';
    if (token.front() == '+' || negative) token.remove_prefix(1);
    if (token.empty() || token.front() == '+' || token.front() == '-') return {};

    if (type == Item::Hex || (type == Item::Decimal && std::is_integral_v<T>)) {
        const int base = type == Item::Hex ? 16 : 10;
        if (base == 16 && token.size() >= 2 && token[0] == '0' &&
            (token[1] == 'x' || token[1] == 'X')) token.remove_prefix(2);
        uint64_t magnitude = 0;
        const auto parsed = std::from_chars(token.data(), token.data() + token.size(), magnitude, base);
        if (parsed.ec != std::errc{} || parsed.ptr != token.data() + token.size()) return {};
        if constexpr (std::is_integral_v<T>) {
            if constexpr (std::is_unsigned_v<T>) {
                if ((negative && magnitude != 0) || magnitude > std::numeric_limits<T>::max()) return {};
            } else {
                const uint64_t limit = static_cast<uint64_t>(std::numeric_limits<T>::max());
                if (magnitude > limit + (negative ? 1 : 0)) return {};
                if (negative && magnitude == limit + 1) return {std::numeric_limits<T>::min(), NumberOutcome::Exact};
            }
            const T value = static_cast<T>(magnitude);
            return {negative ? static_cast<T>(-value) : value, NumberOutcome::Exact};
        } else {
            const T value = static_cast<T>(magnitude);
            return {negative ? -value : value, NumberOutcome::Exact};
        }
    }

    // from_chars does not accept a leading '+'. The sign is applied separately.
    using Float = std::conditional_t<std::is_integral_v<T>, long double, T>;
    Float value = 0;
    const auto parsed = std::from_chars(token.data(), token.data() + token.size(), value, std::chars_format::general);
    if (parsed.ec != std::errc{} || parsed.ptr != token.data() + token.size() || !std::isfinite(value)) return {};
    if (negative) value = -value;
    if constexpr (std::is_integral_v<T>) {
        const Float upper = std::ldexp(Float(1), std::numeric_limits<T>::digits);
        const Float lower = std::is_unsigned_v<T> ? Float(0) : -upper;
        if constexpr (std::is_unsigned_v<T>) {
            if (value < 0) return {};
        }
        const Float truncated = std::trunc(value);
        if (truncated < lower || truncated >= upper) return {};
        return {static_cast<T>(truncated), value == truncated ? NumberOutcome::Exact : NumberOutcome::Truncated};
    } else {
        return {value, NumberOutcome::Exact};
    }
}

} // namespace a2lfile::detail
#endif
