#include "custom_shape.h"

ArrowShape::ArrowShape(float length, float tailWidth, float headWidth,
                       float headRatio, QObject* parent)
    : PolygonShape(parent)
    , m_length(length), m_tailW(tailWidth)
    , m_headW(headWidth), m_headRatio(headRatio)
{
    m_color = QColor(200, 60, 60);
    recompute();
}

void ArrowShape::recompute()
{
    // Стрелка направлена вправо (+X), центр масс примерно в середине
    float hl    = m_length / 2.0f;
    float htw   = m_tailW  / 2.0f;
    float hhw   = m_headW  / 2.0f;
    float neckX = -hl + m_length * (1.0f - m_headRatio); // x наконечника

    // Вершины по часовой стрелке, начиная с острия
    m_vertices = {
        QVector2D( hl,    0.0f),    // острие
        QVector2D( neckX, hhw),     // верхний угол наконечника
        QVector2D( neckX, htw),     // верхний переход к хвосту
        QVector2D(-hl,    htw),     // верхний левый угол хвоста
        QVector2D(-hl,   -htw),     // нижний левый угол хвоста
        QVector2D( neckX,-htw),     // нижний переход к хвосту
        QVector2D( neckX,-hhw)      // нижний угол наконечника
    };
}
