#pragma once
#include <utility>

// ═══════════════════════════════════════════════════════════
//  Custom Pair<T1, T2>  —  аналог std::pair (Задание 5)
// ═══════════════════════════════════════════════════════════

template<typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    Pair() : first(), second() {}

    Pair(const T1& a, const T2& b) : first(a), second(b) {}

    Pair(T1&& a, T2&& b)
        : first(std::move(a)), second(std::move(b)) {}

    Pair(const Pair&) = default;
    Pair(Pair&&) noexcept = default;
    Pair& operator=(const Pair&) = default;
    Pair& operator=(Pair&&) noexcept = default;

    bool operator==(const Pair& o) const
    { return first == o.first && second == o.second; }
    bool operator!=(const Pair& o) const { return !(*this == o); }
    bool operator< (const Pair& o) const
    { return first < o.first || (first == o.first && second < o.second); }
    bool operator<=(const Pair& o) const { return !(o < *this); }
    bool operator> (const Pair& o) const { return o < *this; }
    bool operator>=(const Pair& o) const { return !(*this < o); }

    void swap(Pair& o) {
        using std::swap;
        swap(first,  o.first);
        swap(second, o.second);
    }
};

template<typename T1, typename T2>
Pair<T1,T2> make_pair(T1&& a, T2&& b) {
    return Pair<T1,T2>(std::forward<T1>(a), std::forward<T2>(b));
}
