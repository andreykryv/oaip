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
#include "hashtable.h"

struct HashBucketState {
    QRectF rect;
    float  alpha = 1.f;
    float  glow  = 0.f;
    QColor glowColor;
};

struct HashItemState {
    QPointF pos;
    QPointF targetPos;
    float   scale = 0.f;
    float   alpha = 1.f;
    float   glow  = 0.f;
    QColor  glowColor;
    int     key = 0;
    int     value = 0;
    int     bucketIdx = -1;
};

class HashTableVisWidget : public QWidget {
    Q_OBJECT
public:
    explicit HashTableVisWidget(QWidget* parent = nullptr);

    // Операции
    void opInsert(int key, int val);
    void opRemove(int key);
    void opFind(int key);
    void opClear();

    void clearTable();

    std::function<void(const QString&)> onLogMessage;

signals:
    void logMessage(const QString& msg);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

private:
    static constexpr int   BUCKET_H     = 50;
    static constexpr int   BUCKET_W     = 120;
    static constexpr int   ITEM_R       = 18;
    static constexpr int   ANIM_FPS     = 60;
    static constexpr float LERP_SPEED   = 0.15f;

    HashTable<int, int> m_table;
    QTimer*             m_animTimer;

    std::map<int, HashItemState> m_items;  // key → state
    std::vector<HashBucketState> m_buckets;

    std::map<int, QColor> m_highlights;

    void rebuildLayout();
    void animFrame();
    void drawBucket(QPainter& p, int idx, const HashBucketState& s);
    void drawItem(QPainter& p, const HashItemState& s);
    void clearHighlights();

    QPointF itemPosition(int key, int bucketIdx, int chainIdx) const;
};
