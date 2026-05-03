#pragma once
// ╔══════════════════════════════════════════════════════════════╗
// ║  Binary Search Tree (BST) — Template Implementation          ║
// ║  Supports: add, remove, find, traversals, LCA, subtree ops   ║
// ╚══════════════════════════════════════════════════════════════╝
#include <memory>
#include <vector>
#include <functional>
#include <queue>
#include <algorithm>
#include <optional>

template<typename T>
struct BSTNode {
    T data;
    std::shared_ptr<BSTNode<T>> left;
    std::shared_ptr<BSTNode<T>> right;
    std::weak_ptr<BSTNode<T>> parent;

    explicit BSTNode(const T& val) : data(val) {}
};

template<typename T>
class BST {
public:
    using NodePtr = std::shared_ptr<BSTNode<T>>;

    BST() = default;

    // ── Основные операции ────────────────────────────────────────
    void add(const T& val) {
        if (!m_root) {
            m_root = std::make_shared<BSTNode<T>>(val);
        } else {
            insertRecursive(m_root, val);
        }
        ++m_size;
    }

    bool remove(const T& val) {
        auto node = findNode(m_root, val);
        if (!node) return false;
        removeNode(val);
        --m_size;
        return true;
    }

    BSTNode<T>* find(const T& val) const {
        return findNodeRaw(m_root.get(), val);
    }

    // Путь поиска для анимации
    std::vector<BSTNode<T>*> findTrace(const T& val) const {
        std::vector<BSTNode<T>*> trace;
        auto* cur = m_root.get();
        while (cur) {
            trace.push_back(cur);
            if (val == cur->data) break;
            else if (val < cur->data) cur = cur->left.get();
            else cur = cur->right.get();
        }
        return trace;
    }

    // ── Обходы ───────────────────────────────────────────────────
    std::vector<BSTNode<T>*> inorderTrace() const {
        std::vector<BSTNode<T>*> result;
        inorderCollect(m_root.get(), result);
        return result;
    }

    std::vector<BSTNode<T>*> preorderTrace() const {
        std::vector<BSTNode<T>*> result;
        preorderCollect(m_root.get(), result);
        return result;
    }

    std::vector<BSTNode<T>*> postorderTrace() const {
        std::vector<BSTNode<T>*> result;
        postorderCollect(m_root.get(), result);
        return result;
    }

