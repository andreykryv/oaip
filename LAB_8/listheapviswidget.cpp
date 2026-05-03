#include "listheapviswidget.h"
#include <QPainterPath>
#include <cmath>
#include <algorithm>

static float lerp(float a, float b, float t) { return a + (b - a) * t; }
static QPointF lerp(const QPointF& a, const QPointF& b, float t) {
    return {lerp(a.x(), b.x(), t), lerp(a.y(), b.y(), t)};
}

ListHeapVisWidget::ListHeapVisWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(400, 300);
    setMouseTracking(true);

    m_animTimer = new QTimer(this);
    m_animTimer->setInterval(1000 / ANIM_FPS);
    connect(m_animTimer, &QTimer::timeout, this, &ListHeapVisWidget::animFrame);
    m_animTimer->start();
}

void ListHeapVisWidget::opPush(int val) {
    clearHighlights();
    m_heap.push(val);

    if (m_nodes.find(val) == m_nodes.end()) {
        ListHeapNodeState s;
        s.value = val;
        s.scale = 0.f;
        s.alpha = 1.f;
        s.glow  = 1.f;
        s.glowColor = QColor(100, 255, 180);
        m_nodes[val] = s;
    }

    rebuildLayout();
    emit logMessage(QString("➕ Push: <b>%1</b>  |  Size: %2").arg(val).arg(m_heap.size()));
}

void ListHeapVisWidget::opPop() {
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
    }

    QTimer::singleShot(400, this, [this, topVal]() {
        m_heap.pop();
        m_nodes.erase(topVal);
        rebuildLayout();
        emit logMessage(QString("🗑️  Pop: <b>%1</b>  |  Size: %2").arg(topVal).arg(m_heap.size()));
    });
}

void ListHeapVisWidget::opClear() {
    clearHeap();
    emit logMessage("🗑️  Heap cleared");
}

void ListHeapVisWidget::clearHeap() {
    m_heap.clear();
    m_nodes.clear();
    m_highlights.clear();
    update();
}

void ListHeapVisWidget::rebuildLayout() {
    std::map<int, std::pair<int,int>> posMap;  // value → (inorderIdx, depth)
    int counter = 0;
    traverseTree(m_heap.rootNode(), 0, counter, posMap);

    if (posMap.empty()) { update(); return; }

    float totalNodes = (float)posMap.size();
    float hStep = std::max(H_GAP, (float)(width() - 80) / std::max(1.f, totalNodes));
    float vStart = 50.f;

    for (auto& [val, pd] : posMap) {
        auto [idx, depth] = pd;
        float tx = 40.f + idx * hStep;
        float ty = vStart + depth * LEVEL_GAP;

        if (m_nodes.find(val) == m_nodes.end()) {
            ListHeapNodeState s;
            s.value = val;
            s.depth = depth;
            s.scale = 0.f;
            s.alpha = 1.f;
            s.glow  = 0.f;
            s.pos = {tx, ty};
            s.targetPos = {tx, ty};
            m_nodes[val] = s;
        } else {
            m_nodes[val].targetPos = {tx, ty};
            m_nodes[val].depth = depth;
        }
    }

    // Удаляем узлы, которых нет в куче
    std::vector<int> toRemove;
    for (auto& [k, _] : m_nodes) {
        bool found = false;
        for (int v : m_heap.levelOrder()) if (v == k) { found = true; break; }
        if (!found) toRemove.push_back(k);
    }
    for (int k : toRemove) m_nodes.erase(k);

    update();
}

void ListHeapVisWidget::traverseTree(std::shared_ptr<ListHeapNode<int>> node, int depth,
                                      int& counter, std::map<int, std::pair<int,int>>& posMap) {
    if (!node) return;
    traverseTree(node->left, depth + 1, counter, posMap);
    posMap[node->value] = {counter++, depth};
    traverseTree(node->right, depth + 1, counter, posMap);
}

