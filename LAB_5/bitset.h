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
    bool     operator[](size_t pos) const;
    bool     test(size_t pos) const;
    BitSet&  set(size_t pos, bool value = true);
    BitSet&  reset(size_t pos);
    BitSet&  setAll();
    BitSet&  resetAll();

    // Унарные операторы (in-place)
    BitSet&  operator~();          // NOT (in-place)
    // Бинарные операторы (возвращают новый объект)
    BitSet   operator& (const BitSet& other) const;  // AND
    BitSet   operator| (const BitSet& other) const;  // OR
    BitSet   operator^ (const BitSet& other) const;  // XOR
    BitSet   operator<<(size_t n) const;              // сдвиг влево
    BitSet   operator>>(size_t n) const;              // сдвиг вправо
    // Бинарные операторы (in-place)
    BitSet&  operator&=(const BitSet& other);
    BitSet&  operator|=(const BitSet& other);
    BitSet&  operator^=(const BitSet& other);

    // Предикаты
    bool   all()  const;
    bool   any()  const;
    bool   none() const;

    // Метрики
    size_t count()      const;       // число установленных битов
    size_t size()       const noexcept;

    // Флип
    BitSet& flip();                  // все биты
    BitSet& flip(size_t pos);        // один бит

    // Конвертация
    std::string        to_string()  const;
    std::string        to_hex()     const;   // шестнадцатеричное представление
    unsigned long long to_ullong()  const;
    unsigned long      to_ulong()   const;

private:
    size_t m_size;
    std::vector<uint64_t> m_bits;
    static constexpr size_t BITS_PER_WORD = 64;
    size_t   wordIndex(size_t pos) const;
    uint64_t bitMask(size_t pos)   const;
    void     maskLastWord();         // обнуляем лишние биты последнего слова
};

#endif // BITSET_H