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
#include <set>
#include <functional>
#include "arrayheap.h"

struct HeapNodeState {
    QPointF pos;          // Текущая позиция
    QPointF targetPos;    // Целевая позиция
    float   scale = 0.f;  // 0→1 при вставке
    float   alpha = 1.f;  // Прозрачность
    float   glow  = 0.f;  // Свечение
    QColor  glowColor;
    int     value = 0;
    int     index = -1;   // Индекс в массиве кучи
    int     depth = 0;    // Уровень в дереве
    bool    markedForDelete = false;
};

class ArrayHeapVisWidget : public QWidget {
    Q_OBJECT
public:
    explicit ArrayHeapVisWidget(QWidget* parent = nullptr);

    // Операции
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

    ArrayHeap<int> m_heap;
    QTimer*        m_animTimer;

    std::map<int, HeapNodeState> m_nodes;  // value → state
    std::map<int, int>           m_valueToIndex;  // value → heap index

    // Для анимации sift
    struct SiftAnim {
        int fromIdx, toIdx;
        int step = 0;
        int totalSteps = 10;
        bool active = false;
    };
    SiftAnim m_siftAnim;

    std::map<int, QColor> m_highlights;

    void rebuildLayout();
    void animFrame();
    void drawNode(QPainter& p, const HeapNodeState& s);
    void drawEdge(QPainter& p, const QPointF& from, const QPointF& to, float alpha);
    void clearHighlights();

    QPointF centerOf(int idx) const;
    int indexAtPosition(QPointF pos) const;
};
