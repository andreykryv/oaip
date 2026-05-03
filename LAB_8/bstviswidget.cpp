#include "bstviswidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QRadialGradient>
#include <QLinearGradient>
#include <cmath>
#include <algorithm>

// Линейная интерполяция
static float lerp(float a, float b, float t) { return a + (b - a) * t; }
static QPointF lerp(QPointF a, QPointF b, float t) {
    return {lerp((float)a.x(),(float)b.x(),t), lerp((float)a.y(),(float)b.y(),t)};
}

// ════════════════════════════════════════════════════════════════
BSTVisWidget::BSTVisWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(400, 300);
    setMouseTracking(true);

    m_animTimer = new QTimer(this);
    m_animTimer->setInterval(1000 / ANIM_FPS);
    connect(m_animTimer, &QTimer::timeout, this, &BSTVisWidget::animFrame);
    m_animTimer->start();

    m_traverseTimer = new QTimer(this);
    m_traverseTimer->setInterval(600);
    connect(m_traverseTimer, &QTimer::timeout, this, &BSTVisWidget::nextTraverseStep);
}

// ════════════════════════════════════════════════════════════════
//  ОПЕРАЦИИ
// ════════════════════════════════════════════════════════════════
void BSTVisWidget::opAdd(int val) {
    clearHighlights();
    m_bst.add(val);

    // Добавляем в визуальное состояние (если нет)
    if (m_nodes.find(val) == m_nodes.end()) {
        VisNodeState s;
        s.value = val;
        s.scale = 0.f;   // начинаем с 0, анимируем до 1
        s.alpha = 1.f;
        s.glow  = 1.f;
        s.glowColor = QColor(0, 230, 255);
        m_nodes[val] = s;
    }

    rebuildLayout();
    emit logMessage(QString("➕ Добавлено: <b>%1</b>  |  Размер: %2").arg(val).arg(m_bst.size()));
}

void BSTVisWidget::opRemove(int val) {
    clearHighlights();
    if (!m_bst.find(val)) {
        emit logMessage(QString("⚠️  Узел <b>%1</b> не найден").arg(val));
        return;
    }

    // Подсвечиваем красным перед удалением
    m_highlights[val] = QColor(255, 60, 80);
    if (m_nodes.count(val)) {
        m_nodes[val].glow = 1.f;
        m_nodes[val].glowColor = QColor(255, 60, 80);
        m_nodes[val].markedForDelete = true;
    }

    QTimer::singleShot(400, this, [this, val]() {
        m_bst.remove(val);
        m_nodes.erase(val);
        m_parents.erase(val);
        m_highlights.erase(val);
        rebuildLayout();
        emit logMessage(QString("🗑️  Удалено: <b>%1</b>  |  Размер: %2").arg(val).arg(m_bst.size()));
    });
}

void BSTVisWidget::opFind(int val) {
    clearHighlights();
    auto trace = m_bst.findTrace(val);
    bool found = (!trace.empty() && trace.back()->data == val);

    // Анимируем путь поиска
    int delay = 0;
    for (size_t i = 0; i < trace.size(); ++i) {
        QTimer::singleShot(delay, this, [this, node=trace[i], i, found, &trace=this->m_traverseSeq]() {
            (void)trace;
            bool isLast = (m_traverseSeq.empty());
            m_highlights[node->data] = QColor(255, 200, 0);
            if (m_nodes.count(node->data)) {
                m_nodes[node->data].glow = 1.f;
                m_nodes[node->data].glowColor = QColor(255, 200, 0);
            }
        });
        delay += 350;
    }

    if (!trace.empty()) {
        QTimer::singleShot(delay, this, [this, val, found, trace]() {
            int last = trace.back()->data;
            QColor finalCol = found ? QColor(0, 255, 150) : QColor(255, 60, 80);
            m_highlights[last] = finalCol;
            if (m_nodes.count(last)) {
                m_nodes[last].glow = 1.5f;
                m_nodes[last].glowColor = finalCol;
            }
            if (found)
                emit logMessage(QString("🔍 Найдено: <b>%1</b>  ✓").arg(val));
            else
                emit logMessage(QString("🔍 Узел <b>%1</b> не найден  ✗").arg(val));
        });
    } else {
        emit logMessage("🔍 Дерево пусто");
    }
}

