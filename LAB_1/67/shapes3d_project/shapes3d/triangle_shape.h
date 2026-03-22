#pragma once
#include "polygon_shape.h"

/**
 * @brief Треугольник — наследник PolygonShape.
 * Задаётся тремя вершинами в локальных координатах.
 */
class TriangleShape : public PolygonShape
{
    Q_OBJECT
public:
    /** Равносторонний треугольник по умолчанию */
    explicit TriangleShape(float side = 80.0f, QObject* parent = nullptr);

    /** Треугольник с произвольными вершинами */
    TriangleShape(QVector2D a, QVector2D b, QVector2D c,
                  QObject* parent = nullptr);

    QString typeName() const override { return "Triangle"; }

    // Стороны
    float sideA() const;  // BC
    float sideB() const;  // AC
    float sideC() const;  // AB

    // Высота из вершины A
    float height() const;

private:
    void init(float side);
};
