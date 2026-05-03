#include "hashtableviswidget.h"
#include <QPainterPath>
#include <cmath>
#include <algorithm>

static float lerp(float a, float b, float t) { return a + (b - a) * t; }
static QPointF lerp(const QPointF& a, const QPointF& b, float t) {
    return {lerp(a.x(), b.x(), t), lerp(a.y(), b.y(), t)};
}

HashTableVisWidget::HashTableVisWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(500, 350);
    setMouseTracking(true);

    m_animTimer = new QTimer(this);
    m_animTimer->setInterval(1000 / ANIM_FPS);
    connect(m_animTimer, &QTimer::timeout, this, &HashTableVisWidget::animFrame);
    m_animTimer->start();
}

// ════════════════════════════════════════════════════════════════
void HashTableVisWidget::opInsert(int key, int val) {
    clearHighlights();
    m_table.insert(key, val);

    if (m_items.find(key) == m_items.end()) {
        HashItemState s;
        s.key = key;
        s.value = val;
        s.scale = 0.f;
        s.alpha = 1.f;
        s.glow  = 1.f;
        s.glowColor = QColor(100, 200, 255);
        m_items[key] = s;
    } else {
        m_items[key].value = val;
        m_items[key].glow = 1.f;
        m_items[key].glowColor = QColor(100, 200, 255);
    }

    rebuildLayout();
    emit logMessage(QString("➕ Insert: <b>%1 → %2</b>  |  Size: %3, Load: %4")
                        .arg(key).arg(val).arg(m_table.size())
                        .arg(QString::number(m_table.loadFactor(), 'f', 2)));
}

void HashTableVisWidget::opRemove(int key) {
    clearHighlights();
    if (!m_table.contains(key)) {
        emit logMessage(QString("⚠️  Key <b>%1</b> not found").arg(key));
        return;
    }

    m_highlights[key] = QColor(255, 60, 80);
    if (m_items.count(key)) {
        m_items[key].glow = 1.f;
        m_items[key].glowColor = QColor(255, 60, 80);
    }

    QTimer::singleShot(400, this, [this, key]() {
        m_table.erase(key);
        m_items.erase(key);
        m_highlights.erase(key);
        rebuildLayout();
        emit logMessage(QString("🗑️  Removed: <b>%1</b>  |  Size: %2").arg(key).arg(m_table.size()));
    });
}

void HashTableVisWidget::opFind(int key) {
    clearHighlights();
    bool found = m_table.contains(key);

    if (found) {
        m_highlights[key] = QColor(0, 255, 150);
        if (m_items.count(key)) {
            m_items[key].glow = 1.5f;
            m_items[key].glowColor = QColor(0, 255, 150);
        }
        auto opt = m_table.get(key);
        emit logMessage(QString("🔍 Found: <b>%1 → %2</b> ✓").arg(key).arg(opt.value_or(-1)));
    } else {
        emit logMessage(QString("🔍 Key <b>%1</b> not found ✗").arg(key));
    }
}

void HashTableVisWidget::opClear() {
    clearTable();
    emit logMessage("🗑️  Table cleared");
}

void HashTableVisWidget::clearTable() {
    m_table.clear();
    m_items.clear();
    m_buckets.clear();
    m_highlights.clear();
    update();
}

// ════════════════════════════════════════════════════════════════
void HashTableVisWidget::rebuildLayout() {
    size_t cap = m_table.capacity();
    m_buckets.resize(cap);

    int cols = std::max(1, width() / (BUCKET_W + 20));
    int rows = (cap + cols - 1) / cols;
    int startX = 20;
    int startY = 30;
    int gapX = 15;
    int gapY = 80;

    for (size_t i = 0; i < cap; ++i) {
        int col = i % cols;
        int row = i / cols;
        int bx = startX + col * (BUCKET_W + gapX);
        int by = startY + row * gapY;

        m_buckets[i].rect = QRectF(bx, by, BUCKET_W, BUCKET_H);
    }

    // Распределяем элементы по бакетам
    auto bucketsInfo = m_table.getBuckets();
    for (auto& bi : bucketsInfo) {
        int chainIdx = 0;
        for (auto& [key, val] : bi.items) {
            int bx = (int)m_buckets[bi.index].rect.x();
            int by = (int)m_buckets[bi.index].rect.y();
            float tx = bx + 25 + chainIdx * 35;
            float ty = by + BUCKET_H / 2;

            if (m_items.find(key) == m_items.end()) {
                HashItemState s;
                s.key = key;
                s.value = val;
                s.bucketIdx = (int)bi.index;
                s.scale = 0.f;
                s.pos = {tx, ty};
                s.targetPos = {tx, ty};
                m_items[key] = s;
            } else {
                m_items[key].targetPos = {tx, ty};
                m_items[key].bucketIdx = (int)bi.index;
            }
            ++chainIdx;
        }
    }

    // Удаляем узлы, которых нет в таблице
    std::vector<int> toRemove;
    for (auto& [k, _] : m_items) {
        if (!m_table.contains(k)) toRemove.push_back(k);
    }
    for (int k : toRemove) m_items.erase(k);

    update();
}

