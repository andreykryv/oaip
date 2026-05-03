#pragma once
#include <QWidget>
#include <QTimer>
#include <QPointF>
#include <QColor>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include "bst.h"

struct VisNodeState {
    QPointF pos;          // Текущая позиция
    QPointF targetPos;    // Целевая позиция (для анимации)
    float   scale = 0.f;  // 0→1 при вставке
    float   alpha = 1.f;  // 1→0 при удалении
    float   glow  = 0.f;  // 0→1 подсветка
    QColor  glowColor;
    int     value = 0;
    int     depth = 0;
    bool    markedForDelete = false;
};

class BSTVisWidget : public QWidget {
    Q_OBJECT
public:
    enum class TraverseType { Inorder, Preorder, Postorder, LevelOrder };

    explicit BSTVisWidget(QWidget* parent = nullptr);

    // Операции
    void opAdd(int val);
    void opRemove(int val);
    void opFind(int val);
    void opTraverse(TraverseType type);
    void opLCA(int a, int b);
    void opInsertSubtree(int rootVal, std::vector<int> vals);
    void opRemoveSubtree(int val);
    void opInsertBranch(int parentVal, int val, bool asLeft);
    void opRemoveBranch(int val);

    void clearTree();
    BST<int>* bst() { return &m_bst; }

    std::function<void(const QString&)> onLogMessage;

signals:
    void logMessage(const QString& msg);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

private:
    static constexpr int   NODE_R       = 26;
    static constexpr float LEVEL_GAP    = 80.f;
    static constexpr float MIN_H_GAP    = 60.f;
    static constexpr int   ANIM_FPS     = 60;
    static constexpr float LERP_SPEED   = 0.12f;

    BST<int>       m_bst;
    QTimer*        m_animTimer;

    // Позиции из BST-структуры
    std::map<int, VisNodeState> m_nodes;   // value → state

    // Трассировка обхода
    std::vector<BSTNode<int>*> m_traverseSeq;
    int                         m_traverseStep = -1;
    QTimer*                     m_traverseTimer = nullptr;

    // Выделенные узлы: value→цвет
    std::map<int, QColor> m_highlights;

    void rebuildLayout();
    void computePositions(BSTNode<int>* node, int depth, int& counter,
                          std::map<int,std::pair<int,int>>& posMap);
    void animFrame();
    void startTraverse(std::vector<BSTNode<int>*> seq, QColor col);
    void nextTraverseStep();
    void drawNode(QPainter& p, const VisNodeState& s, bool drawEdge,
                  const QPointF& parentPos);
    void drawEdge(QPainter& p, const QPointF& from, const QPointF& to, float alpha);
    void clearHighlights();

    QPointF centerOf(int val) const;
    QString traverseName(TraverseType t) const;

    // Для рёбер: родители
    std::map<int,int> m_parents;  // child value → parent value

    // Анимируемые узлы (ожидающие удаления)
    std::set<int> m_pendingDelete;
};
