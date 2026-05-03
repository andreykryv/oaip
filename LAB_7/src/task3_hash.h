#pragma once
#include <vector>
#include <string>
#include <climits>
#include <functional>

// ═══════════════════════════════════════════════════════════════
//  Задание 3 — Хеш-таблица на основе массива стеков (Вариант 7)
//  Создать хеш-таблицу из случайных целых чисел и найти в ней
//  номер стека, содержащего минимальное значение ключа.
// ═══════════════════════════════════════════════════════════════

struct StackNode3 {
    int key;
    std::string value;
    StackNode3* next = nullptr;
    StackNode3(int k, std::string v) : key(k), value(std::move(v)), next(nullptr) {}
};

class HashTableStacks {
public:
    int tableSize;
    std::vector<StackNode3*> buckets;

    explicit HashTableStacks(int sz = 11) : tableSize(sz), buckets(sz, nullptr) {}

    ~HashTableStacks() { clear(); }

    // Хеш-функция: деление
    int hash(int key) const {
        return ((key % tableSize) + tableSize) % tableSize;
    }

    // Вставка (push на вершину стека)
    void insert(int key, const std::string& val = "") {
        int idx = hash(key);
        auto* n = new StackNode3(key, val.empty() ? std::to_string(key) : val);
        n->next = buckets[idx];
        buckets[idx] = n;
    }

    // Удаление
    bool remove(int key) {
        int idx = hash(key);
        StackNode3** cur = &buckets[idx];
        while (*cur) {
            if ((*cur)->key == key) {
                auto* del = *cur;
                *cur = del->next;
                delete del;
                return true;
            }
            cur = &(*cur)->next;
        }
        return false;
    }

    // Поиск
    StackNode3* search(int key) const {
        int idx = hash(key);
        for (auto* n = buckets[idx]; n; n = n->next)
            if (n->key == key) return n;
        return nullptr;
    }

    // Вариант 7: найти номер стека с минимальным ключом
    int findStackWithMinKey() const {
        int minVal = INT_MAX;
        int minStack = -1;
        for (int i = 0; i < tableSize; ++i) {
            for (auto* n = buckets[i]; n; n = n->next) {
                if (n->key < minVal) {
                    minVal = n->key;
                    minStack = i;
                }
            }
        }
        return minStack;
    }

    int getMinKey() const {
        int minVal = INT_MAX;
        for (int i = 0; i < tableSize; ++i)
            for (auto* n = buckets[i]; n; n = n->next)
                if (n->key < minVal) minVal = n->key;
        return minVal == INT_MAX ? 0 : minVal;
    }

    void clear() {
        for (auto*& b : buckets) {
            while (b) { auto* nx = b->next; delete b; b = nx; }
            b = nullptr;
        }
    }

    // Список элементов в стеке i
    std::vector<std::pair<int,std::string>> getStack(int i) const {
        std::vector<std::pair<int,std::string>> v;
        for (auto* n = buckets[i]; n; n = n->next)
            v.push_back({n->key, n->value});
        return v;
    }

    int totalElements() const {
        int cnt = 0;
        for (int i = 0; i < tableSize; ++i)
            for (auto* n = buckets[i]; n; n = n->next) ++cnt;
        return cnt;
    }
};