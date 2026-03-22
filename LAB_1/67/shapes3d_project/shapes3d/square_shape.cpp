#include "square_shape.h"

SquareShape::SquareShape(float side, QObject* parent)
    : RectangleShape(side, side, parent)
{
    m_color = QColor(255, 160, 0);
}

void SquareShape::setSide(float s)
{
    m_width  = s;
    m_height = s;
    rebuildVertices();
}