void BSTVisWidget::opTraverse(TraverseType type) {
    clearHighlights();
    m_traverseTimer->stop();
    m_traverseStep = -1;
    m_traverseSeq.clear();

    std::vector<BSTNode<int>*> seq;
    switch (type) {
        case TraverseType::Inorder:    seq = m_bst.inorderTrace();    break;
        case TraverseType::Preorder:   seq = m_bst.preorderTrace();   break;
        case TraverseType::Postorder:  seq = m_bst.postorderTrace();  break;
        case TraverseType::LevelOrder: seq = m_bst.levelorderTrace(); break;
    }

    if (seq.empty()) { emit logMessage("🌲 Дерево пусто"); return; }

    QStringList vals;
    for (auto* n : seq) vals << QString::number(n->data);
    emit logMessage(QString("🔁 %1: <b>%2</b>").arg(traverseName(type)).arg(vals.join(" → ")));

    startTraverse(seq, QColor(160, 100, 255));
}

void BSTVisWidget::opLCA(int a, int b) {
    clearHighlights();
    auto [visited, lcaNode] = m_bst.lcaTrace(a, b);

    for (auto* n : visited) {
        m_highlights[n->data] = QColor(255, 180, 50);
        if (m_nodes.count(n->data)) {
            m_nodes[n->data].glow = 0.8f;
            m_nodes[n->data].glowColor = QColor(255, 180, 50);
        }
    }

    if (lcaNode) {
        m_highlights[lcaNode->data] = QColor(0, 255, 150);
        if (m_nodes.count(lcaNode->data)) {
            m_nodes[lcaNode->data].glow = 1.5f;
            m_nodes[lcaNode->data].glowColor = QColor(0, 255, 150);
        }
        emit logMessage(QString("🌿 LCA(%1, %2) = <b>%3</b>").arg(a).arg(b).arg(lcaNode->data));
    } else {
        emit logMessage(QString("⚠️  LCA(%1, %2): один или оба узла не существуют").arg(a).arg(b));
    }
}

void BSTVisWidget::opInsertSubtree(int rootVal, std::vector<int> vals) {
    clearHighlights();
    for (int v : vals) opAdd(v);
    emit logMessage(QString("🌳 Вставлено поддерево с корнем <b>%1</b>, %2 узлов")
                        .arg(rootVal).arg(vals.size()));
}

void BSTVisWidget::opRemoveSubtree(int val) {
    clearHighlights();
    if (!m_bst.find(val)) {
        emit logMessage(QString("⚠️  Узел <b>%1</b> не найден").arg(val));
        return;
    }
    // Соберём узлы поддерева для анимации
    auto trace = m_bst.findTrace(val);
    if (!trace.empty()) {
        m_highlights[trace.back()->data] = QColor(255, 60, 80);
    }
    size_t before = m_bst.size();
    m_bst.removeSubtree(val);
    // Удалим из визуального состояния
    std::vector<int> toRemove;
    for (auto& [k, _] : m_nodes) {
        if (!m_bst.find(k)) toRemove.push_back(k);
    }
    for (int k : toRemove) { m_nodes.erase(k); m_parents.erase(k); }
    rebuildLayout();
    emit logMessage(QString("🗑️  Поддерево с корнем <b>%1</b> удалено (%2 узлов)")
                        .arg(val).arg(before - m_bst.size()));
}

void BSTVisWidget::opInsertBranch(int parentVal, int val, bool asLeft) {
    clearHighlights();
    bool ok = m_bst.insertBranch(parentVal, val, asLeft);
    if (ok) {
        VisNodeState s;
        s.value = val;
        s.scale = 0.f;
        s.alpha = 1.f;
        s.glow  = 1.f;
        s.glowColor = QColor(100, 200, 255);
        m_nodes[val] = s;
        rebuildLayout();
        emit logMessage(QString("🌿 Ветвь: добавлен узел <b>%1</b> к родителю <b>%2</b> (%3)")
                            .arg(val).arg(parentVal).arg(asLeft ? "левый" : "правый"));
    } else {
        emit logMessage(QString("⚠️  Не удалось: родитель <b>%1</b> не найден или место занято")
                            .arg(parentVal));
    }
}

