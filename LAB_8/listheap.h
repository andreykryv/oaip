#pragma once
// ╔══════════════════════════════════════════════════════════════╗
// ║  БИНАРНАЯ КУЧА НА ОСНОВЕ СПИСКА — Динамическая библиотека   ║
// ║  Использует std::weak_ptr для указателя на родителя          ║
// ╚══════════════════════════════════════════════════════════════╝
#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include <stdexcept>

// ── Узел кучи ────────────────────────────────────────────────────
template<typename T>
struct ListHeapNode {
    T value;
    std::shared_ptr<ListHeapNode<T>> left;
    std::shared_ptr<ListHeapNode<T>> right;
    std::weak_ptr<ListHeapNode<T>> parent;   // weak_ptr — нет цикла!

    explicit ListHeapNode(const T& v) : value(v) {}
};

// ── Бинарная куча на основе связанного дерева ────────────────────
template<typename T, typename Compare = std::less<T>>
class ListHeap {
public:
    using NodePtr = std::shared_ptr<ListHeapNode<T>>;

    void push(const T& val) {
        auto newNode = std::make_shared<ListHeapNode<T>>(val);
        if (!m_root) {
            m_root = newNode;
        } else {
            // Находим позицию для вставки (по уровню: m_size + 1)
            NodePtr parent = getNodeAt(m_size);  // индекс родителя: (m_size+1-1)/2 = m_size/2... нет
            // На самом деле для позиции m_size+1 (1-based), родитель — позиция (m_size+1)/2
            NodePtr p = getNodeAt((m_size) / 2);  // 0-based parent
            newNode->parent = p;
            if (!p->left)  p->left  = newNode;
            else           p->right = newNode;
        }
        ++m_size;
        siftUp(newNode);
    }

    const T& top() const {
        if (!m_root) throw std::underflow_error("ListHeap: heap is empty");
        return m_root->value;
    }

    void pop() {
        if (!m_root) throw std::underflow_error("ListHeap: heap is empty");
        if (m_size == 1) { m_root = nullptr; m_size = 0; return; }

        // Последний узел (позиция m_size - 1, 0-based)
        NodePtr last = getNodeAt(m_size - 1);
        m_root->value = last->value;

        // Удаляем последний узел
        auto parentWeak = last->parent;
        if (auto p = parentWeak.lock()) {
            if (p->right == last) p->right = nullptr;
            else                  p->left  = nullptr;
        }
        --m_size;
        siftDown(m_root);
    }

    bool   empty() const noexcept { return m_size == 0; }
    size_t size()  const noexcept { return m_size; }
    void   clear() noexcept { m_root = nullptr; m_size = 0; }

    NodePtr rootNode() const { return m_root; }

    // Обход по уровням (для визуализации)
    std::vector<T> levelOrder() const {
        std::vector<T> result;
        if (!m_root) return result;
        std::queue<NodePtr> q;
        q.push(m_root);
        while (!q.empty()) {
            auto node = q.front(); q.pop();
            result.push_back(node->value);
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        return result;
    }

    // Получить узел по 0-based позиции в BFS-порядке
    NodePtr getNodeAt(size_t idx) const {
        if (!m_root || idx >= m_size) return nullptr;
        // Двоичный путь: бит за битом от MSB к LSB
        size_t pos = idx + 1;  // 1-based
        int bits = 0;
        size_t tmp = pos >> 1;
        while (tmp) { ++bits; tmp >>= 1; }

        NodePtr cur = m_root;
        for (int b = bits - 1; b >= 0 && cur; --b) {
            bool goRight = (pos >> b) & 1;
            cur = goRight ? cur->right : cur->left;
        }
        return cur;
    }

private:
    NodePtr m_root;
    size_t  m_size = 0;
    Compare m_comp;

    void siftUp(NodePtr node) {
        while (true) {
            auto p = node->parent.lock();
            if (!p) break;
            if (m_comp(p->value, node->value)) {
                std::swap(p->value, node->value);
                node = p;
            } else break;
        }
    }

    void siftDown(NodePtr node) {
        while (node) {
            NodePtr best = node;
            if (node->left  && m_comp(best->value, node->left->value))  best = node->left;
            if (node->right && m_comp(best->value, node->right->value)) best = node->right;
            if (best == node) break;
            std::swap(node->value, best->value);
            node = best;
        }
    }
};
