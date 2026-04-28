#ifndef BITSET_H
#define BITSET_H

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>

class BitSet
{
public:
    explicit BitSet(size_t size);
    BitSet(const BitSet& other) = default;
    BitSet& operator=(const BitSet& other) = default;

    // Геттеры/сеттеры
    bool operator[](size_t pos) const;
    bool test(size_t pos) const;
    BitSet& set(size_t pos, bool value = true);
    BitSet& reset(size_t pos);

    // Операторы
    BitSet& operator~();
    BitSet operator&(const BitSet& other) const;
    BitSet operator|(const BitSet& other) const;
    BitSet& operator&=(const BitSet& other);
    BitSet& operator|=(const BitSet& other);

    // Методы
    bool all() const;
    bool any() const;
    bool none() const;
    size_t count() const;
    size_t size() const noexcept;
    void flip();
    void flip(size_t pos);

    std::string to_string() const;
    unsigned long long to_ullong() const;
    unsigned long to_ulong() const;

private:
    size_t m_size;
    std::vector<uint64_t> m_bits;
    static constexpr size_t BITS_PER_WORD = 64;
    size_t wordIndex(size_t pos) const;
    uint64_t bitMask(size_t pos) const;
};

#endif // BITSET_H