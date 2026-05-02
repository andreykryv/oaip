#include "bitset.h"
#include <sstream>
#include <iomanip>
#include <climits>

// ─── Конструктор ──────────────────────────────────────────
BitSet::BitSet(size_t size) : m_size(size)
{
    size_t words = (size + BITS_PER_WORD - 1) / BITS_PER_WORD;
    m_bits.assign(words, 0ULL);
}

// ─── Вспомогательные ──────────────────────────────────────
size_t BitSet::wordIndex(size_t pos) const
{
    if (pos >= m_size) throw std::out_of_range("BitSet: индекс выходит за пределы");
    return pos / BITS_PER_WORD;
}

uint64_t BitSet::bitMask(size_t pos) const
{
    return 1ULL << (pos % BITS_PER_WORD);
}

void BitSet::maskLastWord()
{
    size_t rem = m_size % BITS_PER_WORD;
    if (rem != 0)
        m_bits.back() &= (1ULL << rem) - 1;
}

// ─── Доступ ───────────────────────────────────────────────
bool BitSet::operator[](size_t pos) const
{
    return (m_bits[wordIndex(pos)] & bitMask(pos)) != 0;
}
bool BitSet::test(size_t pos) const { return (*this)[pos]; }

BitSet& BitSet::set(size_t pos, bool value)
{
    if (value) m_bits[wordIndex(pos)] |=  bitMask(pos);
    else       m_bits[wordIndex(pos)] &= ~bitMask(pos);
    return *this;
}

BitSet& BitSet::reset(size_t pos)
{
    m_bits[wordIndex(pos)] &= ~bitMask(pos);
    return *this;
}

BitSet& BitSet::setAll()
{
    for (auto &w : m_bits) w = ~0ULL;
    maskLastWord();
    return *this;
}

BitSet& BitSet::resetAll()
{
    for (auto &w : m_bits) w = 0ULL;
    return *this;
}

// ─── Унарные операторы ────────────────────────────────────
BitSet& BitSet::operator~()
{
    for (auto &w : m_bits) w = ~w;
    maskLastWord();
    return *this;
}

// ─── Бинарные операторы (новый объект) ───────────────────
static void checkSizes(const BitSet &a, const BitSet &b)
{
    if (a.size() != b.size())
        throw std::invalid_argument("BitSet: размеры не совпадают");
}

BitSet BitSet::operator&(const BitSet &o) const
{
    checkSizes(*this, o);
    BitSet r(m_size);
    for (size_t i = 0; i < m_bits.size(); ++i)
        r.m_bits[i] = m_bits[i] & o.m_bits[i];
    return r;
}

BitSet BitSet::operator|(const BitSet &o) const
{
    checkSizes(*this, o);
    BitSet r(m_size);
    for (size_t i = 0; i < m_bits.size(); ++i)
        r.m_bits[i] = m_bits[i] | o.m_bits[i];
    return r;
}

BitSet BitSet::operator^(const BitSet &o) const
{
    checkSizes(*this, o);
    BitSet r(m_size);
    for (size_t i = 0; i < m_bits.size(); ++i)
        r.m_bits[i] = m_bits[i] ^ o.m_bits[i];
    return r;
}

BitSet BitSet::operator<<(size_t n) const
{
    BitSet r(m_size);
    if (n >= m_size) return r;
    for (size_t i = n; i < m_size; ++i)
        if (test(i - n)) r.set(i);
    return r;
}

BitSet BitSet::operator>>(size_t n) const
{
    BitSet r(m_size);
    if (n >= m_size) return r;
    for (size_t i = 0; i + n < m_size; ++i)
        if (test(i + n)) r.set(i);
    return r;
}

// ─── In-place бинарные ────────────────────────────────────
BitSet& BitSet::operator&=(const BitSet &o)
{
    checkSizes(*this, o);
    for (size_t i = 0; i < m_bits.size(); ++i) m_bits[i] &= o.m_bits[i];
    return *this;
}
BitSet& BitSet::operator|=(const BitSet &o)
{
    checkSizes(*this, o);
    for (size_t i = 0; i < m_bits.size(); ++i) m_bits[i] |= o.m_bits[i];
    return *this;
}
BitSet& BitSet::operator^=(const BitSet &o)
{
    checkSizes(*this, o);
    for (size_t i = 0; i < m_bits.size(); ++i) m_bits[i] ^= o.m_bits[i];
    return *this;
}

// ─── Предикаты ────────────────────────────────────────────
bool BitSet::all() const
{
    if (m_size == 0) return true;
    for (size_t i = 0; i < m_bits.size() - 1; ++i)
        if (m_bits[i] != ~0ULL) return false;
    size_t rem = m_size % BITS_PER_WORD;
    uint64_t mask = (rem == 0) ? ~0ULL : (1ULL << rem) - 1;
    return (m_bits.back() & mask) == mask;
}
bool BitSet::any()  const { for (auto w : m_bits) if (w) return true;  return false; }
bool BitSet::none() const { return !any(); }

// ─── Счёт единиц ─────────────────────────────────────────
size_t BitSet::count() const
{
    size_t c = 0;
    for (uint64_t w : m_bits)
        while (w) { w &= w - 1; ++c; }  // Kernighan bit count
    return c;
}

size_t BitSet::size() const noexcept { return m_size; }

// ─── Флип ─────────────────────────────────────────────────
BitSet& BitSet::flip()              { return operator~(); }
BitSet& BitSet::flip(size_t pos)
{
    m_bits[wordIndex(pos)] ^= bitMask(pos);
    return *this;
}

// ─── Конвертации ─────────────────────────────────────────
std::string BitSet::to_string() const
{
    std::string s(m_size, '0');
    for (size_t i = 0; i < m_size; ++i)
        if (test(i)) s[m_size - 1 - i] = '1';
    return s;
}

std::string BitSet::to_hex() const
{
    if (m_size == 0) return "0x0";
    // Сколько нибблов нужно
    size_t nibbles = (m_size + 3) / 4;
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase << std::setfill('0');
    // Идём от старшего к младшему ниблу
    for (size_t n = nibbles; n > 0; --n) {
        size_t bitStart = (n - 1) * 4;
        uint8_t nibble = 0;
        for (size_t b = 0; b < 4; ++b) {
            size_t idx = bitStart + b;
            if (idx < m_size && test(idx))
                nibble |= (1 << b);
        }
        oss << std::setw(1) << (int)nibble;
    }
    return oss.str();
}

unsigned long long BitSet::to_ullong() const
{
    if (m_size > sizeof(unsigned long long) * CHAR_BIT)
        throw std::overflow_error("BitSet слишком большой для unsigned long long");
    unsigned long long val = 0;
    for (size_t i = 0; i < m_size; ++i)
        if (test(i)) val |= 1ULL << i;
    return val;
}

unsigned long BitSet::to_ulong() const
{
    if (m_size > sizeof(unsigned long) * CHAR_BIT)
        throw std::overflow_error("BitSet слишком большой для unsigned long");
    unsigned long val = 0;
    for (size_t i = 0; i < m_size; ++i)
        if (test(i)) val |= 1UL << i;
    return val;
}