#pragma once
// ╔══════════════════════════════════════════════════════════════╗
// ║  Map<K,V> и Set<K> — шаблонные контейнеры на основе BST     ║
// ╚══════════════════════════════════════════════════════════════╝
#include "bst.h"
#include <utility>
#include <optional>
#include <stdexcept>

// ── Пара ключ-значение для Map ───────────────────────────────────
template<typename K, typename V>
struct MapPair {
    K key;
    mutable V value;  // mutable: итератор const может менять value

    explicit MapPair(const K& k) : key(k), value{} {}
    MapPair(const K& k, const V& v) : key(k), value(v) {}

    bool operator<(const MapPair& o) const { return key < o.key; }
    bool operator>(const MapPair& o) const { return key > o.key; }
    bool operator==(const MapPair& o) const { return key == o.key; }
    bool operator!=(const MapPair& o) const { return key != o.key; }
};

// ════════════════════════════════════════════════════════════════
//  Map<KeyType, ValueType>
// ════════════════════════════════════════════════════════════════
template<typename KeyType, typename ValueType>
class Map {
    using Pair = MapPair<KeyType, ValueType>;
    using Tree = BST<Pair>;
    using Node = BSTNode<Pair>;

public:
    // ── Итератор тип 1: через BST-логику ──────────────────────
    class Iterator1 {
    public:
        explicit Iterator1(BSTIterator<Pair> it) : m_it(it) {}

        std::pair<const KeyType&, ValueType&> operator*() {
            return {(*m_it).key, (*m_it).value};
        }
        Iterator1& operator++() { ++m_it; return *this; }
        bool operator==(const Iterator1& o) const { return m_it == o.m_it; }
        bool operator!=(const Iterator1& o) const { return m_it != o.m_it; }
        Node* ptr() const { return m_it.ptr(); }
    private:
        BSTIterator<Pair> m_it;
    };

    // ── Итератор тип 2: через двусвязный inorder-список ───────
    class Iterator2 {
    public:
        explicit Iterator2(ListIterator<Pair> it) : m_it(it) {}

        std::pair<const KeyType&, ValueType&> operator*() {
            return {(*m_it).key, (*m_it).value};
        }
        Iterator2& operator++() { ++m_it; return *this; }
        bool operator==(const Iterator2& o) const { return m_it == o.m_it; }
        bool operator!=(const Iterator2& o) const { return m_it != o.m_it; }
        Node* ptr() const { return m_it.ptr(); }
    private:
        ListIterator<Pair> m_it;
    };

    // ── Основные операции ──────────────────────────────────────
    void insert(const KeyType& key, const ValueType& val) {
        Pair p(key, val);
        if (m_tree.find(p)) {
            // Обновляем значение
            auto* node = m_tree.find(p);
            node->data.value = val;
        } else {
            m_tree.add(p);
        }
        m_tree.rebuildInorderList();
    }

    bool erase(const KeyType& key) {
        return m_tree.remove(Pair(key));
    }

    bool contains(const KeyType& key) const {
        return m_tree.find(Pair(key)) != nullptr;
    }

    std::optional<ValueType> get(const KeyType& key) const {
        auto* node = m_tree.find(Pair(key));
        if (!node) return std::nullopt;
        return node->data.value;
    }

    ValueType& operator[](const KeyType& key) {
        Pair p(key);
        auto* node = m_tree.find(p);
        if (!node) {
            m_tree.add(p);
            m_tree.rebuildInorderList();
            node = m_tree.find(p);
        }
        return node->data.value;
    }

    size_t size() const { return m_tree.size(); }
    bool empty() const { return m_tree.empty(); }
    void clear() { m_tree.clear(); }

    const Tree& tree() const { return m_tree; }

    // ── Итераторы begin/end ────────────────────────────────────
    Iterator1 begin1() const {
        return Iterator1(BSTIterator<Pair>(m_tree.firstInorder(), m_tree.root()));
    }
    Iterator1 end1() const {
        return Iterator1(BSTIterator<Pair>(nullptr, m_tree.root()));
    }

    Iterator2 begin2() const {
        return Iterator2(ListIterator<Pair>(m_tree.firstInorder()));
    }
    Iterator2 end2() const {
        return Iterator2(ListIterator<Pair>(nullptr));
    }

    // Все ключи в порядке возрастания
    std::vector<std::pair<KeyType, ValueType>> toVector() const {
        std::vector<std::pair<KeyType, ValueType>> result;
        auto ord = m_tree.inorder();
        for (auto& p : ord) result.push_back({p.key, p.value});
        return result;
    }

private:
    Tree m_tree;
};

// ════════════════════════════════════════════════════════════════
//  Set<KeyType>  ≡  Map<KeyType, char> (фиктивное Value = '\0')
// ════════════════════════════════════════════════════════════════
template<typename KeyType>
class Set {
    using UnderlyingMap = Map<KeyType, char>;

public:
    using Iterator1 = typename UnderlyingMap::Iterator1;
    using Iterator2 = typename UnderlyingMap::Iterator2;

    void insert(const KeyType& key)   { m_map.insert(key, '\0'); }
    bool erase(const KeyType& key)    { return m_map.erase(key); }
    bool contains(const KeyType& key) const { return m_map.contains(key); }
    size_t size()  const { return m_map.size(); }
    bool empty()   const { return m_map.empty(); }
    void clear()         { m_map.clear(); }

    std::vector<KeyType> toVector() const {
        std::vector<KeyType> r;
        for (auto& p : m_map.toVector()) r.push_back(p.first);
        return r;
    }

    Iterator1 begin1() const { return m_map.begin1(); }
    Iterator1 end1()   const { return m_map.end1(); }
    Iterator2 begin2() const { return m_map.begin2(); }
    Iterator2 end2()   const { return m_map.end2(); }

    const UnderlyingMap& map() const { return m_map; }

private:
    UnderlyingMap m_map;
};
