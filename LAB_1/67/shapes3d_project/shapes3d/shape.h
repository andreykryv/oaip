#pragma once
#include <QObject>
#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <QColor>
#include <QTimer>
#include <functional>

/**
 * @brief Abstract base class for all 2D shapes renderable in 3D space.
 *
 * Hierarchy:
 *   Shape (abstract)
 *   ├── Circle
 *   └── PolygonShape (abstract)
 *       ├── Triangle
 *       ├── QuadShape (abstract)
 *       │   ├── RectangleShape
 *       │   │   └── SquareShape  (квадрат — отдельный класс, наследует Rectangle
 *       │   │                     с инвариантом w==h; LSP нарушается, см. комментарий)
 *       │   └── RhombusShape
 *       ├── StarShape  (5/6/8-конечная звезда)
 *       ├── HexagonShape
 *       └── ArrowShape  (произвольная фигура)
 *
 * Примечание по LSP: SquareShape наследует RectangleShape, что нарушает
 * принцип подстановки Лисков, т.к. квадрат не позволяет независимо менять
 * ширину и высоту, как прямоугольник. В учебных целях такая иерархия оставлена
 * намеренно, но в реальном проекте их стоит разделить через общий интерфейс.
 */
class Shape : public QObject
{
    Q_OBJECT
public:
    explicit Shape(QObject* parent = nullptr);
    virtual ~Shape() = default;

    // ---- Обязательные методы ----
    virtual float area() const = 0;
    virtual float perimeter() const = 0;
    virtual QString typeName() const = 0;

    /**
     * Возвращает локальные 2D-точки контура (относительно центра масс).
     * Используется для рендеринга и геометрических расчётов.
     */
    virtual QVector<QVector2D> localOutlinePoints() const = 0;

    // ---- Центр масс ----
    QVector3D centerOfMass() const { return m_center; }
    void setCenterOfMass(float x, float y, float z = 0.0f);

    // ---- Трансформации ----
    /** Перемещение фигуры на (dx, dy, dz) */
    void translate(float dx, float dy, float dz = 0.0f);

    /** Поворот вокруг точки pivot на angleDeg градусов (в плоскости XY) */
    void rotateFigure(float angleDeg, QVector2D pivot = QVector2D(0,0));

    /** Масштабирование относительно точки pivot */
    void scaleFigure(float factor, QVector2D pivot = QVector2D(0,0));

    // ---- Анимированные трансформации (с генерацией событий) ----
    void animatedTranslate(float dx, float dy, float dz, int durationMs, int steps = 60);
    void animatedRotate(float angleDeg, QVector2D pivot, int durationMs, int steps = 60);
    void animatedScale(float factor, QVector2D pivot, int durationMs, int steps = 60);
    void stopAnimation();

    bool isAnimating() const { return m_animTimer->isActive(); }

    // ---- Вид / цвет ----
    QColor color() const      { return m_color; }
    void setColor(const QColor& c) { m_color = c; emit changed(); }

    float extrusionDepth() const        { return m_extrusionDepth; }
    void setExtrusionDepth(float d)     { m_extrusionDepth = d; emit changed(); }

    float rotation2D() const { return m_rotation2D; }   // текущий угол поворота в плоскости
    float scale2D() const    { return m_scale2D; }

    // ---- Мировые точки контура (с учётом поворота и масштаба) ----
    QVector<QVector2D> worldOutlinePoints() const;

signals:
    void transformStep();       // генерируется каждый шаг анимации
    void transformFinished();   // генерируется по завершении анимации
    void changed();             // любое изменение параметров

protected:
    QVector3D m_center;
    QColor    m_color;
    float     m_extrusionDepth; // "толщина" 3D-объекта вдоль оси Z
    float     m_rotation2D;     // угол поворота фигуры в её плоскости (градусы)
    float     m_scale2D;        // масштаб

private:
    QTimer*   m_animTimer;
    int       m_animStepsDone;
    int       m_animStepsTotal;
    std::function<void()> m_animStep;

    void setupTimer();

private slots:
    void onAnimTimerTick();
};
