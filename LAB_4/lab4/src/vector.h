#pragma once
#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <new>

// ═══════════════════════════════════════════════════════════
//  Custom Vector<T>  —  аналог std::vector (Задание 4)
// ═══════════════════════════════════════════════════════════

template<typename T>
class Vector {
public:

    // ── Итератор ──────────────────────────────────────────
    class Iterator {
    public:
        using difference_type = std::ptrdiff_t;
        explicit Iterator(T* ptr) : m_ptr(ptr) {}

        T&        operator*()  const { return *m_ptr; }
        T*        operator->() const { return m_ptr; }
        Iterator& operator++()       { ++m_ptr; return *this; }
        Iterator  operator++(int)    { Iterator t=*this; ++m_ptr; return t; }
        Iterator& operator--()       { --m_ptr; return *this; }
        Iterator  operator--(int)    { Iterator t=*this; --m_ptr; return t; }
        Iterator  operator+(difference_type n) const { return Iterator(m_ptr+n); }
        Iterator  operator-(difference_type n) const { return Iterator(m_ptr-n); }
        difference_type operator-(const Iterator& o) const { return m_ptr-o.m_ptr; }
        bool operator==(const Iterator& o) const { return m_ptr==o.m_ptr; }
        bool operator!=(const Iterator& o) const { return m_ptr!=o.m_ptr; }
        bool operator< (const Iterator& o) const { return m_ptr< o.m_ptr; }
        bool operator<=(const Iterator& o) const { return m_ptr<=o.m_ptr; }
        bool operator> (const Iterator& o) const { return m_ptr> o.m_ptr; }
        bool operator>=(const Iterator& o) const { return m_ptr>=o.m_ptr; }
        T&   operator[](difference_type n) const { return m_ptr[n]; }
        T* raw() const { return m_ptr; }
    private:
        T* m_ptr;
    };

    // ── Константный итератор ──────────────────────────────
    class ConstIterator {
    public:
        using difference_type = std::ptrdiff_t;
        explicit ConstIterator(const T* ptr) : m_ptr(ptr) {}

        const T&       operator*()  const { return *m_ptr; }
        const T*       operator->() const { return m_ptr; }
        ConstIterator& operator++()       { ++m_ptr; return *this; }
        ConstIterator  operator++(int)    { ConstIterator t=*this; ++m_ptr; return t; }
        ConstIterator& operator--()       { --m_ptr; return *this; }
        ConstIterator  operator--(int)    { ConstIterator t=*this; --m_ptr; return t; }
        ConstIterator  operator+(difference_type n) const { return ConstIterator(m_ptr+n); }
        ConstIterator  operator-(difference_type n) const { return ConstIterator(m_ptr-n); }
        difference_type operator-(const ConstIterator& o) const { return m_ptr-o.m_ptr; }
        bool operator==(const ConstIterator& o) const { return m_ptr==o.m_ptr; }
        bool operator!=(const ConstIterator& o) const { return m_ptr!=o.m_ptr; }
    private:
        const T* m_ptr;
    };

    // ── Обратный итератор ──────────────────────────────────
    class ReverseIterator {
    public:
        explicit ReverseIterator(T* ptr) : m_ptr(ptr) {}
        T&              operator*()  const { return *m_ptr; }
        T*              operator->() const { return m_ptr; }
        ReverseIterator& operator++() { --m_ptr; return *this; }
        ReverseIterator  operator++(int) { ReverseIterator t=*this; --m_ptr; return t; }
        ReverseIterator& operator--() { ++m_ptr; return *this; }
        ReverseIterator  operator--(int) { ReverseIterator t=*this; ++m_ptr; return t; }
        bool operator==(const ReverseIterator& o) const { return m_ptr==o.m_ptr; }
        bool operator!=(const ReverseIterator& o) const { return m_ptr!=o.m_ptr; }
    private:
        T* m_ptr;
    };

    // ── Конструкторы / деструктор ─────────────────────────
    Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

    explicit Vector(std::size_t count, const T& val = T())
        : m_size(count), m_capacity(count)
    {
        m_data = allocate(m_capacity);
        for (std::size_t i = 0; i < m_size; ++i)
            ::new(&m_data[i]) T(val);
    }

    Vector(std::initializer_list<T> il)
        : m_size(il.size()), m_capacity(il.size())
    {
        m_data = allocate(m_capacity);
        std::size_t i = 0;
        for (const auto& v : il)
            ::new(&m_data[i++]) T(v);
    }

    Vector(const Vector& o) : m_size(o.m_size), m_capacity(o.m_capacity) {
        m_data = allocate(m_capacity);
        for (std::size_t i = 0; i < m_size; ++i)
            ::new(&m_data[i]) T(o.m_data[i]);
    }

    Vector(Vector&& o) noexcept
        : m_data(o.m_data), m_size(o.m_size), m_capacity(o.m_capacity)
    { o.m_data=nullptr; o.m_size=0; o.m_capacity=0; }

    ~Vector() { destroy_all(); deallocate(m_data); }

    Vector& operator=(const Vector& o) {
        if (this != &o) { Vector tmp(o); swap(tmp); }
        return *this;
    }
    Vector& operator=(Vector&& o) noexcept {
        if (this != &o) { Vector tmp(std::move(o)); swap(tmp); }
        return *this;
    }

