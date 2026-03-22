#include "rhombus_shape.h"

RhombusShape::RhombusShape(float d1, float d2, QObject* parent)
    : PolygonShape(parent), m_d1(d1), m_d2(d2)
{
    m_color = QColor(160, 80, 200);
    recompute();
}

void RhombusShape::setDiagonals(float d1, float d2)
{
    m_d1 = d1; m_d2 = d2;
    rebuildVertices();
}

void RhombusShape::recompute()
{
    float hd1 = m_d1 / 2.0f;
    float hd2 = m_d2 / 2.0f;
    m_vertices = {
        QVector2D( hd1,  0.0f),  // right
        QVector2D( 0.0f, hd2),   // top
        QVector2D(-hd1,  0.0f),  // left
        QVector2D( 0.0f,-hd2)    // bottom
    };
}
