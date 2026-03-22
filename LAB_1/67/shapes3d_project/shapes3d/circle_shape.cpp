#include "circle_shape.h"
#include <QtMath>

CircleShape::CircleShape(float radius, QObject* parent)
    : Shape(parent), m_radius(radius), m_segments(64)
{
    m_color = QColor(0, 160, 220);
}

float CircleShape::area() const
{
    return M_PI * m_radius * m_radius;
}

float CircleShape::perimeter() const
{
    return 2.0f * M_PI * m_radius;
}

QVector<QVector2D> CircleShape::localOutlinePoints() const
{
    QVector<QVector2D> pts;
    pts.reserve(m_segments);
    for (int i = 0; i < m_segments; ++i) {
        float angle = 2.0f * M_PI * i / m_segments;
        pts.append(QVector2D(m_radius * qCos(angle),
                             m_radius * qSin(angle)));
    }
    return pts;
}
