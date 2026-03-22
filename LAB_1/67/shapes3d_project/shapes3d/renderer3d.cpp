#include "renderer3d.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPolygonF>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>

Renderer3D::Renderer3D(QWidget* parent)
    : QWidget(parent)
    , m_selectedIndex(-1)
    , m_azimuth(30.0f)
    , m_elevation(25.0f)
    , m_zoom(1.0f)
    , m_dragging(false)
{
    setMinimumSize(600, 500);
    setMouseTracking(true);

    // Тёмный фон
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(20, 22, 30));
    setAutoFillBackground(true);
    setPalette(pal);
}

void Renderer3D::addShape(Shape* shape)
{
    m_shapes.append(shape);
    connect(shape, &Shape::changed, this, [this]() { update(); });
    connect(shape, &Shape::transformStep, this, [this]() { update(); });
    update();
}

void Renderer3D::removeShape(Shape* shape)
{
    m_shapes.removeOne(shape);
    update();
}

void Renderer3D::clearShapes()
{
    m_shapes.clear();
    m_selectedIndex = -1;
    update();
}

Shape* Renderer3D::selectedShape()
{
    if (m_selectedIndex >= 0 && m_selectedIndex < m_shapes.size())
        return m_shapes[m_selectedIndex];
    return nullptr;
}

// ── Проекция ────────────────────────────────────────────────────────────────

QPointF Renderer3D::project(float x, float y, float z) const
{
    float azRad  = qDegreesToRadians(m_azimuth);
    float elRad  = qDegreesToRadians(m_elevation);

    // Вращение вокруг оси Z (азимут)
    float rx = x * qCos(azRad) + y * qSin(azRad);
    float ry = -x * qSin(azRad) + y * qCos(azRad);

    // Вращение вокруг оси X (возвышение)
    float rz = z * qCos(elRad) - ry * qSin(elRad);
    float ry2 = z * qSin(elRad) + ry * qCos(elRad);

    // Перспектива
    float focalLen = 600.0f;
    float d = focalLen / (focalLen + ry2 + 1.0f);

    float cx = width()  / 2.0f;
    float cy = height() / 2.0f;

    return QPointF(cx + rx * d * m_zoom,
                   cy - rz * d * m_zoom);
}

// ── Отрисовка ────────────────────────────────────────────────────────────────

void Renderer3D::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Фон
    p.fillRect(rect(), QColor(20, 22, 30));

    // Сетка (grid)
    p.setPen(QPen(QColor(50, 55, 70), 1));
    int gridSize = 40;
    for (int gx = -5; gx <= 5; ++gx) {
        QPointF a = project(gx * gridSize, -5 * gridSize, 0);
        QPointF b = project(gx * gridSize,  5 * gridSize, 0);
        p.drawLine(a, b);
    }
    for (int gy = -5; gy <= 5; ++gy) {
        QPointF a = project(-5 * gridSize, gy * gridSize, 0);
        QPointF b = project( 5 * gridSize, gy * gridSize, 0);
        p.drawLine(a, b);
    }

    // Оси
    drawAxes(p);

    // Фигуры (сначала невыбранные, потом выбранная)
    for (int i = 0; i < m_shapes.size(); ++i) {
        if (i != m_selectedIndex)
            drawShape(p, m_shapes[i], false);
    }
    if (m_selectedIndex >= 0 && m_selectedIndex < m_shapes.size())
        drawShape(p, m_shapes[m_selectedIndex], true);

    // Подсказка
    p.setPen(QColor(120, 130, 150));
    p.setFont(QFont("Arial", 9));
    p.drawText(10, height() - 10, "ЛКМ - вращение  |  Колесо - масштаб  |  Выберите фигуру в списке");
}

void Renderer3D::drawAxes(QPainter& p)
{
    auto drawAxis = [&](float ex, float ey, float ez, QColor color, QString label) {
        QPointF o = project(0, 0, 0);
        QPointF e = project(ex, ey, ez);
        p.setPen(QPen(color, 2));
        p.drawLine(o, e);
        p.setPen(color);
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(e + QPointF(4, -4), label);
    };
    drawAxis(80, 0, 0,  QColor(255, 80, 80),  "X");
    drawAxis(0, 80, 0,  QColor(80, 255, 80),  "Y");
    drawAxis(0, 0, 80,  QColor(80, 120, 255), "Z");
}

