#include "star_shape.h"
#include <QtMath>

StarShape::StarShape(PointCount pts, float outerR, float innerR,
                     QObject* parent)
    : PolygonShape(parent), m_points(pts), m_outerR(outerR), m_innerR(innerR)
{
    m_color = QColor(255, 200, 0);
    recompute();
}

QString StarShape::typeName() const
{
    switch (m_points) {
    case Star5: return "Star5";
    case Star6: return "Star6";
    case Star8: return "Star8";
    }
    return "Star";
}

void StarShape::recompute()
{
    m_vertices.clear();
    int n = static_cast<int>(m_points);
    // Каждый луч: внешняя вершина + внутренняя вершина между лучами
    for (int i = 0; i < n; ++i) {
        float outerAngle = -M_PI_2 + 2.0 * M_PI * i / n;
        float innerAngle = outerAngle + M_PI / n;

        m_vertices.append(QVector2D(m_outerR * qCos(outerAngle),
                                    m_outerR * qSin(outerAngle)));
        m_vertices.append(QVector2D(m_innerR * qCos(innerAngle),
                                    m_innerR * qSin(innerAngle)));
    }
}