void BSTVisWidget::opRemoveBranch(int val) {
    opRemoveSubtree(val);
}

void BSTVisWidget::clearTree() {
    m_bst.clear();
    m_nodes.clear();
    m_parents.clear();
    m_highlights.clear();
    m_traverseTimer->stop();
    m_traverseSeq.clear();
    emit logMessage("🗑️  Дерево очищено");
    update();
}

// ════════════════════════════════════════════════════════════════
//  ВЫЧИСЛЕНИЕ ПОЗИЦИЙ
// ════════════════════════════════════════════════════════════════
void BSTVisWidget::rebuildLayout() {
    std::map<int, std::pair<int,int>> posMap;  // value → (inorderIdx, depth)
    m_parents.clear();

    int counter = 0;
    computePositions(m_bst.root(), 0, counter, posMap);

    if (posMap.empty()) { update(); return; }

    // Масштабирование в пространстве виджета
    float totalNodes = (float)posMap.size();
    float hStep = std::max(MIN_H_GAP, (float)(width() - 80) / std::max(1.f, totalNodes));
    float vStart = 50.f;

    for (auto& [val, pd] : posMap) {
        auto [idx, depth] = pd;
        float tx = 40.f + idx * hStep;
        float ty = vStart + depth * LEVEL_GAP;

        if (m_nodes.find(val) == m_nodes.end()) {
            VisNodeState s;
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

    // Удаляем визуальные узлы, которых нет в дереве
    std::vector<int> toRemove;
    for (auto& [k, _] : m_nodes) {
        if (posMap.find(k) == posMap.end()) toRemove.push_back(k);
    }
    for (int k : toRemove) m_nodes.erase(k);

    // Обновляем родителей
    std::function<void(BSTNode<int>*, int)> fillParents = [&](BSTNode<int>* n, int par) {
        if (!n) return;
        if (par != -1) m_parents[n->data] = par;
        fillParents(n->left.get(), n->data);
        fillParents(n->right.get(), n->data);
    };
    fillParents(m_bst.root(), -1);

    update();
}

void BSTVisWidget::computePositions(BSTNode<int>* node, int depth, int& counter,
                                     std::map<int,std::pair<int,int>>& posMap) {
    if (!node) return;
    computePositions(node->left.get(),  depth + 1, counter, posMap);
    posMap[node->data] = {counter++, depth};
    computePositions(node->right.get(), depth + 1, counter, posMap);
}

// ════════════════════════════════════════════════════════════════
//  АНИМАЦИЯ
// ════════════════════════════════════════════════════════════════
void BSTVisWidget::animFrame() {
    bool needUpdate = false;
    for (auto& [val, s] : m_nodes) {
        // Позиция
        QPointF dp = s.targetPos - s.pos;
        if (dp.manhattanLength() > 0.5f) {
            s.pos = lerp(s.pos, s.targetPos, LERP_SPEED);
            needUpdate = true;
        }
        // Масштаб
        if (s.scale < 1.f) {
            s.scale = std::min(1.f, s.scale + 0.08f);
            needUpdate = true;
        }
        // Свечение угасает
        if (s.glow > 0.f) {
            s.glow = std::max(0.f, s.glow - 0.015f);
            needUpdate = true;
        }
    }
    if (needUpdate) update();
}

void BSTVisWidget::startTraverse(std::vector<BSTNode<int>*> seq, QColor col) {
    m_traverseSeq = seq;
    m_traverseStep = 0;
    for (auto& [k, s] : m_nodes) { s.glow = 0.f; }
    m_highlights.clear();

    m_traverseTimer->start();
}

void BSTVisWidget::nextTraverseStep() {
    if (m_traverseStep < 0 || m_traverseStep >= (int)m_traverseSeq.size()) {
        m_traverseTimer->stop();
        return;
    }
    auto* node = m_traverseSeq[m_traverseStep];
    m_highlights[node->data] = QColor(160, 100, 255);
    if (m_nodes.count(node->data)) {
        m_nodes[node->data].glow = 1.5f;
        m_nodes[node->data].glowColor = QColor(160, 100, 255);
    }
    ++m_traverseStep;
    update();
}

// ════════════════════════════════════════════════════════════════
//  ОТРИСОВКА
// ════════════════════════════════════════════════════════════════
void BSTVisWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Фон
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
        p.drawText(rect(), Qt::AlignCenter, "Дерево пусто\nДобавьте первый узел →");
        return;
    }

    // Рёбра (рисуем до узлов)
    for (auto& [child, parentVal] : m_parents) {
        if (!m_nodes.count(child) || !m_nodes.count(parentVal)) continue;
        auto& cs = m_nodes.at(child);
        auto& ps = m_nodes.at(parentVal);
        float alpha = std::min(cs.alpha, ps.alpha) * std::min(cs.scale, ps.scale);
        drawEdge(p, ps.pos, cs.pos, alpha);
    }

    // Узлы
    for (auto& [val, s] : m_nodes) {
        drawNode(p, s, false, {});
    }
}

