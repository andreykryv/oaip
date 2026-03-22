#pragma once
#include <QString>

// Абстрактный базовый класс для всех фигур 
class Shape3D {
public:
    virtual ~Shape3D() = default;
    virtual double  area()       const = 0;
    virtual double  perimeter()  const = 0;
    virtual QString typeName()   const = 0;
    virtual void translate(float dx, float dy, float dz) = 0;
    virtual void rotateDeg(float angleDeg, float ax, float ay, float az) = 0;
    virtual void setScale(float s) = 0;
};