void Renderer3D::fillPolygon(QPainter& p, const QVector<QVector2D>& pts,
                              float z, QColor color)
{
    QPolygonF poly;
    for (const auto& pt : pts)
        poly << project(pt.x(), pt.y(), z);
    p.setBrush(QBrush(color));
    p.drawPolygon(poly);
}

void Renderer3D::drawSides(QPainter& p, const QVector<QVector2D>& pts,
                            float zBot, float zTop, QColor color)
{
    int n = pts.size();
    for (int i = 0; i < n; ++i) {
        const QVector2D& a = pts[i];
        const QVector2D& b = pts[(i+1) % n];

        QPolygonF face;
        face << project(a.x(), a.y(), zBot)
             << project(b.x(), b.y(), zBot)
             << project(b.x(), b.y(), zTop)
             << project(a.x(), a.y(), zTop);

        // Освещение: нормаль грани — затемняем в зависимости от ориентации
        float nx = (b.y() - a.y());
        float ny = -(b.x() - a.x());
        float len = qSqrt(nx*nx + ny*ny);
        if (len > 0) { nx /= len; ny /= len; }

        // Проекция нормали на направление "света" (с фронта)
        float azRad = qDegreesToRadians(m_azimuth);
        float lx = qSin(azRad), ly = -qCos(azRad);
        float bright = 0.4f + 0.6f * qMax(0.0f, nx*lx + ny*ly);

        QColor faceColor = color.darker(100 + static_cast<int>(80 * (1.0f - bright)));
        p.setBrush(faceColor);
        p.drawPolygon(face);
    }
}

void Renderer3D::drawShape(QPainter& p, Shape* shape, bool selected)
{
    if (!shape) return;

    QVector<QVector2D> pts = shape->worldOutlinePoints();
    if (pts.isEmpty()) return;

    QVector3D cen = shape->centerOfMass();
    float zBot = cen.z();
    float zTop = cen.z() + shape->extrusionDepth();

    QColor col = shape->color();
    QColor topColor   = col;
    QColor bottomColor = col.darker(130);

    // Рамка выделения
    QPen outlinePen = selected
        ? QPen(QColor(255, 255, 80), 2)
        : QPen(QColor(255,255,255, 60), 1);

    p.setPen(outlinePen);

    // Боковые грани
    drawSides(p, pts, zBot, zTop, col);

    // Нижняя грань
    p.setPen(outlinePen);
    fillPolygon(p, pts, zBot, bottomColor);

    // Верхняя грань (чуть светлее)
    fillPolygon(p, pts, zTop, topColor.lighter(115));

    // Ребра верхней грани
    p.setPen(outlinePen);
    QPolygonF topPoly;
    for (const auto& pt : pts)
        topPoly << project(pt.x(), pt.y(), zTop);
    p.setBrush(Qt::NoBrush);
    p.drawPolygon(topPoly);

    // Подпись
    QPointF labelPos = project(cen.x(), cen.y(), zTop + 10);
    p.setPen(selected ? QColor(255, 255, 80) : QColor(200, 200, 200));
    p.setFont(QFont("Arial", 9));
    p.drawText(labelPos, shape->typeName());
}

// ── Управление камерой ───────────────────────────────────────────────────────

void Renderer3D::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        m_dragging = true;
        m_lastMousePos = e->pos();
    }
}

void Renderer3D::mouseMoveEvent(QMouseEvent* e)
{
    if (m_dragging) {
        QPoint delta = e->pos() - m_lastMousePos;
        m_azimuth   += delta.x() * 0.5f;
        m_elevation += delta.y() * 0.5f;
        m_elevation = qBound(-89.0f, m_elevation, 89.0f);
        m_lastMousePos = e->pos();
        update();
    }
}

void Renderer3D::mouseReleaseEvent(QMouseEvent*)
{
    m_dragging = false;
}

void Renderer3D::wheelEvent(QWheelEvent* e)
{
    float delta = e->angleDelta().y() / 120.0f;
    m_zoom *= (1.0f + delta * 0.1f);
    m_zoom = qBound(0.2f, m_zoom, 5.0f);
    update();
}
