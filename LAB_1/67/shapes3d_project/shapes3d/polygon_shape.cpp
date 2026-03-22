#include "polygon_shape.h"
#include <QtMath>

PolygonShape::PolygonShape(QObject* parent)
    : Shape(parent)
{}

float PolygonShape::area() const
{
    // Формула Гаусса (shoelace formula)
    float sum = 0.0f;
    int n = m_vertices.size();
    for (int i = 0; i < n; ++i) {
        const QVector2D& a = m_vertices[i];
        const QVector2D& b = m_vertices[(i + 1) % n];
        sum += a.x() * b.y() - b.x() * a.y();
    }
    return qAbs(sum) * 0.5f;
}

float PolygonShape::perimeter() const
{
    float p = 0.0f;
    int n = m_vertices.size();
    for (int i = 0; i < n; ++i) {
        p += (m_vertices[(i+1)%n] - m_vertices[i]).length();
    }
    return p;
}

void PolygonShape::setVertices(const QVector<QVector2D>& v)
{
    m_vertices = v;
    emit changed();
}

void PolygonShape::setVertex(int i, QVector2D v)
{
    if (i >= 0 && i < m_vertices.size()) {
        m_vertices[i] = v;
        emit changed();
    }
}

void PolygonShape::rebuildVertices()
{
    recompute();
    emit changed();
}
