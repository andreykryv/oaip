#include "arrayheapviswidget.h"
#include <QPainterPath>
#include <cmath>
#include <algorithm>

static float lerp(float a, float b, float t) { return a + (b - a) * t; }
static QPointF lerp(const QPointF& a, const QPointF& b, float t) {
    return {lerp(a.x(), b.x(), t), lerp(a.y(), b.y(), t)};
}

ArrayHeapVisWidget::ArrayHeapVisWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(400, 300);
    setMouseTracking(true);

    m_animTimer = new QTimer(this);
    m_animTimer->setInterval(1000 / ANIM_FPS);
    connect(m_animTimer, &QTimer::timeout, this, &ArrayHeapVisWidget::animFrame);
    m_animTimer->start();
}

// ════════════════════════════════════════════════════════════════
void ArrayHeapVisWidget::opPush(int val) {
    clearHighlights();
    m_heap.push(val);

    if (m_nodes.find(val) == m_nodes.end()) {
        HeapNodeState s;
        s.value = val;
        s.scale = 0.f;
        s.alpha = 1.f;
        s.glow  = 1.f;
        s.glowColor = QColor(0, 230, 150);
        s.index = (int)m_heap.size() - 1;
        m_nodes[val] = s;
        m_valueToIndex[val] = s.index;
    }

    rebuildLayout();
    emit logMessage(QString("➕ Push: <b>%1</b>  |  Size: %2").arg(val).arg(m_heap.size()));
}

void ArrayHeapVisWidget::opPop() {
    clearHighlights();
    if (m_heap.empty()) {
        emit logMessage("⚠️  Heap is empty");
        return;
    }

    int topVal = m_heap.top();
    m_highlights[topVal] = QColor(255, 60, 80);
    if (m_nodes.count(topVal)) {
        m_nodes[topVal].glow = 1.f;
        m_nodes[topVal].glowColor = QColor(255, 60, 80);
        m_nodes[topVal].markedForDelete = true;
    }

    QTimer::singleShot(400, this, [this, topVal]() {
        m_heap.pop();
        m_nodes.erase(topVal);
        m_valueToIndex.erase(topVal);
        rebuildLayout();
        emit logMessage(QString("🗑️  Pop: <b>%1</b>  |  Size: %2").arg(topVal).arg(m_heap.size()));
    });
}

void ArrayHeapVisWidget::opClear() {
    clearHeap();
    emit logMessage("🗑️  Heap cleared");
}

void ArrayHeapVisWidget::clearHeap() {
    m_heap.clear();
    m_nodes.clear();
    m_valueToIndex.clear();
    m_highlights.clear();
    update();
}

// ════════════════════════════════════════════════════════════════
void ArrayHeapVisWidget::rebuildLayout() {
    auto data = m_heap.rawData();
    int levels = 0;
    int n = (int)data.size();
    while ((1 << levels) <= n) ++levels;

    float totalWidth = width() - 60;
    float levelHeight = LEVEL_GAP;
    float vStart = 50.f;

    for (int i = 0; i < n; ++i) {
        int level = 0;
        int tmp = i + 1;
        while (tmp > 1) { tmp >>= 1; ++level; }

        int nodesAtLevel = 1 << level;
        float levelWidth = std::min(totalWidth, (float)nodesAtLevel * H_GAP * 2);
        float startX = (width() - levelWidth) / 2;
        float hStep = levelWidth / std::max(1, nodesAtLevel);
        int idxInLevel = i - ((1 << level) - 1);

        float tx = startX + idxInLevel * hStep + hStep / 2;
        float ty = vStart + level * levelHeight;

        int val = data[i];
        if (m_nodes.find(val) == m_nodes.end()) {
            HeapNodeState s;
            s.value = val;
            s.index = i;
            s.depth = level;
            s.scale = 0.f;
            s.alpha = 1.f;
            s.glow  = 0.f;
            s.pos = {tx, ty};
            s.targetPos = {tx, ty};
            m_nodes[val] = s;
        } else {
            m_nodes[val].targetPos = {tx, ty};
            m_nodes[val].index = i;
        }
        m_valueToIndex[val] = i;
    }

    // Удаляем узлы, которых нет в куче
    std::vector<int> toRemove;
    for (auto& [k, _] : m_nodes) {
        bool found = false;
        for (int v : data) if (v == k) { found = true; break; }
        if (!found) toRemove.push_back(k);
    }
    for (int k : toRemove) {
        m_nodes.erase(k);
        m_valueToIndex.erase(k);
    }

    update();
}