void BSTVisWidget::drawEdge(QPainter& p, const QPointF& from, const QPointF& to, float alpha) {
    QColor edgeCol(50, 100, 180, (int)(alpha * 160));
    p.setPen(QPen(edgeCol, 1.5f));

    // Небольшое скругление линии
    QPainterPath path;
    path.moveTo(from);
    QPointF ctrl = {(from.x() + to.x()) / 2, from.y() + 20};
    path.quadTo(ctrl, to);
    p.drawPath(path);
}

void BSTVisWidget::drawNode(QPainter& p, const VisNodeState& s, bool, const QPointF&) {
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

    // Базовый цвет из highlights
    QColor base1, base2, border;
    if (m_highlights.count(s.value)) {
        QColor hc = m_highlights.at(s.value);
        base1 = hc.darker(200);
        base2 = hc.darker(300);
        border = hc;
    } else {
        base1 = QColor(28, 40, 90);
        base2 = QColor(15, 22, 55);
        border = QColor(60, 120, 220);
    }

    grad.setColorAt(0, base1);
    grad.setColorAt(1, base2);
    p.setBrush(grad);

    // Рамка с градиентом
    p.setPen(QPen(border, 1.8f));
    p.drawEllipse(center, r, r);

    // Текст
    float textAlpha = sc * s.alpha;
    QColor textCol(220, 235, 255, (int)(textAlpha * 255));
    p.setPen(textCol);
    QFont f("JetBrains Mono", std::max(7, (int)(10 * sc)), QFont::Bold);
    p.setFont(f);
    QRectF textRect(center.x() - r, center.y() - r, r * 2, r * 2);
    p.drawText(textRect, Qt::AlignCenter, QString::number(s.value));
}

void BSTVisWidget::resizeEvent(QResizeEvent*) {
    rebuildLayout();
}

void BSTVisWidget::mousePressEvent(QMouseEvent* e) {
    // Клик по узлу — подсвечиваем
    QPointF pos = e->position();
    for (auto& [val, s] : m_nodes) {
        if ((pos - s.pos).manhattanLength() < NODE_R) {
            clearHighlights();
            m_highlights[val] = QColor(255, 220, 0);
            s.glow = 1.f;
            s.glowColor = QColor(255, 220, 0);
            emit logMessage(QString("👆 Узел: <b>%1</b>  |  Глубина: %2").arg(val).arg(s.depth));
            break;
        }
    }
}

void BSTVisWidget::clearHighlights() {
    m_highlights.clear();
    for (auto& [k, s] : m_nodes) s.glow = 0.f;
    m_traverseTimer->stop();
    m_traverseSeq.clear();
}

QPointF BSTVisWidget::centerOf(int val) const {
    if (m_nodes.count(val)) return m_nodes.at(val).pos;
    return {};
}

QString BSTVisWidget::traverseName(TraverseType t) const {
    switch (t) {
        case TraverseType::Inorder:    return "Inorder (L-N-R)";
        case TraverseType::Preorder:   return "Preorder (N-L-R)";
        case TraverseType::Postorder:  return "Postorder (L-R-N)";
        case TraverseType::LevelOrder: return "Level-order (BFS)";
    }
    return "";
}
