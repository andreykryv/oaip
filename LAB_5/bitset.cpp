#include "bitset.h"
#include <sstream>
#include <climits>

BitSet::BitSet(size_t size) : m_size(size)
{
    size_t words = (size + BITS_PER_WORD - 1) / BITS_PER_WORD;
    m_bits.resize(words, 0);
}

size_t BitSet::wordIndex(size_t pos) const
{
    if (pos >= m_size)
        throw std::out_of_range("BitSet index out of range");
    return pos / BITS_PER_WORD;
}

uint64_t BitSet::bitMask(size_t pos) const
{
    return 1ULL << (pos % BITS_PER_WORD);
}

bool BitSet::operator[](size_t pos) const
{
    return (m_bits[wordIndex(pos)] & bitMask(pos)) != 0;
}

bool BitSet::test(size_t pos) const
{
    return (*this)[pos];
}

BitSet& BitSet::set(size_t pos, bool value)
{
    if (value)
        m_bits[wordIndex(pos)] |= bitMask(pos);
    else
        m_bits[wordIndex(pos)] &= ~bitMask(pos);
    return *this;
}

BitSet& BitSet::reset(size_t pos)
{
    m_bits[wordIndex(pos)] &= ~bitMask(pos);
    return *this;
}

BitSet& BitSet::operator~()
{
    for (auto& word : m_bits)
        word = ~word;
    // Обнуляем лишние биты в последнем слове
    size_t remaining = m_size % BITS_PER_WORD;
    if (remaining != 0)
        m_bits.back() &= (1ULL << remaining) - 1;
    return *this;
}

BitSet BitSet::operator&(const BitSet& other) const
{
    if (m_size != other.m_size)
        throw std::invalid_argument("BitSet sizes differ");
    BitSet result(m_size);
    for (size_t i = 0; i < m_bits.size(); ++i)
        result.m_bits[i] = m_bits[i] & other.m_bits[i];
    return result;
}

BitSet BitSet::operator|(const BitSet& other) const
{
    if (m_size != other.m_size)
        throw std::invalid_argument("BitSet sizes differ");
    BitSet result(m_size);
    for (size_t i = 0; i < m_bits.size(); ++i)
        result.m_bits[i] = m_bits[i] | other.m_bits[i];
    return result;
}

BitSet& BitSet::operator&=(const BitSet& other)
{
    if (m_size != other.m_size)
        throw std::invalid_argument("BitSet sizes differ");
    for (size_t i = 0; i < m_bits.size(); ++i)
        m_bits[i] &= other.m_bits[i];
    return *this;
}

BitSet& BitSet::operator|=(const BitSet& other)
{
    if (m_size != other.m_size)
        throw std::invalid_argument("BitSet sizes differ");
    for (size_t i = 0; i < m_bits.size(); ++i)
        m_bits[i] |= other.m_bits[i];
    return *this;
}

bool BitSet::all() const
{
    if (m_size == 0) return true;
    for (size_t i = 0; i < m_bits.size() - 1; ++i)
        if (m_bits[i] != ~0ULL) return false;
    size_t remaining = m_size % BITS_PER_WORD;
    uint64_t lastMask = (remaining == 0) ? ~0ULL : (1ULL << remaining) - 1;
    return (m_bits.back() & lastMask) == lastMask;
}

bool BitSet::any() const
{
    for (auto word : m_bits)
        if (word != 0) return true;
    return false;
}

bool BitSet::none() const
{
    return !any();
}

size_t BitSet::count() const
{
    size_t c = 0;
    for (auto word : m_bits) {
        while (word) {
            word &= word - 1; // Очистка младшего установленного бита
            ++c;
        }
    }
    return c;
}

size_t BitSet::size() const noexcept
{
    return m_size;
}

void BitSet::flip()
{
    operator~();
}

void BitSet::flip(size_t pos)
{
    m_bits[wordIndex(pos)] ^= bitMask(pos);
}

std::string BitSet::to_string() const
{
    std::string result(m_size, '0');
    for (size_t i = 0; i < m_size; ++i)
        if (test(i)) result[m_size - 1 - i] = '1'; // старший бит слева
    return result;
}

unsigned long long BitSet::to_ullong() const
{
    if (m_size > sizeof(unsigned long long) * CHAR_BIT)
        throw std::overflow_error("Bitset too large to convert to unsigned long long");
    unsigned long long val = 0;
    for (size_t i = 0; i < m_size; ++i)
        if (test(i)) val |= 1ULL << i;
    return val;
}

unsigned long BitSet::to_ulong() const
{
    if (m_size > sizeof(unsigned long) * CHAR_BIT)
        throw std::overflow_error("Bitset too large to convert to unsigned long");
    unsigned long val = 0;
    for (size_t i = 0; i < m_size; ++i)
        if (test(i)) val |= 1UL << i;
    return val;
}