void HashTableVisWidget::animFrame() {
    bool needUpdate = false;
    for (auto& [key, s] : m_items) {
        QPointF dp = s.targetPos - s.pos;
        if (dp.manhattanLength() > 0.5f) {
            s.pos = lerp(s.pos, s.targetPos, LERP_SPEED);
            needUpdate = true;
        }
        if (s.scale < 1.f) {
            s.scale = std::min(1.f, s.scale + 0.08f);
            needUpdate = true;
        }
        if (s.glow > 0.f) {
            s.glow = std::max(0.f, s.glow - 0.02f);
            needUpdate = true;
        }
    }
    if (needUpdate) update();
}

// ════════════════════════════════════════════════════════════════
void HashTableVisWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0, QColor(5, 7, 18));
    bg.setColorAt(1, QColor(8, 12, 30));
    p.fillRect(rect(), bg);

    if (m_buckets.empty()) {
        p.setPen(QColor(60, 80, 120));
        p.setFont(QFont("JetBrains Mono", 14));
        p.drawText(rect(), Qt::AlignCenter, "Таблица пуста\nДобавьте первый элемент →");
        return;
    }

    // Бакеты
    for (size_t i = 0; i < m_buckets.size(); ++i) {
        drawBucket(p, (int)i, m_buckets[i]);
    }

    // Элементы
    for (auto& [key, s] : m_items) {
        drawItem(p, s);
    }
}

void HashTableVisWidget::drawBucket(QPainter& p, int idx, const HashBucketState& s) {
    QRectF r = s.rect;

    // Фон бакета
    QLinearGradient grad(r.topLeft(), r.bottomLeft());
    grad.setColorAt(0, QColor(20, 25, 50));
    grad.setColorAt(1, QColor(12, 18, 40));
    p.setBrush(grad);

    QColor border = m_highlights.empty() ? QColor(40, 50, 90) : QColor(50, 70, 120);
    p.setPen(QPen(border, 1.2f));
    p.drawRoundedRect(r, 6, 6);

    // Индекс бакета
    p.setPen(QColor(120, 140, 180));
    QFont f("JetBrains Mono", 10, QFont::Bold);
    p.setFont(f);
    p.drawText(r.adjusted(4, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, QString("[%1]").arg(idx));
}

void HashTableVisWidget::drawItem(QPainter& p, const HashItemState& s) {
    float sc = s.scale;
    if (sc < 0.01f) return;

    float r = ITEM_R * sc;
    QPointF center = s.pos;

    // Свечение
    if (s.glow > 0.01f) {
        QRadialGradient glow(center, r * 2.5f);
        QColor gc = s.glowColor;
        gc.setAlphaF(s.glow * 0.5f);
        glow.setColorAt(0, gc);
        gc.setAlphaF(0);
        glow.setColorAt(1, gc);
        p.setBrush(glow);
        p.setPen(Qt::NoPen);
        p.drawEllipse(center, r * 2.5f, r * 2.5f);
    }

    // Тело элемента
    QRadialGradient grad(center - QPointF(r * 0.3f, r * 0.3f), r * 1.4f);

    QColor base1, base2, border;
    if (m_highlights.count(s.key)) {
        QColor hc = m_highlights.at(s.key);
        base1 = hc.darker(200);
        base2 = hc.darker(300);
        border = hc;
    } else {
        base1 = QColor(35, 50, 100);
        base2 = QColor(20, 30, 70);
        border = QColor(80, 140, 200);
    }

    grad.setColorAt(0, base1);
    grad.setColorAt(1, base2);
    p.setBrush(grad);
    p.setPen(QPen(border, 1.5f));
    p.drawEllipse(center, r, r);

    // Текст (ключ)
    float textAlpha = sc * s.alpha;
    QColor textCol(230, 245, 255, (int)(textAlpha * 255));
    p.setPen(textCol);
    QFont f("JetBrains Mono", std::max(7, (int)(9 * sc)), QFont::Bold);
    p.setFont(f);
    QRectF textRect(center.x() - r, center.y() - r, r * 2, r * 2);
    p.drawText(textRect, Qt::AlignCenter, QString::number(s.key));
}

void HashTableVisWidget::resizeEvent(QResizeEvent*) {
    rebuildLayout();
}

void HashTableVisWidget::mousePressEvent(QMouseEvent* e) {
    QPointF pos = e->position();
    for (auto& [key, s] : m_items) {
        if ((pos - s.pos).manhattanLength() < ITEM_R * 1.5f) {
            clearHighlights();
            m_highlights[key] = QColor(255, 220, 0);
            s.glow = 1.f;
            s.glowColor = QColor(255, 220, 0);
            emit logMessage(QString("👆 Item: <b>%1 → %2</b>  |  Bucket: %3")
                                .arg(key).arg(s.value).arg(s.bucketIdx));
            break;
        }
    }
}

void HashTableVisWidget::clearHighlights() {
    m_highlights.clear();
    for (auto& [k, s] : m_items) s.glow = 0.f;
}

QPointF HashTableVisWidget::itemPosition(int key, int bucketIdx, int chainIdx) const {
    if (m_items.count(key)) return m_items.at(key).pos;
    return {};
}
