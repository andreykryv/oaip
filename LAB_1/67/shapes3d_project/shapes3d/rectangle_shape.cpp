#include "rectangle_shape.h"

RectangleShape::RectangleShape(float width, float height, QObject* parent)
    : PolygonShape(parent), m_width(width), m_height(height)
{
    m_color = QColor(220, 100, 80);
    recompute();
}

void RectangleShape::recompute()
{
    float hw = m_width  / 2.0f;
    float hh = m_height / 2.0f;
    m_vertices = {
        QVector2D(-hw, -hh),
        QVector2D( hw, -hh),
        QVector2D( hw,  hh),
        QVector2D(-hw,  hh)
    };
}

void RectangleShape::setWidth(float w)
{
    m_width = w;
    rebuildVertices();
}

void RectangleShape::setHeight(float h)
{
    m_height = h;
    rebuildVertices();
}

void RectangleShape::setSize(float w, float h)
{
    m_width  = w;
    m_height = h;
    rebuildVertices();
}
