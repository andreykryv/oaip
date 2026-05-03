#pragma once
#include <stdexcept>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

// ═══════════════════════════════════════════════════════════════
//  Задание 1 — Двухсвязная очередь + два кольца (Вариант 7)
//  Создать двухсвязную очередь из случайных целых чисел.
//  Из элементов, расположенных между максимальным и минимальным,
//  создать первое кольцо. Остальные элементы — второе кольцо.
// ═══════════════════════════════════════════════════════════════

template<typename T>
struct DQNode {
    T data;
    DQNode* prev = nullptr;
    DQNode* next = nullptr;
    explicit DQNode(T d) : data(d) {}
};

template<typename T>
class DoublyQueue {
public:
    DQNode<T>* head = nullptr;
    DQNode<T>* tail = nullptr;
    int size_ = 0;

    ~DoublyQueue() { clear(); }

    void push_back(T val) {
        auto* n = new DQNode<T>(val);
        if (!tail) { head = tail = n; }
        else { n->prev = tail; tail->next = n; tail = n; }
        ++size_;
    }

    void push_front(T val) {
        auto* n = new DQNode<T>(val);
        if (!head) { head = tail = n; }
        else { n->next = head; head->prev = n; head = n; }
        ++size_;
    }

    T pop_front() {
        if (!head) throw std::underflow_error("Queue is empty");
        T d = head->data;
        auto* old = head;
        head = head->next;
        if (head) head->prev = nullptr; else tail = nullptr;
        delete old; --size_;
        return d;
    }

    T pop_back() {
        if (!tail) throw std::underflow_error("Queue is empty");
        T d = tail->data;
        auto* old = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr; else head = nullptr;
        delete old; --size_;
        return d;
    }

    bool empty() const { return size_ == 0; }
    int  size()  const { return size_; }

    void clear() {
        while (head) {
            auto* n = head->next;
            delete head;
            head = n;
        }
        tail = nullptr;
        size_ = 0;
    }

    // Итерация по всем узлам
    void forEach(std::function<void(DQNode<T>*)> fn) const {
        for (auto* n = head; n; n = n->next) fn(n);
    }

    // Получить все значения как вектор
    std::vector<T> toVector() const {
        std::vector<T> v;
        for (auto* n = head; n; n = n->next) v.push_back(n->data);
        return v;
    }

    DQNode<T>* findMax() const {
        if (!head) return nullptr;
        DQNode<T>* m = head;
        for (auto* n = head->next; n; n = n->next)
            if (n->data > m->data) m = n;
        return m;
    }

    DQNode<T>* findMin() const {
        if (!head) return nullptr;
        DQNode<T>* m = head;
        for (auto* n = head->next; n; n = n->next)
            if (n->data < m->data) m = n;
        return m;
    }
};

// ─── Кольцо (circular doubly-linked list) ─────────────────────
template<typename T>
struct RingNode {
    T data;
    RingNode* prev = nullptr;
    RingNode* next = nullptr;
    explicit RingNode(T d) : data(d) {}
};

template<typename T>
class Ring {
public:
    RingNode<T>* start = nullptr;
    int size_ = 0;

    ~Ring() { clear(); }

    void append(T val) {
        auto* n = new RingNode<T>(val);
        if (!start) {
            start = n;
            n->prev = n->next = n;
        } else {
            auto* last = start->prev;
            last->next = n;
            n->prev = last;
            n->next = start;
            start->prev = n;
        }
        ++size_;
    }

    void clear() {
        if (!start) return;
        auto* cur = start->next;
        while (cur != start) {
            auto* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        delete start;
        start = nullptr;
        size_ = 0;
    }

    std::vector<T> toVector() const {
        std::vector<T> v;
        if (!start) return v;
        auto* cur = start;
        do { v.push_back(cur->data); cur = cur->next; } while (cur != start);
        return v;
    }
};

// ─── Алгоритм варианта 7 ──────────────────────────────────────
struct RingResult7 {
    Ring<int> ring1;  // между max и min
    Ring<int> ring2;  // остальные
    int maxVal, minVal;
    int maxPos, minPos;

    RingResult7() : maxVal(0), minVal(0), maxPos(0), minPos(0) {}
    RingResult7(const RingResult7&) = delete;
    RingResult7& operator=(const RingResult7&) = delete;
};

inline std::unique_ptr<RingResult7> buildRings(const DoublyQueue<int>& q) {
    auto res = std::make_unique<RingResult7>();
    if (q.empty()) return res;

    // Найти позиции max и min
    int pos = 0, maxPos = 0, minPos = 0;
    int maxVal = INT_MIN, minVal = INT_MAX;
    for (auto* n = q.head; n; n = n->next, ++pos) {
        if (n->data > maxVal) { maxVal = n->data; maxPos = pos; }
        if (n->data < minVal) { minVal = n->data; minPos = pos; }
    }
    res->maxVal = maxVal; res->minVal = minVal;
    res->maxPos = maxPos; res->minPos = minPos;

    // Упорядочить: lo < hi
    int lo = std::min(maxPos, minPos);
    int hi = std::max(maxPos, minPos);

    // Обходим очередь и распределяем
    pos = 0;
    for (auto* n = q.head; n; n = n->next, ++pos) {
        if (pos > lo && pos < hi)
            res->ring1.append(n->data);   // между max и min
        else
            res->ring2.append(n->data);   // все остальные
    }
    return res;
}