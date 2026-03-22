#pragma once
#include "rectangle_shape.h"

/**
 * @brief Квадрат — наследник RectangleShape с инвариантом width == height.
 *
 * Нарушение LSP: метод setWidth/setHeight устанавливает обе стороны одновременно,
 * что делает квадрат не «настоящим» подтипом прямоугольника.
 * Оставлено намеренно как учебный пример; в продакшн-коде Square и Rectangle
 * следует объединить через общий интерфейс Quadrilateral.
 */
class SquareShape : public RectangleShape
{
    Q_OBJECT
public:
    explicit SquareShape(float side = 70.0f, QObject* parent = nullptr);

    QString typeName() const override { return "Square"; }

    float side() const { return m_width; }
    void  setSide(float s);

    // LSP-нарушение: setWidth и setHeight принудительно синхронизируют обе стороны
    void setWidth(float w)  override { setSide(w); }
    void setHeight(float h) override { setSide(h); }
    void setSize(float w, float /*h*/) override { setSide(w); }
};
