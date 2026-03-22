#include "triangle_shape.h"
#include <QtMath>

TriangleShape::TriangleShape(float side, QObject* parent)
    : PolygonShape(parent)
{
    m_color = QColor(80, 200, 120);
    init(side);
}

TriangleShape::TriangleShape(QVector2D a, QVector2D b, QVector2D c,
                             QObject* parent)
    : PolygonShape(parent)
{
    m_color = QColor(80, 200, 120);
    m_vertices = {a, b, c};
}

void TriangleShape::init(float side)
{
    // Равносторонний треугольник, центр масс в начале координат
    float h = side * qSqrt(3.0f) / 2.0f;
    // Вершины относительно центра масс (h/3 от основания, 2h/3 от вершины)
    m_vertices = {
        QVector2D(0.0f,       2.0f * h / 3.0f),   // вершина
        QVector2D(-side/2.0f, -h / 3.0f),          // левый угол
        QVector2D( side/2.0f, -h / 3.0f)           // правый угол
    };
}

float TriangleShape::sideA() const
{
    if (m_vertices.size() < 3) return 0;
    return (m_vertices[1] - m_vertices[2]).length();
}
float TriangleShape::sideB() const
{
    if (m_vertices.size() < 3) return 0;
    return (m_vertices[0] - m_vertices[2]).length();
}
float TriangleShape::sideC() const
{
    if (m_vertices.size() < 3) return 0;
    return (m_vertices[0] - m_vertices[1]).length();
}
float TriangleShape::height() const
{
    // h = 2*S / a
    float a = sideA();
    return a > 0 ? 2.0f * area() / a : 0.0f;
}