void ArrayHeapVisWidget::animFrame() {
    bool needUpdate = false;
    for (auto& [val, s] : m_nodes) {
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
void ArrayHeapVisWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0, QColor(5, 7, 18));
    bg.setColorAt(1, QColor(8, 12, 30));
    p.fillRect(rect(), bg);

    // Сетка
    p.setPen(QPen(QColor(20, 30, 60), 1, Qt::SolidLine));
    for (int x = 0; x < width(); x += 40) p.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += 40) p.drawLine(0, y, width(), y);

    if (m_nodes.empty()) {
        p.setPen(QColor(60, 80, 120));
        p.setFont(QFont("JetBrains Mono", 14));
        p.drawText(rect(), Qt::AlignCenter, "Куча пуста\nДобавьте первый элемент →");
        return;
    }

    auto data = m_heap.rawData();

    // Рёбра (родитель → дети)
    for (int i = 0; i < (int)data.size(); ++i) {
        int left = ArrayHeap<int>::leftIdx(i);
        int right = ArrayHeap<int>::rightIdx(i);
        int parentVal = data[i];
        if (!m_nodes.count(parentVal)) continue;

        if (left < (int)data.size()) {
            int childVal = data[left];
            if (m_nodes.count(childVal)) {
                float alpha = std::min(m_nodes[parentVal].alpha, m_nodes[childVal].alpha);
                drawEdge(p, m_nodes[parentVal].pos, m_nodes[childVal].pos, alpha);
            }
        }
        if (right < (int)data.size()) {
            int childVal = data[right];
            if (m_nodes.count(childVal)) {
                float alpha = std::min(m_nodes[parentVal].alpha, m_nodes[childVal].alpha);
                drawEdge(p, m_nodes[parentVal].pos, m_nodes[childVal].pos, alpha);
            }
        }
    }

    // Узлы
    for (auto& [val, s] : m_nodes) {
        drawNode(p, s);
    }
}

void ArrayHeapVisWidget::drawEdge(QPainter& p, const QPointF& from, const QPointF& to, float alpha) {
    QColor edgeCol(50, 100, 180, (int)(alpha * 140));
    p.setPen(QPen(edgeCol, 1.5f));
    p.drawLine(from, to);
}

void ArrayHeapVisWidget::drawNode(QPainter& p, const HeapNodeState& s) {
    float sc = s.scale;
    if (sc < 0.01f) return;

    float r = NODE_R * sc;
    QPointF center = s.pos;

    // Свечение
    if (s.glow > 0.01f) {
        QRadialGradient glow(center, r * 2.5f);
        QColor gc = s.glowColor;
        gc.setAlphaF(s.glow * 0.45f);
        glow.setColorAt(0, gc);
        gc.setAlphaF(0);
        glow.setColorAt(1, gc);
        p.setBrush(glow);
        p.setPen(Qt::NoPen);
        p.drawEllipse(center, r * 2.5f, r * 2.5f);
    }

    // Градиентное тело узла
    QRadialGradient grad(center - QPointF(r * 0.3f, r * 0.3f), r * 1.4f);

    QColor base1, base2, border;
    if (m_highlights.count(s.value)) {
        QColor hc = m_highlights.at(s.value);
        base1 = hc.darker(200);
        base2 = hc.darker(300);
        border = hc;
    } else {
        base1 = QColor(28, 40, 90);
        base2 = QColor(15, 22, 55);
        border = QColor(60, 180, 120);
    }

    grad.setColorAt(0, base1);
    grad.setColorAt(1, base2);
    p.setBrush(grad);
    p.setPen(QPen(border, 1.8f));
    p.drawEllipse(center, r, r);

    // Текст
    float textAlpha = sc * s.alpha;
    QColor textCol(220, 255, 235, (int)(textAlpha * 255));
    p.setPen(textCol);
    QFont f("JetBrains Mono", std::max(7, (int)(10 * sc)), QFont::Bold);
    p.setFont(f);
    QRectF textRect(center.x() - r, center.y() - r, r * 2, r * 2);
    p.drawText(textRect, Qt::AlignCenter, QString::number(s.value));
}

void ArrayHeapVisWidget::resizeEvent(QResizeEvent*) {
    rebuildLayout();
}

void ArrayHeapVisWidget::mousePressEvent(QMouseEvent* e) {
    QPointF pos = e->position();
    for (auto& [val, s] : m_nodes) {
        if ((pos - s.pos).manhattanLength() < NODE_R) {
            clearHighlights();
            m_highlights[val] = QColor(255, 220, 0);
            s.glow = 1.f;
            s.glowColor = QColor(255, 220, 0);
            emit logMessage(QString("👆 Node: <b>%1</b>  |  Index: %2").arg(val).arg(s.index));
            break;
        }
    }
}

void ArrayHeapVisWidget::clearHighlights() {
    m_highlights.clear();
    for (auto& [k, s] : m_nodes) s.glow = 0.f;
}

QPointF ArrayHeapVisWidget::centerOf(int idx) const {
    if (m_nodes.empty()) return {};
    for (auto& [_, s] : m_nodes) {
        if (s.index == idx) return s.pos;
    }
    return {};
}

int ArrayHeapVisWidget::indexAtPosition(QPointF pos) const {
    for (auto& [_, s] : m_nodes) {
        if ((pos - s.pos).manhattanLength() < NODE_R) return s.index;
    }
    return -1;
}
