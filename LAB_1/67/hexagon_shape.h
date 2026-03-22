#pragma once
#include "polygon_shape.h"

/** @brief Правильный шестиугольник — наследник PolygonShape. */
class HexagonShape : public PolygonShape
{
    Q_OBJECT
public:
    explicit HexagonShape(float radius = 70.0f, QObject* parent = nullptr);

    QString typeName() const override { return "Hexagon"; }

    float radius() const     { return m_radius; }
    void  setRadius(float r) { m_radius = r; rebuildVertices(); }

    // Сторона правильного шестиугольника = радиусу описанной окружности
    float side() const { return m_radius; }

private:
    float m_radius;
    void recompute() override;
};
