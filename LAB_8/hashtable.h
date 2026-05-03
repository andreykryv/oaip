#pragma once
// ╔══════════════════════════════════════════════════════════════╗
// ║  HashTable<K,V,Hash> — аналог std::unordered_map            ║
// ║  Метод цепочек: std::forward_list для коллизий              ║
// ╚══════════════════════════════════════════════════════════════╝
#include <forward_list>
#include <vector>
#include <functional>
#include <optional>
#include <stdexcept>
#include <cstddef>
#include <utility>

template<
    typename KeyType,
    typename ValueType,
    typename Hash = std::hash<KeyType>
>
class HashTable {
public:
    using PairType  = std::pair<const KeyType, ValueType>;
    using Chain     = std::forward_list<PairType>;

    explicit HashTable(size_t initialCapacity = 16, float maxLoad = 0.75f, Hash hash = Hash{})
        : m_buckets(initialCapacity), m_hash(hash), m_maxLoad(maxLoad) {}

    // ── Вставка ─────────────────────────────────────────────────
    void insert(const KeyType& key, const ValueType& val) {
        if (loadFactor() > m_maxLoad) rehash(m_buckets.size() * 2);

        size_t idx = bucketIndex(key);
        for (auto& pair : m_buckets[idx]) {
            if (pair.first == key) {
                pair.second = val;  // обновляем
                return;
            }
        }
        m_buckets[idx].emplace_front(key, val);
        ++m_size;
    }

    // ── Удаление ────────────────────────────────────────────────
    bool erase(const KeyType& key) {
        size_t idx = bucketIndex(key);
        auto& chain = m_buckets[idx];
        auto prev = chain.before_begin();
        for (auto it = chain.begin(); it != chain.end(); ++it, ++prev) {
            if (it->first == key) {
                chain.erase_after(prev);
                --m_size;
                return true;
            }
        }
        return false;
    }

    // ── Поиск ───────────────────────────────────────────────────
    bool contains(const KeyType& key) const {
        size_t idx = bucketIndex(key);
        for (auto& pair : m_buckets[idx]) {
            if (pair.first == key) return true;
        }
        return false;
    }

    std::optional<ValueType> get(const KeyType& key) const {
        size_t idx = bucketIndex(key);
        for (auto& pair : m_buckets[idx]) {
            if (pair.first == key) return pair.second;
        }
        return std::nullopt;
    }

    // ── Оператор [] (создаёт элемент если отсутствует) ─────────
    ValueType& operator[](const KeyType& key) {
        if (loadFactor() > m_maxLoad) rehash(m_buckets.size() * 2);
        size_t idx = bucketIndex(key);
        for (auto& pair : m_buckets[idx]) {
            if (pair.first == key) return pair.second;
        }
        m_buckets[idx].emplace_front(key, ValueType{});
        ++m_size;
        return m_buckets[bucketIndex(key)].front().second;
    }

    ValueType operator[](const KeyType& key) const {
        auto opt = get(key);
        if (!opt) throw std::out_of_range("HashTable: key not found");
        return *opt;
    }

    // ── Rehash ──────────────────────────────────────────────────
    void rehash(size_t newCapacity) {
        if (newCapacity == 0) newCapacity = 16;
        std::vector<Chain> newBuckets(newCapacity);
        for (auto& chain : m_buckets) {
            for (auto& pair : chain) {
                size_t idx = m_hash(pair.first) % newCapacity;
                newBuckets[idx].emplace_front(pair.first, pair.second);
            }
        }
        m_buckets = std::move(newBuckets);
    }

    // ── Очистка ─────────────────────────────────────────────────
    void clear() {
        for (auto& chain : m_buckets) chain.clear();
        m_size = 0;
    }

    // ── Информационные методы ───────────────────────────────────
    size_t size()     const { return m_size; }
    bool   empty()    const { return m_size == 0; }
    size_t capacity() const { return m_buckets.size(); }
    float  loadFactor() const {
        return m_buckets.empty() ? 0.f : (float)m_size / m_buckets.size();
    }

    // Для визуализации: содержимое каждого бакета
    struct BucketInfo {
        size_t index;
        std::vector<std::pair<KeyType, ValueType>> items;
    };

    std::vector<BucketInfo> getBuckets() const {
        std::vector<BucketInfo> result;
        for (size_t i = 0; i < m_buckets.size(); ++i) {
            BucketInfo bi;
            bi.index = i;
            for (auto& pair : m_buckets[i]) {
                bi.items.push_back({pair.first, pair.second});
            }
            result.push_back(bi);
        }
        return result;
    }

    // Индекс бакета для ключа (для визуализации)
    size_t bucketFor(const KeyType& key) const { return bucketIndex(key); }

private:
    std::vector<Chain> m_buckets;
    Hash   m_hash;
    float  m_maxLoad;
    size_t m_size = 0;

    size_t bucketIndex(const KeyType& key) const {
        return m_hash(key) % m_buckets.size();
    }
};