    // ── Доступ к элементам ────────────────────────────────
    T& at(std::size_t pos) {
        if (pos >= m_size) throw std::out_of_range("Vector::at — индекс за пределами");
        return m_data[pos];
    }
    const T& at(std::size_t pos) const {
        if (pos >= m_size) throw std::out_of_range("Vector::at — индекс за пределами");
        return m_data[pos];
    }
    T&       operator[](std::size_t pos)       { return m_data[pos]; }
    const T& operator[](std::size_t pos) const { return m_data[pos]; }
    T&       front()       { return m_data[0]; }
    const T& front() const { return m_data[0]; }
    T&       back()        { return m_data[m_size-1]; }
    const T& back()  const { return m_data[m_size-1]; }
    T*       data()        { return m_data; }
    const T* data()  const { return m_data; }

    // ── Итераторы ─────────────────────────────────────────
    Iterator      begin()        { return Iterator(m_data); }
    Iterator      end()          { return Iterator(m_data + m_size); }
    ConstIterator begin()  const { return ConstIterator(m_data); }
    ConstIterator end()    const { return ConstIterator(m_data + m_size); }
    ConstIterator cbegin() const { return ConstIterator(m_data); }
    ConstIterator cend()   const { return ConstIterator(m_data + m_size); }
    ReverseIterator rbegin() { return ReverseIterator(m_data + m_size - 1); }
    ReverseIterator rend()   { return ReverseIterator(m_data - 1); }

    // ── Вместимость ───────────────────────────────────────
    bool        empty()    const { return m_size == 0; }
    std::size_t size()     const { return m_size; }
    std::size_t capacity() const { return m_capacity; }
    std::size_t max_size() const { return static_cast<std::size_t>(-1) / sizeof(T); }

    void reserve(std::size_t new_cap) {
        if (new_cap <= m_capacity) return;
        T* nd = allocate(new_cap);
        for (std::size_t i = 0; i < m_size; ++i) {
            ::new(&nd[i]) T(std::move(m_data[i]));
            m_data[i].~T();
        }
        deallocate(m_data);
        m_data = nd;
        m_capacity = new_cap;
    }

    void resize(std::size_t count, const T& val = T()) {
        if (count > m_capacity) reserve(count);
        if (count > m_size)
            for (std::size_t i = m_size; i < count; ++i)
                ::new(&m_data[i]) T(val);
        else
            for (std::size_t i = count; i < m_size; ++i)
                m_data[i].~T();
        m_size = count;
    }

    // ── Модификаторы ─────────────────────────────────────
    void clear() {
        destroy_all();
        m_size = 0;
    }

    void push_back(const T& val) {
        grow_if_needed();
        ::new(&m_data[m_size++]) T(val);
    }
    void push_back(T&& val) {
        grow_if_needed();
        ::new(&m_data[m_size++]) T(std::move(val));
    }
    void pop_back() {
        if (m_size > 0) { m_data[--m_size].~T(); }
    }

    void assign(std::size_t count, const T& val) {
        clear();
        resize(count, val);
    }
    void assign(std::initializer_list<T> il) {
        clear();
        for (const auto& v : il) push_back(v);
    }

    Iterator insert(Iterator pos, const T& val) {
        std::size_t idx = pos - begin();
        grow_if_needed();
        ::new(&m_data[m_size]) T();
        for (std::size_t i = m_size; i > idx; --i)
            m_data[i] = std::move(m_data[i-1]);
        m_data[idx] = val;
        ++m_size;
        return Iterator(m_data + idx);
    }

    Iterator erase(Iterator pos) {
        std::size_t idx = pos - begin();
        m_data[idx].~T();
        for (std::size_t i = idx; i < m_size-1; ++i)
            m_data[i] = std::move(m_data[i+1]);
        --m_size;
        return Iterator(m_data + idx);
    }

    Iterator erase(Iterator first, Iterator last) {
        std::size_t from = first - begin();
        std::size_t cnt  = last - first;
        for (std::size_t i = from; i < from+cnt; ++i) m_data[i].~T();
        for (std::size_t i = from; i < m_size-cnt; ++i)
            m_data[i] = std::move(m_data[i+cnt]);
        m_size -= cnt;
        return Iterator(m_data + from);
    }

    template<typename... Args>
    Iterator emplace(Iterator pos, Args&&... args) {
        std::size_t idx = pos - begin();
        grow_if_needed();
        ::new(&m_data[m_size]) T();
        for (std::size_t i = m_size; i > idx; --i)
            m_data[i] = std::move(m_data[i-1]);
        m_data[idx].~T();
        ::new(&m_data[idx]) T(std::forward<Args>(args)...);
        ++m_size;
        return Iterator(m_data + idx);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        grow_if_needed();
        ::new(&m_data[m_size++]) T(std::forward<Args>(args)...);
    }

    void swap(Vector& o) noexcept {
        T* td=m_data; std::size_t ts=m_size, tc=m_capacity;
        m_data=o.m_data; m_size=o.m_size; m_capacity=o.m_capacity;
        o.m_data=td; o.m_size=ts; o.m_capacity=tc;
    }

private:
    T*          m_data;
    std::size_t m_size;
    std::size_t m_capacity;

    static T* allocate(std::size_t n) {
        if (n == 0) return nullptr;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    static void deallocate(T* p) { ::operator delete(p); }

    void destroy_all() {
        for (std::size_t i = 0; i < m_size; ++i)
            m_data[i].~T();
    }
    void grow_if_needed() {
        if (m_size == m_capacity)
            reserve(m_capacity == 0 ? 4 : m_capacity * 2);
    }
};
