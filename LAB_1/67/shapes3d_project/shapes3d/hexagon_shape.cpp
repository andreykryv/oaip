#include "hexagon_shape.h"
#include <QtMath>

HexagonShape::HexagonShape(float radius, QObject* parent)
    : PolygonShape(parent), m_radius(radius)
{
    m_color = QColor(0, 180, 160);
    recompute();
}

void HexagonShape::recompute()
{
    m_vertices.clear();
    for (int i = 0; i < 6; ++i) {
        float angle = M_PI / 6.0f + M_PI / 3.0f * i; // 30° + 60°*i
        m_vertices.append(QVector2D(m_radius * qCos(angle),
                                    m_radius * qSin(angle)));
    }
}