void ListHeapVisWidget::animFrame() {
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

void ListHeapVisWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0, QColor(5, 7, 18));
    bg.setColorAt(1, QColor(8, 12, 30));
    p.fillRect(rect(), bg);

    p.setPen(QPen(QColor(20, 30, 60), 1, Qt::SolidLine));
    for (int x = 0; x < width(); x += 40) p.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += 40) p.drawLine(0, y, width(), y);

    if (m_nodes.empty()) {
        p.setPen(QColor(60, 80, 120));
        p.setFont(QFont("JetBrains Mono", 14));
        p.drawText(rect(), Qt::AlignCenter, "Куча пуста\nДобавьте первый элемент →");
        return;
    }

    // Собираем рёбра из структуры кучи
    auto collectEdges = [this](auto self, std::shared_ptr<ListHeapNode<int>> node) -> std::vector<std::pair<int,int>> {
        std::vector<std::pair<int,int>> edges;
        if (!node) return edges;
        if (node->left) {
            edges.push_back({node->value, node->left->value});
            auto childEdges = self(self, node->left);
            edges.insert(edges.end(), childEdges.begin(), childEdges.end());
        }
        if (node->right) {
            edges.push_back({node->value, node->right->value});
            auto childEdges = self(self, node->right);
            edges.insert(edges.end(), childEdges.begin(), childEdges.end());
        }
        return edges;
    };

    auto edges = collectEdges(collectEdges, m_heap.rootNode());

    // Рёбра
    for (auto& [parentVal, childVal] : edges) {
        if (!m_nodes.count(parentVal) || !m_nodes.count(childVal)) continue;
        float alpha = std::min(m_nodes[parentVal].alpha, m_nodes[childVal].alpha);
        drawEdge(p, m_nodes[parentVal].pos, m_nodes[childVal].pos, alpha);
    }

    // Узлы
    for (auto& [val, s] : m_nodes) {
        drawNode(p, s);
    }
}

void ListHeapVisWidget::drawEdge(QPainter& p, const QPointF& from, const QPointF& to, float alpha) {
    QColor edgeCol(50, 100, 180, (int)(alpha * 140));
    p.setPen(QPen(edgeCol, 1.5f));
    p.drawLine(from, to);
}

void ListHeapVisWidget::drawNode(QPainter& p, const ListHeapNodeState& s) {
    float sc = s.scale;
    if (sc < 0.01f) return;

    float r = NODE_R * sc;
    QPointF center = s.pos;

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

    QRadialGradient grad(center - QPointF(r * 0.3f, r * 0.3f), r * 1.4f);

    QColor base1, base2, border;
    if (m_highlights.count(s.value)) {
        QColor hc = m_highlights.at(s.value);
        base1 = hc.darker(200);
        base2 = hc.darker(300);
        border = hc;
    } else {
        base1 = QColor(28, 50, 40);
        base2 = QColor(15, 32, 25);
        border = QColor(60, 200, 140);
    }

    grad.setColorAt(0, base1);
    grad.setColorAt(1, base2);
    p.setBrush(grad);
    p.setPen(QPen(border, 1.8f));
    p.drawEllipse(center, r, r);

    float textAlpha = sc * s.alpha;
    QColor textCol(220, 255, 235, (int)(textAlpha * 255));
    p.setPen(textCol);
    QFont f("JetBrains Mono", std::max(7, (int)(10 * sc)), QFont::Bold);
    p.setFont(f);
    QRectF textRect(center.x() - r, center.y() - r, r * 2, r * 2);
    p.drawText(textRect, Qt::AlignCenter, QString::number(s.value));
}

void ListHeapVisWidget::resizeEvent(QResizeEvent*) {
    rebuildLayout();
}

void ListHeapVisWidget::mousePressEvent(QMouseEvent* e) {
    QPointF pos = e->position();
    for (auto& [val, s] : m_nodes) {
        if ((pos - s.pos).manhattanLength() < NODE_R) {
            clearHighlights();
            m_highlights[val] = QColor(255, 220, 0);
            s.glow = 1.f;
            s.glowColor = QColor(255, 220, 0);
            emit logMessage(QString("👆 Node: <b>%1</b>  |  Depth: %2").arg(val).arg(s.depth));
            break;
        }
    }
}

void ListHeapVisWidget::clearHighlights() {
    m_highlights.clear();
    for (auto& [k, s] : m_nodes) s.glow = 0.f;
}
