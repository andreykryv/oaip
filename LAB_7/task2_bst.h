#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <queue>
#include <memory>

// ═══════════════════════════════════════════════════════════════
//  Задание 2 — Дерево поиска (Вариант 7)
//  Определить количество символов во всех строках,
//  находящихся в узлах дерева.
// ═══════════════════════════════════════════════════════════════

struct BSTNode {
    int key;
    std::string data;
    BSTNode* left  = nullptr;
    BSTNode* right = nullptr;
    BSTNode(int k, std::string d) : key(k), data(std::move(d)) {}
};

class BST {
public:
    BSTNode* root = nullptr;

    ~BST() { clearNode(root); root = nullptr; }

    // Вставка
    void insert(int key, const std::string& data) {
        root = insertNode(root, key, data);
    }

    // Удаление
    void remove(int key) {
        root = removeNode(root, key);
    }

    // Поиск
    BSTNode* search(int key) const {
        return searchNode(root, key);
    }

    // Балансировка (метод Дэй-Стаут-Уоррена)
    void balance() {
        // Собираем все узлы inorder
        std::vector<std::pair<int,std::string>> nodes;
        inorderCollect(root, nodes);
        clearNode(root); root = nullptr;
        root = buildBalanced(nodes, 0, (int)nodes.size() - 1);
    }

    // Обходы
    void inorder(std::function<void(BSTNode*)> fn) const {
        inorderTraverse(root, fn);
    }
    void preorder(std::function<void(BSTNode*)> fn) const {
        preorderTraverse(root, fn);
    }
    void postorder(std::function<void(BSTNode*)> fn) const {
        postorderTraverse(root, fn);
    }

    // Вариант 7: подсчёт символов во всех строках
    int countAllChars() const {
        int total = 0;
        inorder([&](BSTNode* n){ total += (int)n->data.size(); });
        return total;
    }

    // Для визуализации: BFS-обход уровней
    struct LevelNode { BSTNode* node; int level; int x; };
    std::vector<LevelNode> bfsLayout() const {
        std::vector<LevelNode> result;
        if (!root) return result;
        struct QItem { BSTNode* node; int level; int x; };
        std::queue<QItem> q;
        q.push({root, 0, 0});
        int levelWidth[32] = {};
        while (!q.empty()) {
            auto [n, lv, x] = q.front(); q.pop();
            result.push_back({n, lv, x});
            ++levelWidth[lv];
            if (n->left)  q.push({n->left,  lv+1, x*2});
            if (n->right) q.push({n->right, lv+1, x*2+1});
        }
        return result;
    }

    int height() const { return nodeHeight(root); }

    // Добавить запись
    void addRecord(int key, const std::string& s) { insert(key, s); }

private:
    void clearNode(BSTNode* n) {
        if (!n) return;
        clearNode(n->left);
        clearNode(n->right);
        delete n;
    }

    BSTNode* insertNode(BSTNode* n, int key, const std::string& data) {
        if (!n) return new BSTNode(key, data);
        if (key < n->key) n->left  = insertNode(n->left,  key, data);
        else if (key > n->key) n->right = insertNode(n->right, key, data);
        else n->data = data; // обновляем
        return n;
    }

    BSTNode* minNode(BSTNode* n) {
        while (n->left) n = n->left;
        return n;
    }

    BSTNode* removeNode(BSTNode* n, int key) {
        if (!n) return nullptr;
        if (key < n->key) n->left = removeNode(n->left, key);
        else if (key > n->key) n->right = removeNode(n->right, key);
        else {
            if (!n->left) { auto* r = n->right; delete n; return r; }
            if (!n->right) { auto* l = n->left; delete n; return l; }
            auto* succ = minNode(n->right);
            n->key = succ->key; n->data = succ->data;
            n->right = removeNode(n->right, succ->key);
        }
        return n;
    }

    BSTNode* searchNode(BSTNode* n, int key) const {
        if (!n || n->key == key) return n;
        return key < n->key ? searchNode(n->left, key) : searchNode(n->right, key);
    }

    void inorderCollect(BSTNode* n, std::vector<std::pair<int,std::string>>& v) const {
        if (!n) return;
        inorderCollect(n->left, v);
        v.push_back({n->key, n->data});
        inorderCollect(n->right, v);
    }

    BSTNode* buildBalanced(const std::vector<std::pair<int,std::string>>& v, int lo, int hi) {
        if (lo > hi) return nullptr;
        int mid = (lo + hi) / 2;
        auto* n = new BSTNode(v[mid].first, v[mid].second);
        n->left  = buildBalanced(v, lo, mid-1);
        n->right = buildBalanced(v, mid+1, hi);
        return n;
    }

    void inorderTraverse(BSTNode* n, std::function<void(BSTNode*)>& fn) const {
        if (!n) return;
        inorderTraverse(n->left, fn);
        fn(n);
        inorderTraverse(n->right, fn);
    }
    void preorderTraverse(BSTNode* n, std::function<void(BSTNode*)>& fn) const {
        if (!n) return;
        fn(n);
        preorderTraverse(n->left, fn);
        preorderTraverse(n->right, fn);
    }
    void postorderTraverse(BSTNode* n, std::function<void(BSTNode*)>& fn) const {
        if (!n) return;
        postorderTraverse(n->left, fn);
        postorderTraverse(n->right, fn);
        fn(n);
    }
    int nodeHeight(BSTNode* n) const {
        if (!n) return 0;
        return 1 + std::max(nodeHeight(n->left), nodeHeight(n->right));
    }
};
