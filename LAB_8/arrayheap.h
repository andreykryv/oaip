#pragma once
// ╔══════════════════════════════════════════════════════════════╗
// ║  БИНАРНАЯ КУЧА НА ОСНОВЕ МАССИВА — Статическая библиотека   ║
// ║  Реализация: max-heap с поддержкой любого компаратора        ║
// ╚══════════════════════════════════════════════════════════════╝
#include <vector>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <optional>

template<typename T, typename Compare = std::less<T>>
class ArrayHeap {
public:
    ArrayHeap() = default;
    explicit ArrayHeap(Compare comp) : m_comp(comp) {}

    // ── Основные операции ────────────────────────────────────────
    void push(const T& val) {
        m_data.push_back(val);
        siftUp(m_data.size() - 1);
    }

    void push(T&& val) {
        m_data.push_back(std::move(val));
        siftUp(m_data.size() - 1);
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("ArrayHeap: heap is empty");
        return m_data[0];
    }

    void pop() {
        if (empty()) throw std::underflow_error("ArrayHeap: heap is empty");
        m_data[0] = std::move(m_data.back());
        m_data.pop_back();
        if (!empty()) siftDown(0);
    }

    // ── Информационные методы ────────────────────────────────────
    bool   empty() const noexcept { return m_data.empty(); }
    size_t size()  const noexcept { return m_data.size(); }
    void   clear() noexcept       { m_data.clear(); }

    const std::vector<T>& rawData() const { return m_data; }

    // ── Индексные вспомогательные функции ────────────────────────
    static size_t parentIdx(size_t i) noexcept { return (i - 1) / 2; }
    static size_t leftIdx  (size_t i) noexcept { return 2 * i + 1; }
    static size_t rightIdx (size_t i) noexcept { return 2 * i + 2; }

    // Последний шаг просеивания вверх/вниз для анимации
    struct SiftStep { size_t from, to; };
    std::vector<SiftStep> pushTrace(const T& val) {
        m_data.push_back(val);
        return siftUpTrace(m_data.size() - 1);
    }
    std::vector<SiftStep> popTrace() {
        if (empty()) return {};
        m_data[0] = std::move(m_data.back());
        m_data.pop_back();
        if (!empty()) return siftDownTrace(0);
        return {};
    }

private:
    std::vector<T> m_data;
    Compare m_comp;

    void siftUp(size_t i) {
        while (i > 0) {
            size_t p = parentIdx(i);
            if (m_comp(m_data[p], m_data[i])) {
                std::swap(m_data[p], m_data[i]);
                i = p;
            } else break;
        }
    }

    void siftDown(size_t i) {
        size_t n = m_data.size();
        while (true) {
            size_t best = i;
            size_t l = leftIdx(i), r = rightIdx(i);
            if (l < n && m_comp(m_data[best], m_data[l])) best = l;
            if (r < n && m_comp(m_data[best], m_data[r])) best = r;
            if (best == i) break;
            std::swap(m_data[i], m_data[best]);
            i = best;
        }
    }

    std::vector<SiftStep> siftUpTrace(size_t i) {
        std::vector<SiftStep> steps;
        while (i > 0) {
            size_t p = parentIdx(i);
            if (m_comp(m_data[p], m_data[i])) {
                steps.push_back({i, p});
                std::swap(m_data[p], m_data[i]);
                i = p;
            } else break;
        }
        return steps;
    }

    std::vector<SiftStep> siftDownTrace(size_t i) {
        std::vector<SiftStep> steps;
        size_t n = m_data.size();
        while (true) {
            size_t best = i;
            size_t l = leftIdx(i), r = rightIdx(i);
            if (l < n && m_comp(m_data[best], m_data[l])) best = l;
            if (r < n && m_comp(m_data[best], m_data[r])) best = r;
            if (best == i) break;
            steps.push_back({i, best});
            std::swap(m_data[i], m_data[best]);
            i = best;
        }
        return steps;
    }
};