    std::vector<BSTNode<T>*> levelorderTrace() const {
        std::vector<BSTNode<T>*> result;
        if (!m_root) return result;
        std::queue<NodePtr> q;
        q.push(m_root);
        while (!q.empty()) {
            auto node = q.front(); q.pop();
            result.push_back(node.get());
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        return result;
    }

    std::vector<T> inorder() const {
        std::vector<T> result;
        inorderValueCollect(m_root.get(), result);
        return result;
    }

    // ── LCA (Lowest Common Ancestor) ─────────────────────────────
    struct LCAResult {
        std::vector<BSTNode<T>*> visited;
        BSTNode<T>* lca = nullptr;
    };

    LCAResult lcaTrace(const T& a, const T& b) const {
        LCAResult res;
        if (!m_root) return res;

        auto pathTo = [this](const T& val) -> std::vector<BSTNode<T>*> {
            std::vector<BSTNode<T>*> path;
            auto* cur = m_root.get();
            while (cur) {
                path.push_back(cur);
                if (val == cur->data) break;
                else if (val < cur->data) cur = cur->left.get();
                else cur = cur->right.get();
            }
            return path;
        };

        auto pathA = pathTo(a);
        auto pathB = pathTo(b);

        BSTNode<T>* lca = nullptr;
        size_t i = 0;
        while (i < pathA.size() && i < pathB.size() && pathA[i] == pathB[i]) {
            lca = pathA[i];
            res.visited.push_back(pathA[i]);
            ++i;
        }
        res.lca = lca;
        return res;
    }

    // ── Поддерево ────────────────────────────────────────────────
    void removeSubtree(const T& val) {
        auto node = findNode(m_root, val);
        if (!node) return;
        removeNode(val);
    }

    bool insertBranch(const T& parentVal, const T& childVal, bool asLeft) {
        auto parent = findNode(m_root, parentVal);
        if (!parent) return false;
        if (asLeft && parent->left) return false;
        if (!asLeft && parent->right) return false;

        auto child = std::make_shared<BSTNode<T>>(childVal);
        child->parent = parent;
        if (asLeft) parent->left = child;
        else parent->right = child;
        ++m_size;
        return true;
    }

    // ── Информация ───────────────────────────────────────────────
    size_t size() const noexcept { return m_size; }
    bool empty() const noexcept { return m_size == 0; }

    BSTNode<T>* root() const { return m_root.get(); }

    void clear() {
        m_root = nullptr;
        m_size = 0;
    }

private:
    NodePtr m_root;
    size_t m_size = 0;

    void insertRecursive(NodePtr& node, const T& val) {
        if (val < node->data) {
            if (!node->left) {
                node->left = std::make_shared<BSTNode<T>>(val);
                node->left->parent = node;
            } else {
                insertRecursive(node->left, val);
            }
        } else if (val > node->data) {
            if (!node->right) {
                node->right = std::make_shared<BSTNode<T>>(val);
                node->right->parent = node;
            } else {
                insertRecursive(node->right, val);
            }
        }
        // Если равно — не вставляем (дубликаты запрещены)
    }

    NodePtr findNode(NodePtr node, const T& val) const {
        if (!node) return nullptr;
        if (val == node->data) return node;
        else if (val < node->data) return findNode(node->left, val);
        else return findNode(node->right, val);
    }

    BSTNode<T>* findNodeRaw(BSTNode<T>* node, const T& val) const {
        if (!node) return nullptr;
        if (val == node->data) return node;
        else if (val < node->data) return findNodeRaw(node->left.get(), val);
        else return findNodeRaw(node->right.get(), val);
    }

    void removeNode(const T& val) {
        if (!m_root) return;

        // Находим узел и его родителя
        NodePtr current = findNode(m_root, val);
        if (!current) return;

        auto parentWeak = current->parent;
        NodePtr parent = parentWeak.lock();

        // Случай 1: лист
        if (!current->left && !current->right) {
            if (!parent) {
                m_root = nullptr;
            } else if (parent->left == current) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
        }
        // Случай 2: один ребёнок
        else if (!current->left) {
            transplant(parent, current, current->right);
        } else if (!current->right) {
            transplant(parent, current, current->left);
        }
        // Случай 3: два ребёнка
        else {
            auto successor = findMin(current->right);
            current->data = successor->data;
            auto succParentWeak = successor->parent;
            auto succParent = succParentWeak.lock();
            if (succParent) {
                if (succParent->left == successor)
                    succParent->left = successor->right;
                else
                    succParent->right = successor->right;
                if (successor->right)
                    successor->right->parent = succParent;
            }
            --m_size;
            return;
        }
        --m_size;
    }

    void transplant(NodePtr parent, NodePtr oldNode, NodePtr newNode) {
        if (!parent) {
            m_root = newNode;
            if (newNode) newNode->parent = {};
        } else if (parent->left == oldNode) {
            parent->left = newNode;
            if (newNode) newNode->parent = parent;
        } else {
            parent->right = newNode;
            if (newNode) newNode->parent = parent;
        }
    }

    NodePtr findMin(NodePtr node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    void inorderCollect(BSTNode<T>* node, std::vector<BSTNode<T>*>& result) const {
        if (!node) return;
        inorderCollect(node->left.get(), result);
        result.push_back(node);
        inorderCollect(node->right.get(), result);
    }

    void preorderCollect(BSTNode<T>* node, std::vector<BSTNode<T>*>& result) const {
        if (!node) return;
        result.push_back(node);
        preorderCollect(node->left.get(), result);
        preorderCollect(node->right.get(), result);
    }

    void postorderCollect(BSTNode<T>* node, std::vector<BSTNode<T>*>& result) const {
        if (!node) return;
        postorderCollect(node->left.get(), result);
        postorderCollect(node->right.get(), result);
        result.push_back(node);
    }

    void inorderValueCollect(BSTNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        inorderValueCollect(node->left.get(), result);
        result.push_back(node->data);
        inorderValueCollect(node->right.get(), result);
    }
};
