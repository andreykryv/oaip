#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <cmath>

// ═══════════════════════════════════════════════════════════════
//  Задание 4 — Хеш-таблица с открытой адресацией (Вариант 7)
//  Изменить функцию вычисления хеш на универсальную.
//  Исследовать время поиска для размеров: 16, 64, 128, 2048.
// ═══════════════════════════════════════════════════════════════

enum class CellState { EMPTY, OCCUPIED, DELETED };

struct HashCell {
    int key = 0;
    std::string value;
    CellState state = CellState::EMPTY;
    int collisions = 0;   // сколько коллизий при вставке
};

class UniversalHashTable {
public:
    int capacity;
    int size_ = 0;
    int totalCollisions = 0;
    std::vector<HashCell> table;

    // Параметры универсальной хеш-функции
    // h(k) = ((a*k + b) mod p) mod m
    // где p — простое, a ∈ [1, p-1], b ∈ [0, p-1]
    long long a, b, p;

    explicit UniversalHashTable(int cap, unsigned seed = 42) : capacity(cap), table(cap) {
        // Большое простое число
        p = 1000000007LL;
        std::mt19937 rng(seed);
        a = std::uniform_int_distribution<long long>(1, p-1)(rng);
        b = std::uniform_int_distribution<long long>(0, p-1)(rng);
    }

    // Универсальная хеш-функция
    int hash(int key) const {
        long long h = ((a * (long long)key + b) % p);
        if (h < 0) h += p;
        return (int)(h % capacity);
    }

    // Линейное зондирование для разрешения коллизий
    bool insert(int key, const std::string& val = "") {
        if (size_ >= capacity) return false;
        int h = hash(key);
        int cols = 0;
        for (int i = 0; i < capacity; ++i) {
            int idx = (h + i) % capacity;
            if (table[idx].state == CellState::EMPTY ||
                table[idx].state == CellState::DELETED) {
                table[idx] = {key, val.empty() ? std::to_string(key) : val,
                               CellState::OCCUPIED, cols};
                ++size_;
                totalCollisions += cols;
                return true;
            }
            if (table[idx].key == key) return false; // дубль
            ++cols;
        }
        return false;
    }

    // Поиск: возвращает индекс или -1
    int search(int key) const {
        int h = hash(key);
        for (int i = 0; i < capacity; ++i) {
            int idx = (h + i) % capacity;
            if (table[idx].state == CellState::EMPTY) return -1;
            if (table[idx].state == CellState::OCCUPIED && table[idx].key == key)
                return idx;
        }
        return -1;
    }

    bool remove(int key) {
        int idx = search(key);
        if (idx < 0) return false;
        table[idx].state = CellState::DELETED;
        --size_;
        return true;
    }

    double loadFactor() const { return capacity ? (double)size_ / capacity : 0.0; }

    void clear() {
        for (auto& c : table) c = HashCell{};
        size_ = 0; totalCollisions = 0;
    }
};

// ─── Измерение времени поиска ──────────────────────────────────
struct SearchBenchmark {
    int tableSize;
    double avgSearchTimeNs;    // среднее время успешного поиска (нс)
    double avgFailTimeNs;      // среднее время неуспешного поиска (нс)
    int    avgProbes;          // среднее число зондирований
    double loadFactor;
    int    collisions;
};

inline SearchBenchmark runBenchmark(int tableSize, int nInsert, unsigned seed) {
    UniversalHashTable ht(tableSize, seed);
    std::mt19937 rng(seed);
    std::vector<int> keys;

    int toInsert = std::min(nInsert, (int)(tableSize * 0.75));
    for (int i = 0; i < toInsert; ++i) {
        int k = std::uniform_int_distribution<int>(-10000, 10000)(rng);
        keys.push_back(k);
        ht.insert(k);
    }

    // Измеряем успешный поиск
    long long totalSuccNs = 0;
    int nSearch = std::min(1000, (int)keys.size());
    for (int i = 0; i < nSearch; ++i) {
        auto t0 = std::chrono::high_resolution_clock::now();
        ht.search(keys[i % keys.size()]);
        auto t1 = std::chrono::high_resolution_clock::now();
        totalSuccNs += std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count();
    }

    // Измеряем неуспешный поиск
    long long totalFailNs = 0;
    for (int i = 0; i < nSearch; ++i) {
        int fk = 999999 + i; // гарантированно не в таблице
        auto t0 = std::chrono::high_resolution_clock::now();
        ht.search(fk);
        auto t1 = std::chrono::high_resolution_clock::now();
        totalFailNs += std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count();
    }

    SearchBenchmark b;
    b.tableSize = tableSize;
    b.avgSearchTimeNs = nSearch > 0 ? (double)totalSuccNs / nSearch : 0;
    b.avgFailTimeNs   = nSearch > 0 ? (double)totalFailNs  / nSearch : 0;
    b.avgProbes   = 0;
    b.loadFactor  = ht.loadFactor();
    b.collisions  = ht.totalCollisions;
    return b;
}

inline std::vector<SearchBenchmark> runAllBenchmarks() {
    std::vector<int> sizes = {16, 64, 128, 2048};
    std::vector<SearchBenchmark> results;
    for (int sz : sizes) {
        results.push_back(runBenchmark(sz, (int)(sz * 0.7), 1234 + sz));
    }
    return results;
}