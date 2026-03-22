#pragma once
#include "polygon_shape.h"

/**
 * @brief Ромб — наследник PolygonShape (не Rectangle!).
 * Задаётся длиной двух диагоналей d1 (горизонтальная) и d2 (вертикальная).
 */
class RhombusShape : public PolygonShape
{
    Q_OBJECT
public:
    explicit RhombusShape(float d1 = 120.0f, float d2 = 80.0f,
                          QObject* parent = nullptr);

    QString typeName() const override { return "Rhombus"; }

    float diagonal1() const { return m_d1; }
    float diagonal2() const { return m_d2; }
    void setDiagonals(float d1, float d2);

    // Переопределяем площадь (формула d1*d2/2 точнее Гаусса для ромба)
    float area() const override { return m_d1 * m_d2 / 2.0f; }

private:
    float m_d1, m_d2;
    void recompute() override;
};
