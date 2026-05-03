#pragma once
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QPainterPath>
#include <QMouseEvent>
#include <vector>
#include <map>
#include <functional>
#include "listheap.h"

struct ListHeapNodeState {
    QPointF pos;
    QPointF targetPos;
    float   scale = 0.f;
    float   alpha = 1.f;
    float   glow  = 0.f;
    QColor  glowColor;
    int     value = 0;
    int     depth = 0;
    bool    isRoot = false;
};

class ListHeapVisWidget : public QWidget {
    Q_OBJECT
public:
    explicit ListHeapVisWidget(QWidget* parent = nullptr);

    void opPush(int val);
    void opPop();
    void opClear();
    void clearHeap();

    std::function<void(const QString&)> onLogMessage;

signals:
    void logMessage(const QString& msg);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

private:
    static constexpr int   NODE_R       = 24;
    static constexpr float LEVEL_GAP    = 70.f;
    static constexpr float H_GAP        = 50.f;
    static constexpr int   ANIM_FPS     = 60;
    static constexpr float LERP_SPEED   = 0.15f;

    ListHeap<int> m_heap;
    QTimer*       m_animTimer;

    std::map<int, ListHeapNodeState> m_nodes;  // value → state
    std::map<int, QColor> m_highlights;

    void rebuildLayout();
    void animFrame();
    void drawNode(QPainter& p, const ListHeapNodeState& s);
    void drawEdge(QPainter& p, const QPointF& from, const QPointF& to, float alpha);
    void clearHighlights();

    void traverseTree(std::shared_ptr<ListHeapNode<int>> node, int depth, int& counter,
                      std::map<int, std::pair<int,int>>& posMap);
};
